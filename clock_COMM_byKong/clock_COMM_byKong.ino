#include <MsTimer2.h>
#include "timer.h"
#include "display.h"
#include "comm.h"

#define BUTTON_MODE   A1  //模式切换键
#define BUTTON_DOWN   A2  //增加键
#define BUTTON_UP     A3  //减小键

#define DISPLAY_DATA_IN   8  //595数据串入
#define DISPLAY_SHIFT_CLK 7  //595移位脉冲信号，上升沿有效
#define DISPLAY_LATCH_CLK 4  //595锁存信号，上升沿有效
#define BEEP              3

#define MODE_LED1   10    //模式指示小灯
#define MODE_LED2   11    //模式指示小灯
#define MODE_LED3   12



#define MASK_DISP_BLINK0 8   //闪烁数字 掩码，最右边，低位
#define MASK_DISP_BLINK1 4
#define MASK_DISP_BLINK2 2
#define MASK_DISP_BLINK3 1
#define MASK_DISP_BLINK_NONE 0
#define MASK_DISP_POINT0 8   //小数点显示 掩码，最右边，低位
#define MASK_DISP_POINT1 4
#define MASK_DISP_POINT2 2
#define MASK_DISP_POINT4 1
#define MASK_DISP_POINT_NONE 0

#define OP_INC 1          //按键后的加减操作符
#define OP_DEC 0

volatile bool g_flag_1HZ = false;
static int s_music_disable = 0;        //消音变量
int g_work_mode = MODE_RUN;
static int s_hour,s_minute,s_second;
static int s_hour_alarm,s_minute_alarm;
bool s_rcv_package = false;
int LED7_display::position_blink_counter[4] = {0,0,0,0};  //类中的静态变量初始化

LED7_display my_led7_display(DISPLAY_SHIFT_CLK,DISPLAY_DATA_IN,DISPLAY_LATCH_CLK);  //实例化我的LED对象

m_timer T_second(500);    //0.5秒
m_timer TP1(10);          //消抖定时器
m_timer TP2(10);
m_timer TP3(10);
m_timer TP4(10);
m_timer TP5(10);
m_timer TP6(10);
m_timer TP7(10);
m_timer TP8(10);
m_timer TP9(10);
m_timer TP10(10);
m_timer TP11(10);

void Timers()//定时器中断服务函数，在此维护全部软定时器
{
    T_second.timer_function();
    TP1.timer_function();
    TP2.timer_function();
    TP3.timer_function();
    TP4.timer_function();
    TP5.timer_function();
    TP6.timer_function();
    TP7.timer_function();
    TP8.timer_function();  
    TP9.timer_function(); 
    TP10.timer_function();  
    TP11.timer_function(); 
}
void setup() 
{
    pinMode(BUTTON_MODE,INPUT);
    pinMode(BUTTON_UP,INPUT);
    pinMode(BUTTON_DOWN,INPUT);

    pinMode(BEEP,OUTPUT);
    digitalWrite(BEEP,HIGH);     //别响
    pinMode(DISPLAY_DATA_IN,OUTPUT);
    pinMode(DISPLAY_SHIFT_CLK,OUTPUT);
    pinMode(DISPLAY_LATCH_CLK,OUTPUT);

    pinMode(MODE_LED1,OUTPUT);
    pinMode(MODE_LED2,OUTPUT);
    pinMode(MODE_LED3,OUTPUT);

    MsTimer2::set(1, Timers);         // 中断设置函数，每 1ms 进入一次中断
    MsTimer2::start();                //开始计时

    Serial.begin(9600);
    Serial.setTimeout(5);             //数据帧之间的间隔超过5mS
}
//通讯校验函数，返回true，校验通过
bool check_sum(unsigned char * data, int length)
{
    unsigned char result;
    for (int i=0;i<=length-1;i++)
    {
        result ^= data[i];
    }
    return !((bool) result);
}

void serialEvent()
{
    if (Serial.available())
    {
        Serial.readBytes(g_rxBuffer,PACKAGE_LENGTH);
        if ((g_rxBuffer[0] == RX_HEADER) && check_sum(g_rxBuffer,PACKAGE_LENGTH))
        {
            s_rcv_package = true;  
        }
    }
}
bool button_eliminate_dither(int key,int & var,int operation_INC_DEC,bool & flag_edge,bool & flag_pressed,m_timer & t1)
{
    //static bool flag_edge,flag_pressed;
    bool Button_Value = digitalRead(key);
    
    if (Button_Value)   //按键没有被按下
    {
        flag_edge = false;
        flag_pressed =false;
        t1.stop();
    }
    if ((!Button_Value) && (!flag_edge))//按键按下的边沿，启动定时器
    {
        flag_edge = true;
        t1.restart();

    }
    if ((t1.count_over()) && (!Button_Value) && flag_edge && (!flag_pressed))//定时结束，按键仍然被按下
    {
        flag_pressed = true;
        if (operation_INC_DEC == OP_DEC)
        {
            var--;     //通过按键递减
        }
        else
        {
            var++;    //通过按键递加
        }
        return true;  //完成一次按键操作
    }
    return false;     //没有按下按键
}

