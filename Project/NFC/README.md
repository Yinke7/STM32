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

### 2021.03.04

* 上传 ` TESTRCT6`
  * 该程序用STM32CubeIDE打开
  * 连接好线路，注意` IRQ`是高电平有效，所以要配置成上升沿触发中断。（前几天就是因为配置成下降沿触发，始终无法读到` UID`，今天才找到原因）
  * 可读取` M1`卡的` UID`

### 2021.03.07

* 上传`ST25R3916`的`demo TestZET6`，能读到`NTAG`的`NEDF`数据

  ![ntagread](https://github.com/Yinke7/STM32/blob/debug/Project/NFC/image/ntag_read.jpg)

* `stm32 + PN532`的程序：https://www.cirmall.com/circuit/5477 

### 2021.03.10

* `demo/en.stsw-st95hf001`大致流程：

  * `Menu_Init()`将菜单结构初始化，并完成每个小功能与对应回调函数的联系，菜单结构为：

    ```c++
    Mian Menu
        	Tag Reader
        			TAG Hunting
        			TAG Detect Wake-UP
        			TAG Reading
        			PC link mode
        	Tag Writer
        			Write TEXT        30
        			Write GEO         67
        			Write URI         71
        			Write SMS        156
        			Write EMAIL     1050
        			Write VCARD     7711
        	Tag Emulator
        			TT2  (ISO14443A)
        					Emulate URL
        					Emulate SMS
        					Previous content
        			TT4A (ISO14443A)
        					Emulate URL
        					Emulate SMS
        					Emulate EMAIL
        					Emulate VCARD
        					Previous content
        	Card Emulator
        			DWL (ISO14443A)
        	Perr to Peer
        			Pong Config Client
        			Pong Config Server
        			Prop P2P Client
        			Prop P2P Server
    ```

    ```c
    /*Main Menu*/
    struct sMenuItem MainMenuItems[] = {
        {(u8*)"     Tag Reader     ", IdleFunc, IdleFunc, &TagReaderMenu},
        {(u8*)"     Tag Writer     ", IdleFunc, IdleFunc, &TagWriterMenu},
        {(u8*)"    Tag Emulator    ", IdleFunc, IdleFunc, &TagEmulMenu},
        {(u8*)"   Card Emulator    ", IdleFunc, IdleFunc, &CardEmulMenu },
        {(u8*)"    Peer to Peer    ", IdleFunc, IdleFunc, &P2PMenu}
    };
    ```

    ```c
    /*TAG Reader*/
    struct sMenuItem TagReaderMenuItems[] = {
        {(u8*)"    TAG Hunting     ", TagHunting, IdleFunc},
        {(u8*)" TAG Detect Wake-UP ", TagDetectWakeUp, IdleFunc},
        {(u8*)"    TAG Reading     ", TagReading, IdleFunc},
        {(u8*)"    PC link mode    ", USBRemoteControl, IdleFunc},
        {(u8*)"<-     Return       ", ReturnFunc, IdleFunc}};
    struct sMenu TagReaderMenu = {(u8*)"   Tag Reader Mode  ", TagReaderMenuItems, countof(TagReaderMenuItems)};
    ```
    
    ```c
    /*TAG Writer*/
    struct sMenuItem TagWriterMenuItems[] = {
        {(u8*)"Write TEXT        30", TagWriteText, IdleFunc},
        {(u8*)"Write GEO         67", TagWriteGEO, IdleFunc},
        {(u8*)"Write URI         71", TagWriteURI, IdleFunc},
        {(u8*)"Write SMS        156", TagWriteSMS, IdleFunc},
        {(u8*)"Write EMAIL     1050",TagWriteEMAIL, IdleFunc},
        {(u8*)"Write VCARD     7711", TagWriteVCARD, IdleFunc},
        {(u8*)"<-     Return       ", ReturnFunc, IdleFunc}};
    struct sMenu TagWriterMenu = {(u8*)"   Tag Writer Mode  ", TagWriterMenuItems, countof(TagWriterMenuItems)};
    ```
    
    ```c
      /*Tag Emulator*/	
      struct sMenuItem TagEmulMenuItems[] = {
          {(u8*)"  TT2  (ISO14443A)  ", IdleFunc, IdleFunc,&TagEmulT2Menu},
          {(u8*)"  TT4A (ISO14443A)  ", IdleFunc, IdleFunc,&TagEmulT4AMenu},
          {(u8*)"<-     Return       ", ReturnFunc, IdleFunc}};
      
      /*TT2 Emulator*/
      struct sMenuItem TagEmulT2MenuItems[] = {
          {(u8*)"    Emulate GEO     ", TagEmulT2GEO, IdleFunc},
          {(u8*)"    Emulate URL     ", TagEmulT2URI, IdleFunc},
          {(u8*)"    Emulate SMS     ", TagEmulT2SMS, IdleFunc},
          {(u8*)"  Previous content  ", TagEmulT2, IdleFunc},
          {(u8*)"<-     Return       ", ReturnFunc, IdleFunc}};
      struct sMenu TagEmulT2Menu = {(u8*)"   TT2 Emulator  ", TagEmulT2MenuItems, countof(TagEmulT2MenuItems)};
      
      /*TT4A Emulator*/
      struct sMenuItem TagEmulT4AMenuItems[] = {
          {(u8*)"    Emulate GEO     ", TagEmulT4AGEO, IdleFunc},
          {(u8*)"    Emulate URL     ", TagEmulT4AURI, IdleFunc},
          {(u8*)"    Emulate SMS     ", TagEmulT4ASMS, IdleFunc},
          {(u8*)"   Emulate EMAIL    ", TagEmulT4AEMAIL, IdleFunc},
          {(u8*)"   Emulate VCARD    ", TagEmulT4AVCARD, IdleFunc},
          {(u8*)"  Previous content  ", TagEmulT4A, IdleFunc},
          {(u8*)"<-     Return       ", ReturnFunc, IdleFunc}};
      struct sMenu TagEmulT4AMenu = {(u8*)"  TT4A Emulator ", TagEmulT4AMenuItems, countof(TagEmulT4AMenuItems)};
    ```
    
    ```c
     /*Card Emulator*/
      struct sMenuItem CardEmulMenuItems[] = {
          {(u8*)"  DWL (ISO14443A)   ", CardEmul14443A, IdleFunc},
          {(u8*)"<-     Return       ", ReturnFunc, IdleFunc}};
      struct sMenu CardEmulMenu = {(u8*)" Card Emulator Mode ", CardEmulMenuItems, countof(CardEmulMenuItems)};
    ```
    
    ```c
      /*Peer to Peer*/
      struct sMenuItem P2PMenuItems[] = { /*{(u8*)"   Prop P2P Auto    ", ProprietaryP2PAuto, IdleFunc},*/
          {(u8*)" Pong Config Client ", PongConfigClient, IdleFunc}, 
          {(u8*)" Pong Config Server ", PongConfigServer, IdleFunc},
          {(u8*)"  Prop P2P Client   ", ProprietaryP2PConfigClient, IdleFunc},
          {(u8*)"  Prop P2P Server   ", ProprietaryP2PConfigServer, IdleFunc},
          {(u8*)"<-     Return       ", ReturnFunc, IdleFunc}};
      struct sMenu P2PMenu = {(u8*)"    Peer to Peer    ", P2PMenuItems, countof(P2PMenuItems)};
    ```
  * 选择对应菜单功能，则执行对应回调函数
  

