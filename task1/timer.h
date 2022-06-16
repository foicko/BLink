#ifndef _TIMER_H_
#define _TIMER_H_
/* 软定时器类
 * 单次定时模式
 * 在中断函数中调用timer_function,实现计数、计数结束判断等工作，计数结束置标志 flag_count_over
 */
class m_timer
{
public:
    m_timer(int T_ms);
    void restart();
    void stop();
    void timer_function();
    
    bool count_over();
    bool enable;         //if enable, start;else stop,but not reset.
    int goal_value;
    int current_value;   //only reseted by reset.
    bool flag_count_over;//when count over,flag_count_over = true,only reseted by restart().
};
#endif
