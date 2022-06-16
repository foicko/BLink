/*
改进了时钟秒的实现方式
重写了串口通信协议
因实现方式原因，将comm.cpp中的代码转移到了主源文件中

@version 0.89
@time 2021/11/26
@auther foicko zha
*/

#include <MsTimer2.h>
// #include "Song.h"
#include "timer.h"
#include "comm.h"
#include <Bounce2.h> //用来消抖的
#include "Tube595.h"
Bounce2::Button bo_BUTT1 = Bounce2::Button();
Bounce2::Button bo_BUTT2 = Bounce2::Button();
Bounce2::Button bo_BUTT3 = Bounce2::Button();
//四个小灯
#define LED1 13
#define LED2 12
#define LED3 11
#define LED4 10
int LEDs[] = {LED1, LED2, LED3, LED4};

//三个按钮
#define BUTT1 15
#define BUTT2 16
#define BUTT3 17

int flag = 0;
// 74_595
#define SDI 8 //数据
#define SFT 7 //输入时钟
#define LCH 4 //锁存时钟
//铃声
int if_ring = 0;
#define alarm 3
int cnt = 0;
bool cnt_bool = 0;

Tube595 tube(SDI, SFT, LCH);

// Song sky;
String data = "";
int seted_hour;
int seted_min;

//温度
float temp = 12.3;
float temp_warning = 37.0;
bool temp_alarm = 0;
//湿度
float wet = 36;
float wet_warning = 66;
bool wet_alarm = 0;
//湿度传感器（假）引脚
#define wetport A0
#define DSPIN A4
#define NOP                          \
    do                               \
    {                                \
        __asm__ __volatile__("nop"); \
    } while (0)

boolean DS18B20_Init()
{
    pinMode(DSPIN, OUTPUT);
    digitalWrite(DSPIN, HIGH);
    delayMicroseconds(5);
    digitalWrite(DSPIN, LOW);
    delayMicroseconds(750); // 480-960
    digitalWrite(DSPIN, HIGH);
    pinMode(DSPIN, INPUT);
    int t = 0;
    while (digitalRead(DSPIN))
    {
        t++;
        if (t > 60)
            return false;
        delayMicroseconds(1);
    }
    t = 480 - t;
    pinMode(DSPIN, OUTPUT);
    delayMicroseconds(t);
    digitalWrite(DSPIN, HIGH);
    return true;
}

void DS18B20_Write(byte data)
{
    pinMode(DSPIN, OUTPUT);
    for (int i = 0; i < 8; i++)
    {
        digitalWrite(DSPIN, LOW);
        delayMicroseconds(10);
        if (data & 1)
            digitalWrite(DSPIN, HIGH);
        else
            digitalWrite(DSPIN, LOW);
        data >>= 1;
        delayMicroseconds(50);
        digitalWrite(DSPIN, HIGH);
    }
}

byte DS18B20_Read()
{
    pinMode(DSPIN, OUTPUT);
    digitalWrite(DSPIN, HIGH);
    delayMicroseconds(2);
    byte data = 0;
    for (int i = 0; i < 8; i++)
    {
        digitalWrite(DSPIN, LOW);
        delayMicroseconds(1);
        digitalWrite(DSPIN, HIGH);
        pinMode(DSPIN, INPUT);
        delayMicroseconds(5);
        data >>= 1;
        if (digitalRead(DSPIN))
            data |= 0x80;
        delayMicroseconds(55);
        pinMode(DSPIN, OUTPUT);
        digitalWrite(DSPIN, HIGH);
    }
    return data;
}

int TempRead()
{
    if (!DS18B20_Init())
        return 0;
    DS18B20_Write(0xCC); //发跳过ROM命令
    DS18B20_Write(0x44); //发读开始转换命令
    if (!DS18B20_Init())
        return 0;
    DS18B20_Write(0xCC);         //发跳过ROM命令
    DS18B20_Write(0xBE);         //读寄存器，共九字节，前两字节为转换值
    int temp = DS18B20_Read();   //低字节
    temp |= DS18B20_Read() << 8; //高字节
    return temp;
}
bool s_rcv_package = false;
int g_work_mode = MODE_RUN;

