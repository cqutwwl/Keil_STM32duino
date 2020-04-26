# Keil_STM32duino
Compile and debug stm32dino using keil integrated development environment

## 1. Get stm32douino source code
Using tools-->Boards Manage menu in Arduino IDE, add st official's stm32duno package.Under C: \ users \ your user name \ appdata \ local \ arduino15 \ packages, there are all the code and compilation tools of this project.
 
### 1.1 STM32duino package.
In the folder C: \ users \ your user name \ appdata \ local \ arduino15 \ packages, there are the following files.
```
STM32                      
  --hardware
  --tools
```
The hardware folder is the source code of the project, and the tools folder is the compilation tool.
```
hardware                       ;
   --stm32                      ;stm32 folder
     --1.7.0                      ;version 1.7.0
       --CI                        ;Command line tool folder, temporarily not used
       --core                    ;Stm32duino core code
       --libraries               ;Stm32duino's own library
       --system                ;STM32 HAL library of all supported devices.
       --variants               ;Various varieties of devices supported by the stm32duino
```
## 2. Create Keil project
We create a new folder stm32duino, copy all the contents of Arduino project's stm32duino packegs's hardware and tools folder to this folder, and then create a new folder MDK to store keil's project files.
```
stm32duino
  --hardware             ;copy as same from C:\Users\youe user name\AppData\Local\Arduino15\packages\STM32\hardware
  --tools                    ;copy as same from C:\Users\youe user name\AppData\Local\Arduino15\packages\STM32\tools
  --mdk                     ;Keil project folder
    --build                  ;Save *.obj and *.lst files generated at compile time in this folder
```
### 2.1Grouping of project files
```
  --User                ;User's Application
  --core                ;STM32Duino core files 
  --variants           ;devices variants
  --libraries           ;libraries
```
### 2.2 Add files to project
#### 2.2.1 User files group
Store the user's application in the user group，*.cpp
#### 2.2.2 Core files group
Add core.lib, `\hardware\stm32\1.7.0\cores\arduino\syscall. C` to the core group.

Core.lib is generated with Arduino IDE, see section 6.

#### 2.2.3 variants files group
This project takes stm32f103ve as an example. The variants group's files needed by the project are in the `\hardware\stm32\1.7.0\variants\PILL_F103XX` folder.Two files need to be added, one is peripheralpins. C and the other is variant.cpp.

## 3. Settings of Keil
### 3.1 Compiler setting
Using `project-->Manage-->Components Environment Books...`menu in the Keil.

open the `Manage Project Item` dialog box，select `Folders/Extension` tab，

checked `Use GCC`，in the `GNU-Tool-Prefix` edit box input arm-none-eabi-.

