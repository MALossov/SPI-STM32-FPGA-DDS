<!--
 * @Description: 
 * @Author: MALossov
 * @Date: 2023-03-07 21:31:45
 * @LastEditTime: 2023-03-07 21:45:26
 * @LastEditors: MALossov
 * @Reference: 
-->
# 2023寒假在家练 - 基于STM32+iCE40的电赛训练平台实现的本地控制的DDS信号发生器

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

## 三、框图和软件流程图

## 四、简单的阴间介绍

## 五、实现的功能及图片展示

## 六、主要代码片段及说明

## 七、遇到的主要难题及解决方法

## 八、未来的计划或建议

## 附录：FPGA资源占用报告