void Refresh()
{
    tube.setDigital(0, 0);
    tube.setDigital(1, 0);
    tube.setDigital(2, 0);
    tube.setDigital(3, 0);
}
void Ring()
{
    if (if_ring)
    {
        // 按钮一消音
        if (bo_BUTT1.pressed())
        {
            if_ring = 0;
        }
    }
}
class timerMaker
{
public:
    int secs = 0;  //秒
    int mins = 9;  //分
    int hors = 12; //时
    //闹钟时间
    int alamin = 10, alahor = 22;
    timerMaker() {}
    timerMaker(int sec, int min, int hour)
    {
        secs = sec;
        mins = min;
        hors = hour;
    }
    void start();
    void stop();
    void Interrupt();
};

void timerMaker::start()
{
    if (secs > 59)
    {
        mins++;
        secs = 0;
    }
    if (mins > 59)
    {
        hors++;
        mins = 0;
    }
    if (hors > 23)
    {
        hors = 0;
        tube.setDigital(0, 0);
        tube.setDigital(1, 0);
    }
    if (alamin > 59)
    {
        alahor++;
        alamin = 0;
        tube.setDigital(2, 0);
        tube.setDigital(3, 0);
    }
    if (alahor > 23)
    {
        alahor = 0;
        tube.setDigital(0, 0);
        tube.setDigital(1, 0);
    }
    //不显示闹钟和温度时，显示时间。
    if ((flag == 1 || flag == 0) && g_work_mode != MODE_DISPLAY_FULL && g_work_mode != MODE_DISPLAY_BLANK)
    {
        Refresh();
        tube.displayInt(hors * 100 + mins);
        // tube.displayFloat(0,3.456,1);//0是设置前导，0为c，1为f。
    }
    // 同样也是假的蜂鸣器
    // tempAlarm_rinf();
    // wetAlarm_ring();

    Ring();
}
//湿度报警铃
void wetAlarm_ring()
{
    if (wet_alarm)
    {
        for (int i = 0; i < 300; i++)
        {
            tone(3, 600 - i);
        }
        if (bo_BUTT1.pressed())
        {
            wet_alarm = 0;
        }
    }
}
//温度报警铃
void tempAlarm_rinf()
{
    if (temp_alarm)
    {
        for (int i = 0; i < 300; i++)
        {
            tone(3, 100 + i);
        }
        if (bo_BUTT1.pressed())
        {
            temp_alarm = 0;
        }
    }
}
void sec_flash()
{
    if (cnt_bool)
    {
        digitalWrite(LEDs[0], LOW);
    }
    else
    {
        digitalWrite(LEDs[0], HIGH);
    }
}
void mode_LEDs()
{
    switch (flag)
    {
    case 0:
        digitalWrite(LED3, HIGH);
        digitalWrite(LED4, HIGH);
        break;
    case 1:
        digitalWrite(LED3, HIGH);
        digitalWrite(LED4, LOW);
        break;
    case 2:
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, HIGH);
        break;
    case 3:
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);
        break;
    default:
        break;
    }
}
// int new_cnt = 0;
timerMaker timer;
void temp_interr()
{
    // new_cnt++;
}
void alarm_interr()
{
    ++cnt;
    if (cnt >= 1000)
    {
        cnt = 0;
        // temp = float(analogRead(tempport) * 0.4482);
        wet = float(analogRead(wetport) * 0.108);
        timer.secs++;
    }
    if (cnt <= 500)
    {
        cnt_bool = true;
    }
    else
    {
        cnt_bool = false;
    }
    if (timer.hors == timer.alahor && timer.mins == timer.alamin)
    {
        if_ring = 1;
    }
    if (wet > wet_warning)
    {
        wet_alarm = 1;
    }
    if (temp > temp_warning)
    {
        temp_alarm = 1;
    }
}
//中断只做很少的工作，改变一下标记就行，其他放在主函数里。
void Interrupt()
{
    alarm_interr();
    // temp_interr();
}
void setalarm()
{

    bo_BUTT1.update();
    bo_BUTT2.update();
    bo_BUTT3.update();
    if (g_work_mode == MODE_DISPLAY_BLANK)
    {
        tube.closeDisplay();
        if (bo_BUTT1.pressed())
            g_work_mode = 1;
    }
    else if (g_work_mode == MODE_DISPLAY_FULL)
    {
        tube.displayAll();
        if (bo_BUTT1.pressed())
            g_work_mode = 1;
    }
    else
        //模式切换
        // 模式又不够用了，于是改用三个小灯来指示模式，这样就有了八种模式。
        if (bo_BUTT1.pressed())
        {
            flag++;
            flag = flag % 7;
        }
        else
            //校分校时
            if (flag == 1)
            {
                if (bo_BUTT3.pressed())
                    timer.mins++;
                if (bo_BUTT2.pressed())
                    timer.hors++;
            }
            else
                //闹钟
                if (flag == 2)
                {
                    Refresh();
                    // tube.closeDisplay();
                    tube.displayInt(timer.alahor * 100 + timer.alamin);
                    if (bo_BUTT3.pressed())
                        timer.alamin++;
                    if (bo_BUTT2.pressed())
                        timer.alahor++;
                }
                else if (flag == 3)
                {
                    //不知道是不是因为跳线帽的原因，温度只显示个位，而且模式不够用了，于是想着在此模式下，切换显示温度和湿度。
                    //确定了，确实是跳线的问题，得想办法弄一个来。
                    Refresh();
                    // tube.closeDisplay();
                    // tube.setLetter(0, 'C');
                    if (!cnt_bool)
                    {
                        temp = TempRead() * 0.0625;
                        // if (temp > -20 && temp < 150)
                        // {
                        //     Serial.println(temp);
                        // }
                    }

                    tube.displayFloat(0, temp, 1);
                    Refresh();
                    // Serial.println(int(ds.getTempC()));
                }
                else
                    //设置温度预警值
                    if (flag == 4)
                    {
                        Refresh();
                        tube.displayFloat(0, temp_warning, 1);
                        if (bo_BUTT3.pressed())
                            timer.alamin++;
                        if (bo_BUTT2.pressed())
                            timer.alahor++;
                    }
                    else
                        //显示湿度
                        if (flag == 5)
                        {
                            // tube.setLetter(0, 'F');
                            tube.displayFloat(1, wet, 1);
                            Refresh();
                        }
                        else
                            //设置湿度预警值
                            if (flag == 6)
                            {
                                Refresh();
                                tube.displayFloat(1, wet_warning, 1);
                                if (bo_BUTT3.pressed())
                                    timer.alamin++;
                                if (bo_BUTT2.pressed())
                                    timer.alahor++;
                            }
}

