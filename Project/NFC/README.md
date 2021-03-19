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
* `MCU：STM32F103RGT6`
* `NFC`芯片：`ST95HF`
* 显示屏：`1.54inch`墨水屏

### 2021.01.08 
* 上传`PCB`工程 `NFCV1.1`(修正了投板`PCB`上的错误)

### 2021.01.09
* 板子焊接大部分焊接完成，初步测试通过。

### 2021.01.11
* 开始移植程序，使用`STM32CubeMX`生成的`demo`,能正常读到`ST95HF`的设备`Device ID`

### 2021.01.25
* 上传`ST95HF`修改过的`demo  EVAL-ST95HF_FW_V3.7.3`
* 读`UID`

### 2021.02.01

* `UART log`:

    ```c
    System Init [OK]
    POR 
    >>>55 00 
    <<<55 00 
    
    read DevID 
    >>>01 00 
    <<<00 0F 4E 46 43 20 46 53 32 4A 41 53 54 34 00 2A CE 
    
    select protocol 
    >>>02 02 02 00 
    <<<00 00 
    
    adjust gain
    >>>09 04 68 01 01 DF 
    <<<00 00 
    
    ============ISO14443A anticollison start=============
    Anticol REQA
    >>>04 02 26 07 
    <<<80 05 04 00 28 00 00 
    
    Anticol CL1
    >>>04 03 93 20 08 
    <<<80 08 F4 74 8F AB A4 28 00 00 
    
    Anticol Select1
    >>>04 08 93 70 F4 74 8F AB A4 28 
    <<<80 06 08 B6 DD 08 00 00 
    
    ============ISO14443A anticollison stop==============
    
    read
    >>>04 03 30 01 28 
    <<<90 04 04 24 00 00
    ```

* 能检测到卡的类型，但是不能读取`NDEF`

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

* `stm32 + PN532`：https://www.cirmall.com/circuit/5477  

* `stm32 + PN532`卡模拟程序：https://download.csdn.net/download/wsqrf/10351894 

### 2021.03.10

