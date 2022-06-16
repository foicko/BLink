
#include "Arduino.h"
#include "display.h"
#define NOP do { __asm__ __volatile__ ("nop"); } while (0)

//SenvenSegmentCode[10]:全灭；SenvenSegmentCode[11]:全亮
const unsigned int SenvenSegmentCode[12] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0};



void LED7_display::write_hc595(unsigned int data)
{
    digitalWrite(latch,LOW);
    NOP;
    for (int i=0;i<16;i++)
    {
        digitalWrite(clk,LOW);
        NOP;
        digitalWrite(sdi,(bool)(((data << i)&0x8000)));
        NOP;
        digitalWrite(clk,HIGH);
        NOP;
    }
    digitalWrite(latch,HIGH);
    NOP;
}
//显示某位置数字不闪烁
void LED7_display::display_postion(int data,int position)
{
    write_hc595((data << 8) | ( 1<< position));

}
//某位置数字闪烁
void LED7_display::display_postion_blink(int data,int position)
{
    
    position_blink_counter[position]++;
    if (position_blink_counter[position] >= 512) {position_blink_counter[position] = 0;}
    if (position_blink_counter[position] < 255)
    {
        display_postion(data,position);
    }
    else
    {
        display_postion(SenvenSegmentCode[10],position);
    }
}
//返回添加小数点后的段码
int LED7_display::get_point_data(int data)
{
    if (g_flag_1HZ)
    {
        return data;
    }
    else
    {
        return data & 0x7f;
    }

}
//全部熄灭
void LED7_display::dispaly_blank()
{
    for (int i = 0;i <= 3;i++)
    {
        display_postion(SenvenSegmentCode[10],i);
    }
}
//全部点亮
void LED7_display::display_full()
{
    for (int i = 0;i <= 3;i++)
    {
        display_postion(SenvenSegmentCode[11],i);
    }
}
//数字闪烁的时候
void LED7_display::display_4num(int *data,int blink_num_position_mask,int blink_point_position_mask)
{
    for (int i=0;i<=3;i++)
    {
        int temp;
        if (blink_point_position_mask & (1<<i))
        {
            temp = get_point_data(SenvenSegmentCode[data[i]]);
        }
        else
        {
            temp = SenvenSegmentCode[data[i]];
        }

        if (blink_num_position_mask & (1 << i))
        {
            display_postion_blink(temp,i);
        }
        else
        {
            display_postion(temp,i);
        }
    }

}
