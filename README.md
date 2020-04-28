# Keil_STM32duino
Compile and debug stm32dino using keil integrated development environment

在Keil中编译、调试STM32Duino
## 1. 获取STM32duino源代码
在Arduino IDE中添加ST官方库STM32duino
如果使用Arduino IDE下载成功了STM32Duino的扩展包，则在C:\Users\youe user name\AppData\Local\Arduino15\packages下有本项目的全部代码和编译工具。
### 1.1 STM32duino扩展包文件内容
在C:\Users\youe user name\AppData\Local\Arduino15\packages这个文件夹下有如下的路径
```
STM32                      
  --hardware
  --tools
```
其中hardware文件夹下是项目的源代码，tools文件夹下是编译工具
```
hardware                    ;
   --stm32                  ;stm32文件夹
     --1.7.0                ;1.7.0版
       --CI                 ;命令行工具，暂时不使用
       --core               ;STM32Duino核心代码
       --libraries          ;项目中自带的库
       --system             ;stm32抽象库（HAL库）
       --variants           ;项目支持的芯片的各种变种
```
## 2.建立Keil的项目
我们新建一个文件夹stm32duino，将hardware，tools文件夹的全部内容复制这个文件夹下.再新建一个文件夹mdk，用于存放Keil的项目文件。再在这个文件夹下建立build文件夹，保存编译时产生的*.o,*.d文件.
```
stm32duino
  --hardware             ;copy as same from C:\Users\youe user name\AppData\Local\Arduino15\packages\STM32\hardware
  --tools                ;copy as same from C:\Users\youe user name\AppData\Local\Arduino15\packages\STM32\tools
  --mdk                  ;新建文件夹保存MDK项目文件
    --build              ;新建文件夹保存编译时产生的obj，lst文件
```
本项目以stm32F103ve为例，项目需要的文件在variants--PILL_F103XX文件夹下。
### 2.1建立项目的代码分组
```
  --User              ;用户的应用程序
  --core              ;STM32Duino的核心文件  
  --variants          ;变种文件
  --libraries         ;Arduino库
```
### 2.2添加文件到项目
#### 2.2.1 USer组
在User组中存放用用户的应用程序，*.cpp
#### 2.2.2 core组
在core组中添加core.lib,syscall.c
> Keil IDE对GCC的支持不是很好，所以完全按照STM32duino原来源代码添加到Keil时，能够编译，个别的CPP文件并不能链接成功。例如HardwareSerial.cpp编译后链接时报错。
#### 2.2.3 variants组
MCU的变种文件是根据一个特定的MCU编写的，每一种具体的STM32 MCU都有一个变种文件保存在variants文件夹下，stm32duino 1.7.0版支持60余种MCU的变种。
本文以STM32F103为例，选择PILL_F103XX文件夹下的变种文件，共需要添加两个文件，一个是PeripheralPins.c，一个是variant.cpp。
需要使用其他型号的STM32 MCU时换成其他的变种文件即可。


## 3.Keil的一些必要设置
### 3.1 编译器设置
点击Keil的project-->Manage-->Components Environment Books...菜单，打开Keil的Manage Project Item对话框，进入Folders/Extension选项卡下，勾选Use GCC，在GNU-Tool-Prefix中输入arm-none-eabi-,在GNU-Tool-Folder输入X:\stm32duino\tools\arm-none-eabi-gcc\8.2.1-1.7\
### 3.1.1 CC选项卡
点击project-->Options for Target 'Target 1'菜单，进入CC选项卡
#### 3.1.1.1 需要定义的宏
在Options for Target 'Target 1'的CC选项卡中，在Define中定义下面的宏。