unsigned char g_rxBuffer[50]; //接受包
unsigned char g_txBuffer[50]; //发送包

unsigned char cal_check_sum(unsigned char *data, int length)
{
    unsigned char result = 0;
    for (int i = 0; i < length - 1; i++)
    {
        result ^= data[i];
    }
    return result;
}
// A0 03 00 00 00 A3 全亮
// A0 04 00 00 00 A4 全灭
//读取当前时间01
// A0 01 00 00 00 A1
void comm_replay_current_time()
{
    g_txBuffer[0] = 0xb0;
    g_txBuffer[1] = g_rxBuffer[1];
    g_txBuffer[2] = timer.hors;
    g_txBuffer[3] = timer.mins;
    g_txBuffer[4] = timer.secs;
    g_txBuffer[5] = cal_check_sum(g_txBuffer, PACKAGE_LENGTH);
    // g_txBuffer[5] = 0xA4;
}
//改变当前时间02
// A0 02 05 04 36 95
void comm_change_alarm_time()
{
    timer.hors = g_rxBuffer[2];
    timer.mins = g_rxBuffer[3];
    timer.secs = g_rxBuffer[4];
    g_txBuffer[0] = 0xb0;
    g_txBuffer[1] = g_rxBuffer[1];
    g_txBuffer[2] = 1;
    g_txBuffer[3] = 0;
    g_txBuffer[4] = 0;
    g_txBuffer[5] = cal_check_sum(g_txBuffer, PACKAGE_LENGTH);
}
//读取当前温度05
// A0 05 00 00 00 A5
void comm_replay_current_tem()
{
    g_txBuffer[0] = 0xb0;
    g_txBuffer[1] = g_rxBuffer[1];
    int t = temp * 10;
    unsigned char high_byte = (t & 0xff00) >> 8;
    unsigned char low_byte = char(t & 0x00ff);
    g_txBuffer[2] = high_byte; //和上面一样，后面改一下
    g_txBuffer[3] = low_byte;
    g_txBuffer[4] = 0;
    g_txBuffer[5] = cal_check_sum(g_txBuffer, PACKAGE_LENGTH);
}
//设置温度06
// A0 06 01 01 00 A6
void comm_set_alarm_temprature()
{
    int high_byte = g_rxBuffer[2]; //高字节
    int low_byte = g_rxBuffer[3];  //低字节
    temp_warning = (high_byte << 8 + low_byte) / 10.0;
    g_txBuffer[0] = 0xb0;
    g_txBuffer[1] = g_rxBuffer[1];
    g_txBuffer[2] = 1;
    g_txBuffer[3] = 0;
    g_txBuffer[4] = 0;
    g_txBuffer[5] = cal_check_sum(g_txBuffer, PACKAGE_LENGTH);
}
//
//读取当前湿度07
// A0 07 00 00 00 A7
void comm_replay_current_wet()
{
    g_txBuffer[0] = 0xb0;
    g_txBuffer[1] = g_rxBuffer[1];
    int t = wet * 10;
    unsigned char high_byte = (t & 0xff00) >> 8;
    unsigned char low_byte = char(t & 0x00ff);
    g_txBuffer[2] = high_byte; //和上面一样，后面改一下
    g_txBuffer[3] = low_byte;
    g_txBuffer[4] = 0;
    g_txBuffer[5] = cal_check_sum(g_txBuffer, PACKAGE_LENGTH);
}
//设置湿度08
// A0 08 01 01 00 A8
void comm_set_alarm_wetness()
{
    //懂了，因为乘了10，所以是比较大的，超过了char，所以分两个存放。
    int high_byte = g_rxBuffer[2]; //高字节
    int low_byte = g_rxBuffer[3];  //低字节
    wet_warning = (high_byte << 8 + low_byte) / 10.0;
    g_txBuffer[0] = 0xb0;
    g_txBuffer[1] = g_rxBuffer[1];
    g_txBuffer[2] = 1;
    g_txBuffer[3] = 0;
    g_txBuffer[4] = 0;
    g_txBuffer[5] = cal_check_sum(g_txBuffer, PACKAGE_LENGTH);
}
//通讯数据解析

