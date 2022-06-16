#ifndef _DISPLAY_H_
#define _DISPLAY_H_

//#define DISPLAY_DATA_IN   8  //595数据串入
//#define DISPLAY_SHIFT_CLK 7  //595移位脉冲信号，上升沿有效
//#define DISPLAY_LATCH_CLK 4  //595锁存信号，上升沿有效



extern volatile bool g_flag_1HZ;

class LED7_display
{
    public: 
        static int position_blink_counter[4];
        LED7_display(int CLK,int SDI,int LATCH)
        {
            clk = CLK;
            sdi = SDI;
            latch = LATCH;
        };   //初始化，绑定管脚
        void dispaly_blank();
        void display_full();
        //blink_num_position_mask，blink_num_position_mask为状态掩码，低四位有效。当相应位为1是，该位有效（有小数点，或者具有闪烁功能）
        void display_4num(int *,int blink_num_position_mask,int blink_point_position_mask);  
    private:
        
        int clk,sdi,latch;
        void write_hc595(unsigned int data);
        void display_postion(int data,int position);
        void display_postion_blink(int data,int position);
        int  get_point_data(int data);                           
};

#endif