in the `GNU-Tool-Folder` edit box input `X:\stm32duino\tools\arm-none-eabi-gcc\8.2.1-1.7\`(X should be replaced by your drive letter)
### 3.1.1 CC tab
using `project-->Options for Target 'Target 1'` menu in the Keil，select `CC` tab.
#### 3.1.1.1 Macro to define
In the `CC` tab of `Options for Target 'Target 1'` ，The following macros are defined in the Define edit box.
* STM32F1xx                    ;use stm32f1xx Series MCU
* STM32F103xE                ;using STM32F103E-Series chip

Before compiling the project, you need to define the global macro definition required by stm32dino library to specify the MCU family and specific device used in the project. For stm32f103vE of stm32f1xx, you need to define two macros above.
The two macros need to be separated by a space or a comma (",") in English.
#### 3.1.1.2 Set header file path
Enter the following header file path in the `Include Paths` edit box of this tab.
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
When using other STM32 MCU, the path of header file should be modified according to the selected MCU type.
For example, when stm32f4xx is used, the stm32f1xx contained in the header file path will be changed to stm32f4xx. When the path is changed to a variety of other CPUs, the path will be changed to the folder path where the other variety files are located.
#### 3.1.1.3 Compile miscellaneous settings
Enter `-mcpu=cortex-m3 -mthumb  -fdata-sections -ffunction-sections  -lstdc++` in the `Misc Control` edit box  

checked `Enable APCS（ARM Procedure Call Standard)`

checked `Generate Stack Check Code`

checked `Support Calls between ARM and THUMB Instruction Set`

In the `Optimization` Checkbox shoud be selected `No Optimization`

In the `Warnning` Checkbox shoud be selected `Level 1`
### 3.1.2 Assembler tab
In the `Assmbler` tab.

In the `Language/Code Generation group`, checked `Enable ARM/Thumb Interworking`

Enter `-mcpu=cortex-m3 -mthumb` in `Misc Controls edit` box.
### 3.1.3 Linker tab
unchecked the `Enable Garbage Collections`，

unchecked the `Do not use Standard System Startup Files`，

unchecked the `Do not use use Standard System Libraries`.

checked the `Use Math Libraries`。

Set the link script in the `Linker Script File` edit box with `\hardware\stm32\1.7.0\variants\PILL_F103XX folder` file. 

The link script is in the variants folder under the variants folder. Later, the content to be modified will be described in detail.see section 5.

Enter `-Wl,--gc-sections -Wl,--entry=Reset_Handler  -lstdc++` in the `Misc Controls` edit box.
## 4. Variant file of STM32 MCU
The ARM based MCU of ST semiconductor company has a lot of product lines, from Cortex-M0 to cortex-m7, etc. stm32duino version 1.7.0 supports 13 product lines from stm32f0xx to stm32wbxx. So there are many varieties of MCU series. The variants file is in the variants folder. Stm32dino version 1.7.0 supports 62 variants of STM32 MCU.

There are two variants files of MCU shoud be used,  namely:
* `PeripheralPins.c`
* `variant.cpp`

There are also link script files of MCU under the folder of the same MCU variant:
* `ldscript.ld`

The linked script file is used by the linked program after compilation. Because the program is compiled and linked with Keil, the linked script file needs to be modified. It is mainly to modify the address and capacity of flash and ram in MCU. And use this file in keil's link settings.

## 5. Modify linked script file
When using gcc to compile Arduino project in keil IDE, you need to use link script files. Some link script files of MCU variants in stm32dino need to be modified before they can be used in keil. The main reason is that the link script file used in the Arduino IDE has configuration parameters that cannot be used in the Keil IDE.

For example, the link script file under the `\hardware\stm32\1.7.0\variants\PILL_F103XX` folder is as follows (partial) before modification

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
RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = LD_MAX_DATA_SIZE
FLASH (rx)      : ORIGIN = 0x8000000 + LD_FLASH_OFFSET, LENGTH = LD_MAX_SIZE - LD_FLASH_OFFSET
}

```
In this file, you can intercept the parts that need to be modified.
```
... ...
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
... ...
```
You can see that there are three parameters in the file.

* `LD_MAX_DATA_SIZE`      //Maximum address of RAM (size dependent)
* `LD_FLASH_OFFSET`         //Offset address of internal flash
* `LD_MAX_SIZ`                  //size of internal flash
When compiling with Arduino IDE, there is a file to control these parameters, which cannot be used in the Keil IDE, so these parameters should be directly represented by numbers.

```
... ....
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
... ...
```

## 6.Make core library file core.lib
There is a core.lib file in the core filegroup of the project, which is compiled with Arduino ide.

Open a `*. Ino` file in Arduino IDE, set the configuration of STM32 MCU development board, and click Arduino's "`Verify`" button to compile stm32duno project. After the compilation is successful, Arduino ide will generate a core. a file in the `C:\WINDOWS\TEMP\arduino_build_xxxxxx\core` folder(xxxxxx is a numbers randomly generated by the Arduino IDE during compilation,It can be viewed in the Arduino IDE's output window). Copy this file to the folder of Keil's project file. (it can be in the same folder as the application), and change its file name to core.lib. So you have the core library file core.lib.

## 7. Using the math library

if mathematical calculation is needed, the ARM's mathematical library should be added to the project. The mathematical library is in the folder of `\ tools \ CMSIS \ 5.5.1 \ CMSIS \ DSP \ lib \ GCC`. Because stm32f1xx Series MCU is the Cortex-M3 core of ARM, so the libarm_cortexM3l_math.a mathematical library file under this folder should be selected,and should be changed to libarm_cortexM3l_math.lib because it is used in Keil IDE。

## 8. Compile and debugging
Just follow the normal compilation and debugging process of Keil IDE. Since a lot of code has been compiled into the library core.lib, the library file core.lib does not need to be recompiled, so the compilation speed of the project is very fast. The disadvantage is that it does not support the source code debugging of the core library. Because of the use of Keil integrated development environment, such shortcomings can also be accepted, so that professional development tools can be used to develop and debug Arduino applications。