//工作模式切换函数
void mode_switch()
{
    static bool flag_edge;
    static bool flag_pressed;
    if (button_eliminate_dither(BUTTON_MODE,g_work_mode,OP_INC,flag_edge,flag_pressed,TP1))
    {
        if (g_work_mode == 5) 
        {
            g_work_mode = 0;
        }
    }

    digitalWrite(MODE_LED1,!(g_work_mode & 1));  //模式指示灯显示
    digitalWrite(MODE_LED2,!(g_work_mode & 2));
    digitalWrite(MODE_LED3,!(g_work_mode & 4));
}
//音乐在这里写
void sing(bool enable)
{
    static unsigned char frequency;
    frequency++;
    if (enable)
    {
        if (frequency <128)
        {
            digitalWrite(BEEP,HIGH);
        }
        else
        {
            digitalWrite(BEEP,LOW);
        }
    }
    else
    {
        digitalWrite(BEEP,HIGH);  //不响了
    }
}
//比较当前时间和闹铃时间,并根据情况启动闹钟
void alarm_function()
{
    if ((s_hour == s_hour_alarm) && (s_minute == s_minute_alarm))
    {
        if (s_music_disable == 0)
        {
            sing(true);  
        }
        else
        {
            sing(false);  
        }
    }
    else
    {
        digitalWrite(BEEP,HIGH);  //不响了  
    }
}
void elimate_music_noise()        //消音函数
{
    static bool flag_edge = false,flag_pressed = false;
    static bool flag_edge1 = false,flag_pressed1 = false;
    button_eliminate_dither(BUTTON_UP,s_music_disable,OP_INC,flag_edge,flag_pressed,TP2);
    if (s_music_disable > 1) 
    {
        s_music_disable = 0;
    } 
    button_eliminate_dither(BUTTON_DOWN,s_music_disable,OP_DEC,flag_edge1,flag_pressed1,TP3);
    if (s_music_disable < 0) 
    {
        s_music_disable = 1;
    } 
}
//时钟工作函数，时分秒计数，2进制计数

void clock_running()
{   
    if (T_second.count_over())
    {
        if (g_flag_1HZ)
        {
            g_flag_1HZ = false;
        }
        else
        {
            g_flag_1HZ = true;
        }
        if (g_flag_1HZ)
        {
            if (s_second >=59)
            {
                s_second = 0;  
                if (s_minute >=59)
                {
                    s_minute = 0; 
                    if (s_hour >= 23)
                    {
                        s_hour = 0;  
                    }
                    else
                    {
                        s_hour++;  
                    }
                }
                else
                {
                    s_minute++;  
                }
            }
            else
            {
                s_second++;  
            }
        }
        T_second.restart();
    }
    
}
//校正小时模式，该函数用于增加或者减小“全局小时”变量
void modify_hour()
{
    static bool flag_edge = false,flag_pressed = false;
    static bool flag_edge1 = false,flag_pressed1 = false;
    button_eliminate_dither(BUTTON_UP,s_hour,OP_INC,flag_edge,flag_pressed,TP4);
    if (s_hour >= 24) 
    {
        s_hour = 0;
    }
    button_eliminate_dither(BUTTON_DOWN,s_hour,OP_DEC,flag_edge1,flag_pressed1,TP5);
    if (s_hour < 0) 
    {
        s_hour = 23;
    }
}
//校正小时模式，该函数用于增加或者减小“全局分钟”变量
void modify_minute()
{
    static bool flag_edge = false,flag_pressed = false;
    static bool flag_edge1 = false,flag_pressed1 = false;
    button_eliminate_dither(BUTTON_UP,s_minute,OP_INC,flag_edge,flag_pressed,TP6);
    if (s_minute >= 60) 
    {
        s_minute = 0;
    }
    button_eliminate_dither(BUTTON_DOWN,s_minute,OP_DEC,flag_edge1,flag_pressed1,TP7);
    if (s_minute < 0) 
    {
        s_minute = 59;
    }
}
void set_hour_alarm()
{
    static bool flag_edge = false,flag_pressed = false;
    static bool flag_edge1 = false,flag_pressed1 = false;
    button_eliminate_dither(BUTTON_DOWN,s_hour_alarm,OP_DEC,flag_edge,flag_pressed,TP8);
    if (s_hour_alarm < 0) 
    {
        s_hour_alarm = 23;
    }
    button_eliminate_dither(BUTTON_UP,s_hour_alarm,OP_INC,flag_edge1,flag_pressed1,TP9);
    if (s_hour_alarm == 24) 
    {
        s_hour_alarm = 0;
    }
}
void set_minute_alarm()
{
    static bool flag_edge = false,flag_pressed = false;
    static bool flag_edge1 = false,flag_pressed1 = false;
    button_eliminate_dither(BUTTON_DOWN,s_minute_alarm,OP_DEC,flag_edge,flag_pressed,TP10);
    if (s_minute_alarm < 0) 
    {
        s_minute_alarm = 59;
    }
    button_eliminate_dither(BUTTON_UP,s_minute_alarm,OP_INC,flag_edge1,flag_pressed1,TP11);
    if (s_minute_alarm == 60) 
    {
        s_minute_alarm = 0;
    }
}

