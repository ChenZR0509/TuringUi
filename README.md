# TuringUi

---

>- **CzrTuringB：**主要用以记录STM32各种外设的学习过程，并结合其功能设计了一款OledUi框架，**菜鸡一个，大佬的话可以自行忽略掉**。
>- **博客：**[czrturingb.top](http://www.czrturingb.top/)

---

## 第一章	特性

- 主控：STM32F429IGT6(野火的一个STM32最小系统板)，原理图我放在了Library目录下
- 实时操作系统：FreeRtos
- OLED外设：
  - 分辨率：128×64
  - 芯片：ssd1306
  - 通信接口：IIC

## 第二章	TuringUi

>- 快速IIC+DMA传输
>- 基于CRC冗余检验算法的区域块刷新技术
>- 四像素模拟灰度
>- 基于Bresenham算法的任意角度直线、圆、椭圆的绘制算法
>- 基于背向面法的三维消隐算法
>- 基于贝塞尔曲线的Oled缓动动画实现
