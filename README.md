# AD7606
## 一.工程说明
该工程基于NUCLEO-G474RE和中瑞科技制作的AD7606数模采集模块，利用HAL库coding的驱动代码。
## 二.AD7606的主要特征：
16位8通道双极性采样，采样范围可通过改变模块的电阻改变为+-5v或+-10v  
外置基准源2.5v，200 kSPS，一般用于直流或几十HZ的信号  
（更多详情见AD7606芯片手册和模块用户手册）  
## 三.主要引脚配制:
DB7(DoutA)(GPIO_INPUT)-->PC9
DB8(DoutB)(GPIO_INPUT)-->PA7   
ST_A(CONVST A)-->PA11  
ST_B(CONVST B)--> PA12 
RST(RESET)-->PB8    
BUSY-->PB9  
CS-->PB11  
CLK(RD/SCLK)-->PB12  
OS0-->PC5  
OS1-->PC6  
OS2-->PC8
## 四.主要函数
### void AD7606_Init(uint8_t os )  
参数：设置过采样模式
功能：AD7606初始化
返回值：无
### void AD7606_RANGE10(float vol_data[])
参数：返回8通道电压值数组
功能：采样范围为+-10v的采样，转换以及数据处理，最终生成电压值数组
返回值：无