//时钟主体功能，校正小时、校正分钟、设置闹钟时间
void modify_clock_function()
{
    switch (g_work_mode)
    {
        case MODE_MODIFY_HOUR:
            modify_hour();
            break;
        case MODE_MODIFY_MINUTE: 
            modify_minute();
            break;
        case MODE_SET_HOUR_ALARM:  
            set_hour_alarm();
            break;
        case MODE_SET_MINUTE_ALARM:  
            set_minute_alarm();
            break;
        default:
            break; 
    }  

    /* Serial.print("hour ");Serial.print(s_hour);Serial.print("\n");
    Serial.print("minute ");Serial.print(s_minute);Serial.print("\n");
    Serial.print("second ");Serial.print(s_second);Serial.print("\n");
    Serial.print("hour——alarm ");Serial.print(s_hour_alarm);Serial.print("\n");
    Serial.print("minute--alarm ");Serial.print(s_minute_alarm);Serial.print("\n");
*/
}    
void Display()
{
    int data[4];
    switch (g_work_mode)
    {
        case MODE_RUN:
            data[3] = s_minute % 10; 
            data[2] = s_minute / 10;
            data[1] = s_hour % 10;
            data[0] = s_hour / 10;
            my_led7_display.display_4num(data, MASK_DISP_BLINK_NONE, MASK_DISP_POINT2);
            break;
        case MODE_MODIFY_HOUR:
            data[3] = s_minute % 10; 
            data[2] = s_minute / 10;
            data[1] = s_hour % 10;
            data[0] = s_hour / 10;
            my_led7_display.display_4num(data, MASK_DISP_BLINK3 | MASK_DISP_BLINK2, MASK_DISP_POINT_NONE);
            break;
        case MODE_MODIFY_MINUTE: 
            data[3] = s_minute % 10; 
            data[2] = s_minute / 10;
            data[1] = s_hour % 10;
            data[0] = s_hour / 10;
            my_led7_display.display_4num(data, MASK_DISP_BLINK1 | MASK_DISP_BLINK0, MASK_DISP_POINT_NONE);
            break;
        case MODE_SET_HOUR_ALARM:  
            data[3] = s_minute_alarm % 10; 
            data[2] = s_minute_alarm / 10;
            data[1] = s_hour_alarm % 10;
            data[0] = s_hour_alarm / 10;
            my_led7_display.display_4num(data, MASK_DISP_BLINK3 | MASK_DISP_BLINK2, MASK_DISP_POINT_NONE);
            break;
        case MODE_SET_MINUTE_ALARM: 
            data[3] = s_minute_alarm % 10; 
            data[2] = s_minute_alarm / 10;
            data[1] = s_hour_alarm % 10;
            data[0] = s_hour_alarm / 10;
            my_led7_display.display_4num(data, MASK_DISP_BLINK1 | MASK_DISP_BLINK0, MASK_DISP_POINT_NONE);
            break;
        case MODE_DISPLAY_BLANK:
            my_led7_display.dispaly_blank();
            break;
        case  MODE_DISPLAY_FULL:
            my_led7_display.display_full();
            break;
        case MODE_SET_ALARM_ON_TEST:
            sing(true);
        case MODE_SET_ALARM_OFF_TEST:
            sing(false); 
        default:;
    } 
}


void loop() 
{
    mode_switch();           //模式切换
    clock_running();         //时钟主体函数，时分秒工作
    alarm_function();        //比较报警功能
    elimate_music_noise();   //消音处理
    Display();               //数码管显示
    modify_clock_function(); //校时，修改报警时间等

    parse_commdata_exec();
}
