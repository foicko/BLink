#include <MsTimer2.h>
// #include "Song.h"
#include <Bounce2.h> //用来消抖的
#include "Tube595.h"
#include "DS18B20.h"
Bounce2::Button bo_butt1 = Bounce2::Button();
Bounce2::Button bo_butt2 = Bounce2::Button();
Bounce2::Button bo_butt3 = Bounce2::Button();
//四个小灯
const int led1 = 13;
const int led2 = 12;
const int led3 = 11;
const int led4 = 10;
const int leds[] = {led1, led2, led3, led4};

//三个按钮
const int butt1 = 15;
const int butt2 = 16;
const int butt3 = 17;

int flag = 3;
int before = 0;
//按钮状态
int butt_state1 = 0;
int butt_state2 = 0;
int butt_state3 = 0;
//74_595
const int sdi = 8; //数据
const int sft = 7; //输入时钟
const int lch = 4; //锁存时钟
//铃声
int if_ring = 0;
const int alarm = 3;
int cnt = 0;
bool cnt_bool = 0;

Tube595 tube(sdi, sft, lch);

//  Song sky;
String data = "0159";
int seted_hour;
int seted_min;

//温度
int temp = 0;
//湿度
int wet = 0;
//湿度传感器（假）引脚
#define wetport A0
#define tempport A4
DS18B20 ds(18);

void refresh()
{
    tube.setDigital(0, 0);
    tube.setDigital(1, 0);
    tube.setDigital(2, 0);
    tube.setDigital(3, 0);
}
void Ring()
{
    //  if(if_ring)
    //  sky.start();
}
class timerMaker
{
public:
    int secs = 0;  //秒
    int mins = 9;  //分
    int hors = 22; //时
    //闹钟时间
    int alamin = 55, alahor = 23;
    timerMaker() {}
    timerMaker(int sec, int min, int hour)
    {
        secs = sec;
        mins = min;
        hors = hour;
    }
    void start();
    void stop();
    void alarm_func();
};

void timerMaker::start()
{
    secs = millis() / 1000;
    secs = secs + 56;
    if (secs - before > 59)
    {
        mins++;
        before += 60;
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
    //不显示闹钟和温度是，显示时间。
    if (flag != 3 && flag != 2)
    {
        refresh();
        tube.displayInt(hors * 100 + mins);
    }
    // tube.displayInt(mins * 100 + secs - before);
    Ring();
}
void sec_flash()
{
    if (cnt_bool)
    {
        digitalWrite(leds[0], LOW);
    }
    else
        digitalWrite(leds[0], HIGH);
}
void mode_leds()
{
    switch (flag)
    {
    case 0:
        digitalWrite(led3, HIGH);
        digitalWrite(led4, HIGH);
        break;
    case 1:
        digitalWrite(led3, HIGH);
        digitalWrite(led4, LOW);
        break;
    case 2:
        digitalWrite(led3, LOW);
        digitalWrite(led4, HIGH);
        break;
    case 3:
        digitalWrite(led3, LOW);
        digitalWrite(led4, LOW);
        break;
    default:
        break;
    }
}
timerMaker timer;
//中断只做很少的工作，改变一下标记就行，其他放在主函数里。
void alarm_func()
{
    ++cnt;
    if (cnt >= 1000)
    {
        cnt = 0;
        if(ds.selectNext())
            temp = int(ds.getTempC());
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
        if_ring = 1;
}
void setalarm()
{

    bo_butt1.update();
    bo_butt2.update();
    bo_butt3.update();

    //模式切换
    if (bo_butt1.pressed())
    {
        flag++;
        flag = flag % 4;
    }
    //校分
    if (flag == 1)
    {
        if (bo_butt3.pressed())
            timer.mins++;
        if (bo_butt2.pressed())
            timer.hors++;
    }
    //闹钟
    if (flag == 2)
    {
        refresh();
        // tube.closeDisplay();
        tube.displayInt(timer.alahor * 100 + timer.alamin);
        if (bo_butt3.pressed())
            timer.alamin++;
        if (bo_butt2.pressed())
            timer.alahor++;
    }
    if (flag == 3)
    {
        //不知道是不是因为跳线帽的原因，温度只显示个位，而且模式不够用了，于是想着在此模式下，切换显示温度和湿度。
        //确定了，确实是跳线的问题，得想办法弄一个来。
        refresh();
        // tube.closeDisplay();
        if (cnt_bool)
        {
            tube.setLetter(0, 'C');
            tube.displayInt(temp);
        }
        else
        {
            tube.setLetter(0, 'F');
            tube.displayInt(wet);
        }
        Serial.println(temp);
    }
}

//初步写了一个很简单的demo，数据是字符串形式，只有四位，前两位表示操作数，后两位表示时分
void comTrans()
{
    while (Serial.available() > 0)
    {
        data += char(Serial.read());
        delay(3); //缓冲区等待
        if (Serial.available() <= 0)
        {
            Serial.println("the str is " + data + "\n");

            // Serial.print(data+"\n");
        }
        delay(3);
    }
    //全亮
    if (data[0] == '9' && data[1] == '9')
    {
        tube.displayAll();
        delay(2000);
    }
    //全灭
    if (data[0] == '6' && data[1] == '6')
    {
        tube.closeDisplay();
        delay(2000);
    }
    //00 调时
    if (data[0] == '0' && data[1] == '0')
    {
        int k = (data[2] - '0') * 10 + (data[3] - '0');
        timer.hors = k;
        Serial.print("The hour has been seted to ");
        Serial.print(k);
        Serial.println();
        refresh();
    }
    //01 调分
    if (data[0] == '0' && data[1] == '1')
    {
        int k = (data[2] - '0') * 10 + (data[3] - '0');
        timer.mins = k;
        Serial.print("The mins has been seted to ");
        Serial.print(k);
        Serial.println();
        refresh();
    }
    //10 闹钟时
    if (data[0] == '1' && data[1] == '0')
    {
        int k = (data[2] - '0') * 10 + (data[3] - '0');
        timer.alahor = k;
        Serial.print("The alarm hour has been seted to ");
        Serial.print(k);
        Serial.println();
        refresh();
    }
    //11 闹钟分
    if (data[0] == '1' && data[1] == '1')
    {
        int k = (data[2] - '0') * 10 + (data[3] - '0');
        timer.alamin = k;
        Serial.print("The alarm mins has been seted to ");
        Serial.print(k);
        Serial.println();
        refresh();
    }
    data = "";
}
void setup()
{
    // put your setup code here, to run once:
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(led4, OUTPUT);
    pinMode(butt1, INPUT);
    pinMode(butt2, INPUT);
    pinMode(butt3, INPUT);
    bo_butt1.attach(butt1);
    bo_butt1.interval(50);
    bo_butt2.attach(butt2);
    bo_butt2.interval(50);
    bo_butt3.attach(butt3);
    bo_butt3.interval(50);
    MsTimer2::set(1, alarm_func);
    MsTimer2::start();
    Serial.begin(9600);
}
void loop()
{
    timer.start();     //计时开始
    mode_leds();       //模式灯
    sec_flash();       //秒灯
    setalarm();        //按钮调节
    MsTimer2::start(); //中断
    comTrans();        //串口通信
}