void parse_commdata_exec()
{
    if (s_rcv_package) //如果接收到包
    {
        switch (g_rxBuffer[1])
        {
        case CMD_READ_CURRENT_TIME:
            comm_replay_current_time();
            break;
        case CMD_SET_ALARM_TIME:
            comm_change_alarm_time();
            break;
        case CMD_SET_LED_FULL:
            g_work_mode = MODE_DISPLAY_FULL;
            break;
        case CMD_SET_LED_BLANK:
            g_work_mode = MODE_DISPLAY_BLANK;
            break;
        case CMD_READ_CURRENT_TEM:
            comm_replay_current_tem();
            break;
        case CMD_SET_ALARM_TEMPR:
            comm_set_alarm_temprature();
            break;
        case CMD_READ_CURRENT_HUMIDITY:
            comm_replay_current_wet();
            break;
        case CMD_SET_ALARM_HUMIDITY:
            comm_set_alarm_wetness();
            break;
        default:;
        }
    }
}

//校验和
bool check_sum(unsigned char *data, int length)
{
    unsigned char result = 0;
    for (int i = 0; i <= length - 1; i++)
    {
        result ^= data[i];
    }
    return !((bool)result);
}
//功能码

void serialEvent()
{
    if (check_sum(g_txBuffer, PACKAGE_LENGTH) && (s_rcv_package == true) && g_work_mode != MODE_DISPLAY_FULL && g_work_mode != MODE_DISPLAY_BLANK)
    {
        Serial.write(g_txBuffer, PACKAGE_LENGTH);
        s_rcv_package = false;
    }

    if (Serial.available())
    {
        Serial.readBytes(g_rxBuffer, PACKAGE_LENGTH);
        if ((g_rxBuffer[0] == RX_HEADER) && check_sum(g_rxBuffer, PACKAGE_LENGTH))
        {
            s_rcv_package = true;
        }
    }
}

void setup()
{
    // put your setup code here, to run once:
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    pinMode(BUTT1, INPUT);
    pinMode(BUTT2, INPUT);
    pinMode(BUTT3, INPUT);
    bo_BUTT1.attach(BUTT1);
    bo_BUTT1.interval(50);
    bo_BUTT2.attach(BUTT2);
    bo_BUTT2.interval(50);
    bo_BUTT3.attach(BUTT3);
    bo_BUTT3.interval(50);
    MsTimer2::set(1, Interrupt);
    MsTimer2::start(); //中断
    Serial.begin(9600);
}
void loop()
{

    timer.start();         //计时开始
    mode_LEDs();           //模式灯
    sec_flash();           //秒灯
    setalarm();            //按钮调节
    serialEvent();         //串口事件
    parse_commdata_exec(); //协议处理
}
