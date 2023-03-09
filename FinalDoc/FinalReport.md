<!--
 * @Description: 
 * @Author: MALossov
 * @Date: 2023-03-07 21:31:45
 * @LastEditTime: 2023-03-09 21:11:35
 * @LastEditors: MALossov
 * @Reference: 
-->
# 2023寒假在家练 - 基于STM32+iCE40的电赛训练平台实现的本地控制的DDS信号发生器

[TOC]

## 一、项目介绍

本次使用的项目使用基于STM32+iCE40的电赛训练平台完成。
本次使用的板卡载有ICE40UP5K FPGA和STM32G031 MCU，使用FPGA控制高速DAC产生波形，使用MCU控制OLED屏幕和读取按键、旋转编码器输入并进行处理。
完成的任务为：

**项目3 - DDS任意波形发生器/本地控制**

1. 通过板上的高速DAC（10bits/最高125Msps）配合FPGA内部DDS的逻辑（最高48Msps），生成波形可调（正弦波、三角波、方波）、频率可调（DC-）、幅度可调的波形
2. 生成模拟信号的频率范围为DC-5MHz，调节精度为1Hz
3. 生成模拟信号的幅度为最大1Vpp，调节范围为0.1V-1V
4. 在OLED上显示当前波形的形状、波形的频率以及幅度
5. 利用板上旋转编码器和按键能够对波形进行切换、进行参数调节

## 二、设计思路

### 总览

1. 本次的设计主要分为两部分，一部分是FPGA部分，一部分是MCU部分。
   1. FPGA部分：
      1. 参考开源代码设计SPI软核，与STM32进行交互，实现FPGA与MCU的通信。
      2. 使用LUT查找表进行FPGA相关的计算，实现DDS的逻辑。通过添加多张查找表实现多种波形的生成。
      3. 通过将查找表的值进行位运算并且补齐，产生输出信号，实现高速DAC的输出和幅度调节。
      4. 使用LED灯指示FPGA的工作状态。
   2. MCU部分：
      1. 使用2个定时器进行旋转编码器的读取，实现旋转编码器的输入。为旋转编码器和按键添中断，实现按键的输入和准确的读取，并且不影响主程序。
      2. 自行添加显示结构体，实现OLED的显示框架设置，避免频繁地调用底层函数，同时通过结构体方便进行显示的刷新和显示的管理。
      3. 使用SPI与FPGA进行通信，实现FPGA与MCU的通信。为SPI通信编制协议和编解码函数，实现数据的传输、校验等。

### SPI通信部分

> 由于代码太长，这里暂时不放出状态机代码，详细代码请见以下部分和附件。

1. SPI通信协议如下：将通信分为4个byte，首位CMD位，仅由STM32主机向FPGA从机发送，第二、三、四位均为数据位，通过读取CMD位，借助SPI全双工的优势，在剩下的3位当中可以进行读取、写入等操作：

    | byte  |   0   |   1   |   2   |   3   |
    | :---: | :---: | :---: | :---: | :---: |
    | 命令  | 数据  | 数据  | 数据  | 数据  |
2. 实现指令的逻辑为：由两部分实现一整个SPI的子模块，
   1. 一个模块为SPI的具体逻辑模块：负责实现MISO和MOSI总线上的操作，由SCLK线上升沿决定下一个状态该如何操作，具体操作为：当SCLK上升沿时，若为主机发送数据，则将数据从MOSI总线上读取，若为从机发送数据，则将数据从MISO总线上读取，同时将数据写入到SPI的寄存器中，以便于主机读取。
   2. 另一个模块为SPI顶层模块，由一个状态机先判断收到的8位是否为特定CMD位，若为特定CMD位，则根据CMD位的值进行相应的操作，若不为CMD位，则根据状态机的状态进行相应的操作。
   3. 如下为我实现的指令：

    | 命令  |                                                   功能                                                   |
    | :---: | :------------------------------------------------------------------------------------------------------: |
    | 0x00  |                                   空指令，什么也不做，即略过剩下的操作                                   |
    | 0x01  |                                 初始化SPI通信，状态机由IDLE转为WAIT_CMD                                  |
    | 0X02  |                  向FPGA写入后24位并且取反(~)，将MOSI总线上读到的数据位移存储到寄存器中                   |
    | 0X03  |                     从FPGA读出写入的24位地址，将寄存器中的数据位移由MISO取最高位发送                     |
    | 0X04  |              通过SPI改变FPGA板上灯的颜色，即读取MOSI位移数据中最后三位并赋值给连接LED的REG               |
    |  0x5  |                        读取LED状态，即为将LED REG当中的值写入发送寄存器的最后一位                        |
    |  0x6  | 接下来连续发送4*24位的数据(CMD位留空并调用4次发送)；即后四次FPGA均为读出寄存器数组数据，并位移由MISO发送 |
    |  0x7  |    接下来连续读入4*24位的数据(CMD位留空并调动4次接收)；即后四次FPGA均为读入数据，并存入对应寄存器数组    |

    > 0x6 和 0x7 命令的目标是发送和接收分段为多个的值 尽可能快地打包。由于数据以最快的方式发送，因此在 fpga 端，这是寄存器上的测试，指示 空闲缓冲区，在主机端，这是对发送的元数据的测试 SPI 数据包。
    > 此实现的一大缺点是主机必须发送读取请求 然后通过在SPI线路上发送NOP来读取数据。改进可能是做 请求和阅读同时进行，但我们希望保持简单。

