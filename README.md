# SPI-STM32-FPGA-DDS

## Aim

### Definitly MAKING A DDS with ARM+FPGA

Try my best to finish the task of : **2023寒假一起练 - 基于STM32+iCE40的电赛训练平台**

I choose the task of **making a DDS** with **STM32G031+iCE40** development kit made by *eetree*.

To figure **the detailed Project Discription**, visit:

[2023寒假一起练平台（2）- 基于STM32+iCE40的电赛训练平台](https://www.eetree.cn/project/detail/1349)

## Feature

- [x] Amazing GUI with Encoder & Keys
  - [x] GUI framework made by myself
  > Thanks to the hardwork of [afiskon/stm32-ssd1306](https://github.com/afiskon/stm32-ssd1306.git)
- [x] SPI Commu with FPGA
  - [x] SPI Slave not using IP core but verilog to simu it
  > Thanks to the hardwork of [damdoy/ice40_ultraplus_examples](https://github.com/damdoy/ice40_ultraplus_examples)
- [ ] Base DDS Func

## Hardware

- [电赛扩展板 x1](https://www.eetree.cn/project/detail/251)
- [DAC模块 x1](https://www.eetree.cn/project/detail/62)
- [STM32+iCE40核心板 x1](https://www.eetree.cn/project/detail/7)

## Software

- STM32:
  - CubeMX 6.7.0
  - Keil uVision 5 + ARM_CC 5.06
  - CubeProgrammer(Use UART To program device)
- FPGA:
  - Lattice Radiant Software
  - Lattice IP Core(PLL)
- distributed version control :
  - GIT: you can see my every change

## Usage

### 项目3 - DDS任意波形发生器/本地控制

具体要求：

> 通过板上的高速DAC（10bits/最高125Msps）配合FPGA内部DDS的逻辑（最高48Msps），生成波形可调（正弦波、三角波、方波）、频率可调、幅度可调的波形
>
> 生成模拟信号的频率范围为DC-5MHz，调节精度为1Hz
>
> 生成模拟信号的幅度为最大1Vpp，调节范围为0.1V-1V
>
> 在OLED上显示当前波形的形状、波形的频率以及幅度
利用板上旋转编码器和按键能够对波形进行切换、进行参数调节

### License

It's defined in the [LICENSE](LICENSE) file.
