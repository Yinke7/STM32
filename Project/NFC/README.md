# Introduction
此项目参考了b站稚晖君的Link-Card项目，但是在其基础上做了修改，采用STM32和ST95HF芯片，预期将实现NFC Reader/Tag/Emulation等功能，一张卡可模拟多张卡，包括门禁、公交等场景。

# Version iteration
## V1.0
* 主控芯片：32L051K8U6
* NFC芯片：ST95HF
* 显示屏：1.54inch墨水屏

### 2020.12.15
* 上传PCB工程 NFCV1.0.zip

### 2020.12.25
* 板子焊接完后，初步测试通过。

### 2020.12.28
* 发现STM32在使用内部2M时钟时，点灯可正常运行。使用内部16M时钟时，在调试模式下通过打断点方式，观察到可正常运行，但是退出调试模式，重新上电后就会卡住，LED也没反应。
* 可能的原因：
	1. 芯片问题，芯片在焊接时，被破坏了
	2. 电源问题，电流可能不够
	3. 软件问题，硬件都正常的情况下，STM32在16M时，进入了Sleep状态，需要被唤醒

### 2020.12.30
* 此版本暂时放一边，准备使用ST95官方的硬件方案，用STM32F103RGT6作为MCU，另外预留出STM32L051K8U6的位置。

## V1.1
* MCU：STM32F103RGT6
* NFC芯片：ST95HF
* 显示屏：1.54inch墨水屏

### 2021.01.08 
* 上传PCB工程 NFCV1.1(修正了投板PCB上的错误)

### 2021.01.09
* 板子焊接大部分焊接完成，初步测试通过。

### 2021.01.11
* 开始移植程序，使用STM32CubeMX生成的demo,能正常读到ST95HF的设备ID

### 2021.01.25
* 上传ST95HF修改过的demo ` EVAL-ST95HF_FW_V3.7.3`
* 读UID

### 2021.02.01
* 能检测到卡的类型，但是不能读取NDEF

* 发送命令：

  ```c
  >>>04 03 30 01 28
  ```

* 返回数据：

  ```c
  <<<90 04 04 24 00 00
  ```


* 根据` st95hf.pdf`中的` ErrorCodes`可以看出，返回数据中的错误码是` 0x90`，其描述是` Residual bits in last byte. Useful for ACK/NAK reception of ISO/IEC 14443 Type A`，大概意思是最后一格字节有剩余位，被用在` ISO14443A`的` ACK/NAK`中，但是在` ACK/NAK`相关部分介绍中，返回数据的` 90 04`后要么是` 0a`，要么是` 02`，但是我接受的数据` 90 04 04 24 00 00`中后面是` 04`，所以这就存在问题。

  ![ErrorCode](https://github.com/Yinke7/STM32/blob/debug/Project/NFC/image/errorcode.jpg)

  ![ACK/NAK](https://github.com/Yinke7/STM32/blob/debug/Project/NFC/image/ACK_NAK.jpg)

### 2021.02.03
* 测试ST官方的NFC扩展版` X-NUCLEO-NFC06A1`

### 2021.03.03

* 增加文件夹 ` demo`，包含` st25r3916`相关测试程序
  * ` ST25R3916_X-NUCLEO-NFC06A1.zip`：ST官方程序
  * ` STM32CubeExpansion_NFC5_V1.2.0.zip`：读取` M1`卡的程序，https://www.stmcu.org.cn/module/forum/thread-621336-1-1.html 



