#include "timer.h"

m_timer::m_timer(int T_ms)//构造函数，设置定时时间 
{
    goal_value = T_ms;
    enable = true;
    current_value = 0;
    flag_count_over = false;
}

void m_timer::restart()
{
    enable = true;
    current_value = 0;
    flag_count_over = false;
}
void m_timer::stop()
{
    enable = false;
    current_value = 0;
}
bool m_timer::count_over()
{
    return flag_count_over; 
}
//软定时器主体函数，在中断服务程序中调用
//完成计数、置标志位等操作
void m_timer::timer_function()
{
    if (enable)
    {
        if (current_value >= goal_value - 1)
        {
            flag_count_over = true;
            enable = false;
        }
        else
        {
            current_value++;
        }
    }
}