3. SPI的校验：借助全双工总线，当主机读取SPI数据包时，前三个字节是数据 最后一个字节是状态字节，它只有七位。(即为：在MISO总线上，数据的排列为先数据，后状态，他们的含义为：`bit 7 - 数据已经成功从FPGA被写出；bit 6 - 数据已经从FPGA被写入`)因此，在主机检验时，只需要检验最后一位byte即可，并在读出和读入指令时校验值并不相同，例如，检验主机写入从机时，检测函数为：`#define STATUS_FPGA_RECV_MASK (0x1<<STATUS_FPGA_RECV_OFFSET) //OFFSET为6`。如果校验失败，则进行重新发送指令即可。

4. 借助这些基础函数，我们可以向FPGA中新建新的指令（即为开辟新的寄存器并存入数据），实现快速存入/读取频率、幅度、波形信息；并且封装为C语言函数由STM32实现。接下来，发送波形部分则可以直接对齐这些寄存器地址即可实现。

### OLED显示部分

1. OLED显示部分考虑到由于没有硬件SPI的加持，因此使用了软件SPI，即通过GPIO模拟SPI总线。具体实现为通过位运算和FOR循环改变MISO的电平，从而实现SPI的通信。具体实现类似为：

   ```c
    void OLED_WriteByte(uint8_t dat)
    {
        uint8_t i;
        for(i=0;i<8;i++)
        {
            OLED_SCLK_L;
            if(dat&0x80)
                OLED_SDIN_H;
            else
                OLED_SDIN_L;
            OLED_SCLK_H;
            dat<<=1;
        }
    }
   ```

2. 底层通信实现后，发现由于底板设计原因，GPIO速率太快时，OLED显示屏容易出现闪屏、显示失位的问题，通过阅读SSD1306的手册和常见驱动，最终选择了一款能够较为轻易实现全刷新的驱动，实现的方法为：**将屏幕内容保存在MCU的本地数组中，每次全刷新屏幕时，将数组的全部内容写入OLED中这样可以实现115Hz的全刷新率，远远高于使用中景园驱动！！！**
   1. 而如果使用中景园驱动，由于其采用的是分屏刷新，因此刷新率只有30Hz左右，且由于其采用的是分屏刷新，因此在刷新时更容易会出现闪屏的情况，因此，我们选择了一种更为轻易实现的方法，即为全屏刷新。同时由于本地保留了全部的屏幕信息，因此可以通过一次全刷新指令，实现屏幕的清屏操作；而不用通过重新发送全部的初始化指令。

3. 结构体封装：
   1. 以标题封装为例，我们将标题封装为一个字符串指针和它的位置，例如：

    ```c
        typedef struct LabelText {
        char* Title;
        uint8_t pos_x, pos_y;
    }LabelText;
    ```

    2. 因此，我们可以通过它的`x--横坐标的位置`和`y--行数`确定开始写入屏幕时的x和y，之后直接写入字符串即可，例如：

