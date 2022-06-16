// #include "timer.h"
// #include <MsTimer2.h>
// int DSPIN = A4;
// m_timer tp1(480);
// int colcnt = 2;
// void Timers()
// {
//     tp1.timer_function();
// }
// boolean DS18B20_Init()
// {
//     if (colcnt)
//     {
//         colcnt --;
//         tp1.restart();
//         pinMode(DSPIN, OUTPUT);
//         digitalWrite(DSPIN, HIGH);
//         delayMicroseconds(5);
//         digitalWrite(DSPIN, LOW);
//         Serial.println("Before:");
//         Serial.println(tp1.current_value);
//         // Serial.println(tp1.count_over());
//     }
//     // delayMicroseconds(750);//480-960
//     if (tp1.count_over())
//     {
//         Serial.println("After:");
//         Serial.println(tp1.current_value);
//         tp1.restart();
//         colcnt = 2;
//         digitalWrite(DSPIN, HIGH);
//         pinMode(DSPIN, INPUT);
//         int t = 0;
//         Serial.println("Read is :"); 
//         Serial.println(digitalRead(DSPIN)); 
//         while (digitalRead(DSPIN))
//         {
//             Serial.println(digitalRead(DSPIN)); 
//             t++;
//             if (t > 70)
//                 return false;
//             delayMicroseconds(1);
//         }
//         t = 480 - t;
//         pinMode(DSPIN, OUTPUT);
//         delayMicroseconds(t);
//         digitalWrite(DSPIN, HIGH);
//         return true;
//     }
// }

// void DS18B20_Write(byte data)
// {
//     pinMode(DSPIN, OUTPUT);
//     for (int i = 0; i < 8; i++)
//     {
//         digitalWrite(DSPIN, LOW);
//         delayMicroseconds(10);
//         if (data & 1)
//             digitalWrite(DSPIN, HIGH);
//         else
//             digitalWrite(DSPIN, LOW);
//         data >>= 1;
//         delayMicroseconds(50);
//         digitalWrite(DSPIN, HIGH);
//     }
// }

// byte DS18B20_Read()
// {
//     pinMode(DSPIN, OUTPUT);
//     digitalWrite(DSPIN, HIGH);
//     delayMicroseconds(2);
//     byte data = 0;
//     for (int i = 0; i < 8; i++)
//     {
//         digitalWrite(DSPIN, LOW);
//         delayMicroseconds(1);
//         digitalWrite(DSPIN, HIGH);
//         pinMode(DSPIN, INPUT);
//         delayMicroseconds(5);
//         data >>= 1;
//         if (digitalRead(DSPIN))
//             data |= 0x80;
//         delayMicroseconds(55);
//         pinMode(DSPIN, OUTPUT);
//         digitalWrite(DSPIN, HIGH);
//     }
//     return data;
// }

// int TempRead()
// {
//     if (!DS18B20_Init())
//         return 0;
//     DS18B20_Write(0xCC); //发跳过ROM命令
//     DS18B20_Write(0x44); //发读开始转换命令
//     if (!DS18B20_Init())
//         return 0;
//     DS18B20_Write(0xCC);         //发跳过ROM命令
//     DS18B20_Write(0xBE);         //读寄存器，共九字节，前两字节为转换值
//     int temp = DS18B20_Read();   //低字节
//     temp |= DS18B20_Read() << 8; //高字节
//     return temp;
// }
// void setup()
// {
//     // put your setup code here, to run once:
//     Serial.begin(9600);
//     MsTimer2::set(1, Timers); // 中断设置函数，每 1ms 进入一次中断
//     MsTimer2::start();        //开始计时
// }
// void loop()
// {
//     // put your main code here, to run repeatedly:
//     float temp = TempRead();
//     // Serial.println(temp);
//     temp *= 0.0625; //转换精度为0.0625/LSB
//     Serial.println(temp);
//     delay(1000);
// }
