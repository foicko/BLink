// void Delay1ms(unsigned int y)
// {
//     unsigned int x;
//     for (; y > 0; y--)
//     {
//         for (x = 110; x > 0; x--)
//             ;
//     }
// }

// unsigned char Ds18b20Init()
// {
//     unsigned char i;
//     pinMode(DSPORT, OUTPUT);
//     digitalWrite(DSPORT, LOW);
//     i = 70;
//     while (i--)
//         ;
//     digitalWrite(DSPORT, HIGH);
//     i = 0;
//     pinMode(DSPORT, INPUT);
//     while (digitalRead(DSPORT))
//     {
//         Delay1ms(1);
//         i++;
//         if (i > 5)
//         {
//             return 0;
//         }
//     }
//     return 1;
// }

// unsigned char Ds18b20ReadByte()
// {
//     unsigned char byte, bi;
//     unsigned int i, j;
//     pinMode(DSPORT, OUTPUT);
//     for (j = 8; j > 0; j--)
//     {
//         digitalWrite(DSPORT, LOW);
//         i++;
//         digitalWrite(DSPORT, HIGH);
//         i++;
//         i++;
//         bi = DSPORT;
//         /*byte，7bi，0。*/
//         byte = (byte >> 1) | (bi << 7);
//         i = 4;
//         while (i--)
//             ;
//     }
//     return byte;
// }

// void Ds18b20WriteByte(unsigned char dat)
// {
//     unsigned int i, j;
//     pinMode(DSPORT, OUTPUT);
//     for (j = 0; j < 8; j++)
//     {
//         digitalWrite(DSPORT, LOW);
//         i++;
//         digitalWrite(DSPORT, dat & 0x01);

//         i = 6;
//         while (i--)
//             ;

//         digitalWrite(DSPORT, HIGH);

//         dat >>= 1;
//     }
// }

// void Ds18b20ChangTemp()
// {
//     Ds18b20Init();
//     Delay1ms(1);
//     Ds18b20WriteByte(0xcc);
//     Ds18b20WriteByte(0x44);
// }

// void Ds18b20ReadTempCom()
// {

//     Ds18b20Init();
//     Delay1ms(1);
//     Ds18b20WriteByte(0xcc);
//     Ds18b20WriteByte(0xbe);
// }

// int Ds18b20ReadTemp()
// {
//     int temp = 0;
//     unsigned char tmh, tml;
//     Ds18b20ChangTemp();
//     Ds18b20ReadTempCom();
//     tml = Ds18b20ReadByte();
//     tmh = Ds18b20ReadByte();
//     temp = tmh;
//     temp <<= 8;
//     temp |= tml;
//     return temp;
// }