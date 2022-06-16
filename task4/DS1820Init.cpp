// #define DSPORT A4
// void Delay1ms(uint y) //延时1ms
// {
//     uint x;
//     for (; y > 0; y--)
//     {
//         for (x = 110; x > 0; x--)
//             ;
//     }
// }

// unsigned char Ds18b20Init() //初始化时序对应的代码
// {
//     unsigned char i;
//     pinMode(DSPORT, OUTPUT);
//     digitalWrite(DSPORT, LOW); //将总线拉低480us~960us
//     i = 70;
//     while (i--)
//         ;                       //延时642us
//     digitalWrite(DSPORT, HIGH); //然后拉高总线，如果DS18B20做出反应会将在15us~60us后总线拉低
//     i = 0;
//     pinMode(DSPORT, INPUT);
//     while (digitalRead(DSPORT)) //等待DS18B20拉低总线
//     {
//         Delay1ms(1);
//         i++;
//         if (i > 5) //等待>5MS
//         {
//             return 0; //初始化失败
//         }
//     }
//     return 1; //初始化成功
// }

// unsigned char Ds18b20ReadByte() //读时序对应的代码
// {
//     unsigned char byte, bi;
//     uint i, j;
//     pinMode(DSPORT, OUTPUT);
//     for (j = 8; j > 0; j--)
//     {
//         digitalWrite(DSPORT, LOW); //先将总线拉低1us
//         i++;
//         digitalWrite(DSPORT, HIGH); //然后释放总线
//         i++;
//         i++;         //延时6us等待数据稳定
//         bi = DSPORT; //读取数据，从最低位开始读取
//         /*将byte左移一位，然后与上右移7位后的bi，注意移动之后移掉那位补0。*/
//         byte = (byte >> 1) | (bi << 7);
//         i = 4; //读取完之后等待48us再接着读取下一个数
//         while (i--)
//             ;
//     }
//     return byte;
// }

// void Ds18b20WriteByte(unsigned char dat) //写时序对应的代码
// {
//     uint i, j;
//     pinMode(DSPORT, OUTPUT);
//     for (j = 0; j < 8; j++)
//     {
//         digitalWrite(DSPORT, LOW); //每写入一位数据之前先把总线拉低1us
//         i++;
//         DSPORT = dat & 0x01; //然后写入一个数据，从最低位开始
//         i = 6;
//         while (i--)
//             ;
//         　　　　　　　 //延时68us，持续时间最少60us
//             digitalWrite(DSPORT, HIGH);
//         　　　　　 //然后释放总线，至少1us给总线恢复时间才能接着写入第二个数值
//             dat >>= 1;
//     }
// }
// /*******************************************************************************
//  * 函 数 名         : Ds18b20ChangTemp
//  * 函数功能         : 让18b20开始转换温度
//  * 输    入        : 无
//  * 输    出        : 无
//  *******************************************************************************/

// void Ds18b20ChangTemp()
// {
//     Ds18b20Init();
//     Delay1ms(1);
//     Ds18b20WriteByte(0xcc); //跳过ROM操作命令
//     Ds18b20WriteByte(0x44); //温度转换命令
//     // Delay1ms(100);    //等待转换成功，而如果你是一直刷着的话，就不用这个延时了
// }

// /*******************************************************************************
//  * 函 数 名         : Ds18b20ReadTempCom
//  * 函数功能          : 发送读取温度命令
//  * 输    入         : 无
//  * 输    出         : 无
//  *******************************************************************************/

// void Ds18b20ReadTempCom()
// {

//     Ds18b20Init();
//     Delay1ms(1);
//     Ds18b20WriteByte(0xcc); //跳过ROM操作命令
//     Ds18b20WriteByte(0xbe); //发送读取温度命令
// }

// /*******************************************************************************
//  * 函 数 名         : Ds18b20ReadTemp
//  * 函数功能           : 读取温度
//  * 输    入         : 无
//  * 输    出         : 无
//  *******************************************************************************/

// int Ds18b20ReadTemp()
// {
//     int temp = 0;
//     unsigned char tmh, tml;
//     Ds18b20ChangTemp();      //先写入转换命令
//     Ds18b20ReadTempCom();    //然后等待转换完后发送读取温度命令
//     tml = Ds18b20ReadByte(); //读取温度值共16位，先读低字节
//     tmh = Ds18b20ReadByte(); //再读高字节
//     temp = tmh;
//     temp <<= 8;
//     temp |= tml;
//     return temp;
// }

// // //*****************************
// // float DS18b20()
// // {
// //     int i;
// //     unsigned char L, M;
// //     DS18b20_Init();      //初始化
// //     DS18b20_Write(0xcc); //跳过ROM，因为只用到了一个DS18B20,不需要操作
// //     DS18b20_Write(0x44); // 0x44，温度转换指令
// //     DS18b20_Init();      //根据协议，再次初始化
// //     DS18b20_Write(0xcc);
// //     DS18b20_Write(0xbe); //读取暂存器
// //     L = DS18b20_Red();   //将暂存器的值赋予给L
// //     M = DS18b20_Red();   //将暂存器的值赋予给L
// //     i = M;
// //     i <<= 8;                   //意思是将M放在高8位
// //     i |= L;                    //这样就是16位，高八位是M，低八位是L
// //     i = i * 0.0625 * 10 + 0.5; //这里*10+0.5的目的是为了取整，所以这里的i其实是真实温度的十倍
// //     return i;
// // }
// // unsigned char DS18b20_Red()
// // {
// //     unsigned char i, dat, j;
// //     for (i = 0; i < 8; i++)
// //     {
// //         DQ = 0;
// //         _nop_();
// //         DQ = 1;
// //         _nop_();
// //         j = DQ;
// //         Delay_us(13); // 84.5us
// //         dat = (j << 7) | (dat >> 1);
// //         DQ = 1;
// //         _nop_;
// //     }
// //     return dat;
// // }
// // void DS18b20_Write(unsigned char dat)
// // {
// //     unsigned char i;
// //     for (i = 0; i < 8; i++)
// //     {
// //         DQ = 0;
// //         _nop_();
// //         DQ = dat & 0x01;
// //         Delay_us(13); // 84.5us
// //         DQ = 1;
// //         _nop_();
// //         dat >>= 1;
// //     }
// // }
// // unsigned char DS18b20_Init()
// // {
// //     bit i;
// //     DQ = 1;
// //     Delay_us(1); // 6.5us
// //     DQ = 0;
// //     Delay500us();
// //     DQ = 1;
// //     Delay_us(4); // 26us
// //     i = DQ;
// //     Delay_us(20); // 130us
// //     DQ = 1;
// //     Delay_us(1); // 6.5us
// //     return i;
// // }
// void Delay500us() //@11.0592MHz
// {
//     unsigned char i;
//     _nop_();
//     i = 497;
//     while (--i)
//         ;
// }
// void Delay_us(uchar i) //@11.0592MHz
// {
//     while (i--)
//         ; //一次6.5us
// }