编译项目前需要定义STM32duino库需要的全局宏定义，以指定项目使用的MCU系列及具体芯片型号。对于STM32F1xx的STM32F103VX，需要定义两个宏分别是：
* STM32F1xx                ;使用STM32F1xx系列MCU
* STM32F103xE              ;使用STM32F103的E系列芯片
两个宏之间需要使用空格或英文的逗号（“,”号）分隔。
#### 3.1.1.2 设置头文件路径
在Include Paths中输入下面的头文件路径。
```
..\hardware\stm32\1.7.0\cores
..\hardware\stm32\1.7.0\system\Drivers\CMSIS\Device\ST\STM32F1xx\Source\Templates
..\hardware\stm32\1.7.0\system\Drivers\CMSIS\Device\ST\STM32F1xx\Source\Templates\gcc
..\hardware\stm32\1.7.0\cores\arduino
..\hardware\stm32\1.7.0\cores\arduino\avr
..\hardware\stm32\1.7.0\cores\arduino\stm32
..\hardware\stm32\1.7.0\system\Drivers\STM32F1xx_HAL_Driver\Src
..\hardware\stm32\1.7.0\system\Drivers\STM32F1xx_HAL_Driver\Inc
..\hardware\stm32\1.7.0\system\STM32F1xx
..\hardware\stm32\1.7.0\system\Drivers\CMSIS\Device\ST\STM32F1xx\Include
..\hardware\stm32\1.7.0\variants\PILL_F103XX
..\hardware\stm32\1.7.0\cores\arduino\stm32\HAL
..\hardware\stm32\1.7.0\cores\arduino\stm32\LL
..\hardware\stm32\1.7.0\cores\arduino\stm32\usb
```
当使用其他的STM32 MCU时头文件的路径就要根据选择的MCU种类修改。
例如，当使用STM32F4xx时，头文件路径中包含的STM32F1xx就要改为STM32F4xx，当换成其他CPU的变种时路径..\hardware\stm32\1.7.0\variants\PILL_F103XX中的PILL_F103XX换成其他变种文件所在的文件夹路径。
#### 3.1.1.3 编译杂项设置
在Misc Control中输入-mcpu=cortex-m3 -mthumb  -fdata-sections -ffunction-sections  -lstdc++
在Enable APCS（ARM Procedure Call Standard)前打勾
在Generate Stack Check Code前打勾
在Support Calls between ARM and THUMB Instruction Set前打勾
Optimization中选择No Optimization
Warnning中选择Level 1
### 3.1.2 Assembler选项卡
在Assmbler选项卡中
在Language/Code Generation的Enable ARM/Thumb Interworking前打勾
在Misc Controls中输入-mcpu=cortex-m3 -mthumb
### 3.1.3 Linker选项卡
去掉Enable Garbage Collections，Do not use Standard System Startup Files，Do not use use Standard System Libraries前的勾（如果有）
在Use Math Libraries前打勾。
在Linker Script File中设置链接脚本，链接脚本在variants文件夹下的变种文件夹中，后文详述其需要修改的内容。
在Misc Controls中输入-Wl,--gc-sections -Wl,--entry=Reset_Handler  -lstdc++
## 4. STM32 MCU的变种文件
意法半导体公司的基于ARM的MCU有很多的产品系列，从Cortex-M0到Cortex-M7等，1.7.0版本的stm32duino支持从STM32F0xx到STM32WBxx的共13个产品系列。所以有很多的MCU的变种系列。变种文件在variants文件夹下。1.7.0版的STM32duino共支持62个STM32 MCU的变种。

MCU的变种文件有两个，分别是：
* PeripheralPins.c
* variant.cpp

同一个MCU变种的文件夹下还有MCU的连接脚本文件：
* ldscript.ld

链接脚本文件是编译后链接程序使用的。由于是使用Keil 编译、链接程序，所以链接脚本文件需要做一定的修改。主要是修改MCU内部Flash，RAM的地址和容量。并在Keil的链接设置中使用这个文件。