```c
//! Menu Item update funcs(Without screen display)

void updateLabels(LabelText Label, uint8_t row)
{
    Label.pos_x = Title_x(Label.Title);
    Label.pos_y = Index_y(row);
    ssd1306_SetCursor(Label.pos_x, Label.pos_y);
    ssd1306_WriteString(Label.Title, Font_7x10, White);
}
...
// 之后调用update即可
```

4. 最终，我们创建了一个大的menu的结构体，并将所有的标签、数值等都保存在这个结构体当中，之后只需要编写函数，遍历这个结构体当中的每一个子项，并且更新这些子项并完成屏幕的更新即可，结构体如下：

    ```c
    typedef struct MenuList {
        DDS_Wave WaveForm;  //当前的波形
        SwitchController Switches;  //按键控制结构体（定义所有按键）
        LabelText Label[5]; //每一项的标签
        DataText Data[4];   //每一项的数值
        flagType switchFlg, encFlg, Inited; //按键和编码器的标志位
        ChoosenBar Choose;  //当前选择的选项
        DDS_State DDS_STATE;    //当前DDS的状态
    } MenuList;
    ```

5. 之后，我们只需要在主循环中，调用这个结构体、标签的、值的更新函数即可。

### 编码器和按键部分

1. 编码器部分：编码器部分的实现主要是通过中断来实现的，当编码器的A相和B相的电平发生变化时，就会触发中断，从而实现编码器的读取。具体实现为：
   1. 将旋转编码器的A脚和B脚均设置为上拉输入，当旋转时，A脚和B脚的电平会发生变化，从而触发中断。
   2. 如结构体所示意，将要检查旋转编码器的A相和B相的电平，因此需要定义两个枚举类型，分别为A相和B相的电平，以及编码器的状态，分别为A相上升沿、B相上升沿、编码器空闲。
   3. 具体实现如下：

   ```c
    typedef struct {
    enum B_Pin_Set { B_Low, B_High } A_Rising;
    enum B_Pin_Set A_Falling;
    enum A_Pin_Stat { A_Rised, A_Finished, EncIDLE } A_State;

    }Encoder_TypeDef;
    Encoder_TypeDef Encoder = { B_Low, B_High, EncIDLE };
    ```

    4. 一开始我们采用的是单次中断查询编码器的状态，但是由于编码器的旋转速度较快，因此会导致编码器的状态无法及时更新，因此我们采用了双次中断的方式：
       1. 即在A相上升沿中断时，检查B相的电平，如果B相的电平为高，则编码器的状态为A相上升沿，如果B相的电平为低，则编码器的状态为B相上升沿。
       2. 之后在B相上升沿中断时，检查A相的电平，如果A相的电平为高，则编码器的状态为B相上升沿，如果A相的电平为低，则编码器的状态为A相上升沿，这样就可以实现编码器的状态的及时更新。
2. 按键和编码器的显示更新：
    1. 以编码器为例，我们最终将编码器的状态保存在结构体当中，并在该结构体中值被改变时检查该值，最后实现相应的屏幕更新，并在更新屏幕的同时改变DDS的状态：


   ```c
   char* ButtonMsgs[5] = { "SW1_Pressed","SW2_Pressed","Enc_Pressed","Enc_Clock","Enc_AntiClock" };
   ...
   //! Function Callback Funcs, called by IO
    void callBackingBtns(void)
    {
        updateSTATE(DDS_Menu_Modifying);
        if (Menu.encFlg) {
            switch (Menu.Switches.Encoder)
            {
            case ClockWise:
                updateButtonMsgs(Menu.Switches.ButtonMsgs[3]);
                Menu.Switches.SwitchAction = Increase;
                break;
            ...
            }
        }
        ssd1306_UpdateScreen();
    }
    
    void updateButtonMsgs(ButtonMsg* ButtonMsg)
    {
        uint8_t pos_x = Button_x(ButtonMsg);

        ssd1306_FillRectangle(0, 54, 128, 64, White);
        ssd1306_SetCursor(pos_x, 55);
        ssd1306_WriteString(ButtonMsg, Font_6x8, Black);
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

    }
   ```

### DDS实现

1. DDS的内部寄存器：

## 三、框图和软件流程图




## 四、简单的硬件介绍

## 五、实现的功能及图片展示

## 六、主要代码片段及说明

## 七、遇到的主要难题及解决方法

## 八、未来的计划或建议

## 附录：FPGA资源占用报告