* `demo/en.stsw-st95hf001`大致流程：

  * `Menu_Init()`将菜单结构初始化，并完成每个小功能与对应回调函数的联系，菜单结构为：

    ```c
    Mian Menu
    |__	Tag Reader
    |	|__	TAG Hunting
    |	|__	TAG Detect Wake-UP
    |	|__	TAG Reading
    |	|__	PC link mode
    |
    |__	Tag Writer
    |	|__	Write TEXT        30
    |	|__	Write GEO         67
    |	|__	Write URI         71
    |	|__	Write SMS        156
    |	|__	Write EMAIL     1050
    |	|__	Write VCARD     7711
    |
    |__	Tag Emulator
    |	|__	TT2  (ISO14443A)
    |	|	|__	Emulate URL
    |	|	|__	Emulate SMS
    |	|	|__	Previous content
    |	|    
    |	|__	TT4A (ISO14443A)
    |		|__	Emulate URL
    |		|__	Emulate SMS
    |		|__	Emulate EMAIL
    |		|__	Emulate VCARD
    |		|__	Previous content
    |		
    |__	Card Emulator
    |	|__	DWL (ISO14443A)
    |	
    |__	Perr to Peer
    |	|__	Pong Config Client
    |	|__	Pong Config Server
    |	|__	Prop P2P Client
    |	|__	Prop P2P Server
    |
    ...
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
    struct sMenu TagReaderMenu = {(u8*)"  Tag Reader Mode  ", TagReaderMenuItems, countof(TagReaderMenuItems)};
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
  

### 2021.03.11

  * `ISO/IEC-14443-TypeA/TypeB`防冲突原理和算法：https://blog.csdn.net/tianyuan12345678/article/details/80724114 （此文章应该是摘自`*/pdf/ISO14443-3.pdf`，可将两者结合阅读）
  
  * 截取`Chap 2021.02.01`的`uart log`部分：

      ```c
      Anticol CL1
      >>>04 03 93 20 08 
      <<<80 08 F4 74 8F AB A4 28 00 00 
      ```

      经分析`ISO14443-3.pdf`和上文链接，发现在`Respone`数据`80 08 F4 74 8F AB A4 28 00 00 `中，`Byte[0:1]`是`ST95HF`自身返回的数据，`byte[2:5]`是卡片返回的内容，是其四个字节的`UID`,`byte[6]`是`BCC (see ISO14443-3.pdf Chap 4)`，是在四字节`UID`之上做异或的结果，即是`0xF4^ 0x74^ 0x8F^ 0xAB = 0xA4`，`byte[7:9]`也是`ST95HF`自身返回的数据

### 2021.03.12

* 添加`STM32CubeExpansion_NFC3_V1.3.0 - M1_.rar` https://www.stmcu.org.cn/module/forum/thread-620546-1-1.html ， https://www.stmcu.org.cn/module/forum/thread-621336-1-1.html

* `NDEF Format`的数据格式 https://learn.adafruit.com/adafruit-pn532-rfid-nfc/ndef# 

    * `NDEF(NFC Data Exchange Format)`

    * `Messages`

        * `NDEF Messages `是`NDEF Records`的基本“传输”机制，每个`Message`包含一个或多个`NDEF Records`

    * `Records`

        * `NDEF Records`包含了特定的有效负载，其结构如下所示

            ```c
            Bit 7     6       5       4       3       2       1       0
            ------  ------  ------  ------  ------  ------  ------  ------ 
            [ MB ]  [ ME ]  [ CF ]  [ SR ]  [ IL ]  [        TNF         ]  
             
            [                         TYPE LENGTH                        ]
             
            [                       PAYLOAD LENGTH                       ]
             
            [                          ID LENGTH                         ]
             
            [                         RECORD TYPE                        ]
             
            [                              ID                            ]
             
            [                           PAYLOAD                          ]
            ```

        * `Record Header`定义如下所示

            ```c
            TNF Value	Record Type
            ---------	-----------------------------------------
            0x00		Empty Record
                		Indicates no type, id, or payload is associated with this NDEF Record.This record type is useful on newly formatted cards since every NDEF tag must have at least one NDEF Record.
                
            0x01         Well-Known Record
            			Indicates the type field uses the RTD type name format.  This type name is used to stored any record defined by a Record Type Definition (RTD), such as storing RTD Text, RTD URIs, etc., and is one of the mostly frequently used and useful record types.
                
            0x02         MIME Media Record
            			Indicates the payload is an intermediate or final chunk of a chunked NDEF Record
                
            0x03         Absolute URI Record
            			Indicates the type field contains a value that follows the absolute-URI BNF
            construct defined by RFC 3986
                
            0x04         External Record
            			Indicates the type field contains a value that follows the RTD external
            name specification
                
            0x05         Unknown Record
            			Indicates the payload type is unknown
                
            0x06         Unchanged Record
            			Indicates the payload is an intermediate or final chunk of a chunked NDEF Record
                
            IL Value	ID LENGTH Field
            --------- 	---------------------------------------
            			IL标志表示ID长度字段是否可以使用。如果将其设置为0，则在记录中省略ID长度字段。
                            
            SR 			Short Record Bit
            --------	--------------------------------------
                      	如果有效载荷长度字段为1字节(8比特/0-255)或更少，SR标志将被设置为1。这允许更紧凑的记录。
                            
            CF			Chunk Flag
            --------	--------------------------------------
            			CF标志表明这是第一个记录块还是中间记录块。
            		
            ME			Message End
            --------	--------------------------------------
                       	ME标志指示这是否是消息中的最后一条记录
                            
            MB			Message Begin
            --------	--------------------------------------
                        MB标志指示这是否是消息中的第一条记录
            ```

        * `Type Length`

        * `Payload Length`

        * `ID Length`

        * `Record Type`

        * `Record ID`

        * `Payload`


### 2021.03.19

* `Mifare Classic S50`卡加密算法：https://blog.csdn.net/trouble_makers/article/details/102879491  