## 5. 链接脚本文件
在Keil IDE中使用GCC编译Arduino项目时，需要使用链接脚本文件，一些STM32duino中带的MCU变种的链接脚本文件需要修改后才能在Keil中使用，主要是这些连接脚本文件使用Arduino IDE配置MCU的FLASH,RAM的大小和容量，在使用Keil IDE时无法使用Arduino IDE才可以使用的FLASH，RAM参数配置,

例如variants\PILL_F103XX文件夹下的链接脚本文件在没修改前如下所示(局部）。

```
...
/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = 0x20000000 + LD_MAX_DATA_SIZE;    /* end of RAM */
/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 0x200;      /* required amount of heap  */
_Min_Stack_Size = 0x400; /* required amount of stack */

/* Specify the memory areas */
MEMORY
{
RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = LD_MAX_DATA_SIZE
FLASH (rx)      : ORIGIN = 0x8000000 + LD_FLASH_OFFSET, LENGTH = LD_MAX_SIZE - LD_FLASH_OFFSET
}



```
在这个文件中截取需要修改的部分
```
以上省略
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = 0x20000000 + LD_MAX_DATA_SIZE;    /* end of RAM */
/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 0x200;      /* required amount of heap  */
_Min_Stack_Size = 0x400; /* required amount of stack */

/* Specify the memory areas */
MEMORY
{
RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = LD_MAX_DATA_SIZE
FLASH (rx)      : ORIGIN = 0x8000000 + LD_FLASH_OFFSET, LENGTH = LD_MAX_SIZE - LD_FLASH_OFFSET
}
以下省略
```
可以看到文件中有3个参数

* LD_MAX_DATA_SIZE  //RAM的最高地址（容量相关）
* LD_FLASH_OFFSET   //内部FLASH的偏移地址
* LD_MAX_SIZ        //内部FLASH的容量

在使用Arduino IDE编译时，有一个文件控制这些参数，在Keil中无法使用这些参数，所以要将这些参数直接用数字表示。

```
以上省略
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = 0x20010000 ;    /* end of RAM这里做了修改 */
/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 0x200;      /* required amount of heap  */
_Min_Stack_Size = 0x400; /* required amount of stack */

/* Specify the memory areas */
MEMORY
{
RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 64K  //这里做了修改
FLASH (rx)     : ORIGIN = 0x8000000 , LENGTH = 512K //这里做了修改
}
以下省略
```

## 6.制作核心库文件core.lib
在项目的Core文件组中有一个core.lib文件，这个文件是用Arduino IDE编译后获得的。

在Arduino IDE中打开任意一个*.INO文件，设置好STM32 MCU 开发板的配置，点击Arduino的“验证”按钮编译stm32duino项目，编译成功后Arduino IDE会在C:\WINDOWS\TEMP\arduino_build_xxxxxx\core文件夹下生成一个core.a文件（xxxxxx是Arduino IDE编译器编译时随机产生的数字，可以在IDE的输出窗口中查看），将这个文件拷贝到Keil的项目文件的文件夹下。（可以与应用程序在同一个文件夹下），并将其文件名改为core.lib。这样就有核心库文件了。

## 7. 使用数学库

需要进行数学计算，DSP处理时要将数学库添加到项目中，数学库在\tools\CMSIS\5.5.1\CMSIS\DSP\Lib\GCC文件夹中，由于STM32F1xx系列MCU是ARM的Cortex-M3内核，所以这个文件夹下的libarm_cortexM3l_math.a数学库文件，由于是在Keil中使用，所以要将这个库文件的扩展名改为libarm_cortexM3l_math.lib。

## 8. 编译调试
按正常的Keil IDE的编译、调试流程即可。由于已经将很多的代码编译成了库core.lib，库文件core.lib不需要重新编译，所以项目的编译速度很快。缺点是不支持核心库部分的源代码调试。因为使用Keil集成开发环境，有这样的缺点也可以接受，这样就可以使用专业的开发工具开发、调试Arduino的应用程序了。

