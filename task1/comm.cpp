// // #include "comm.h"
// unsigned char g_rxBuffer[50]; //接受包
// unsigned char g_txBuffer[50]; //发送包

// unsigned char cal_check_sum(unsigned char *data, int length)
// {
//     unsigned char result = 0;
//     for (int i = 0; i < length - 1; i++)
//     {
//         result ^= data[i];
//     }
//     // data[length - 1]= result;
//     return result;
// }
// //读取当前时间01
// void comm_replay_current_time()
// {
//     // Serial.println("I am Ok");
//     g_txBuffer[0] = 0xb0;
//     g_txBuffer[1] = g_rxBuffer[1];
//     g_txBuffer[2] = 0x12;
//     g_txBuffer[3] = 0x13;
//     g_txBuffer[4] = 0x14;
//     // g_txBuffer[5] = cal_check_sum(g_txBuffer, 6);
//     g_txBuffer[5] = 0xA4;
// }
// //改变当前时间02
// void comm_change_alarm_time()
// {
//     timer.hors = g_rxBuffer[2];
//     timer.mins = g_rxBuffer[3];
//     timer.secs = g_rxBuffer[4];
//     g_txBuffer[0] = 0xb0;
//     g_txBuffer[1] = g_rxBuffer[1];
//     g_txBuffer[2] = 1;
//     g_txBuffer[3] = 0;
//     g_txBuffer[4] = 0;
//     g_txBuffer[5] = cal_check_sum(g_txBuffer, 6);
// }
// // // 全灭03
// // void comm_close_display()
// // {
// // }
// // // 全亮 04
// // void comm_display_all()
// // {
// // }
// //读取当前温度05
// void comm_replay_current_tem()
// {
//     g_txBuffer[0] = 0xb0;
//     g_txBuffer[1] = g_rxBuffer[1];
//     // g_txBuffer[2] = temp; //温度高字节 温度改成float类型
//     // g_txBuffer[3] = temp; //温度低字节 数码管显示还得改加一个小数显示。
//     g_txBuffer[4] = 0;
//     g_txBuffer[5] = cal_check_sum(g_txBuffer, 6);
// }
// //设置温度06
// void comm_set_alarm_temprature()
// {
//     // temp = g_rxBuffer[2]; //高字节
//     // temp = g_rxBuffer[3]; //低字节
//     g_txBuffer[0] = 0xb0;
//     g_txBuffer[1] = g_rxBuffer[1];
//     g_txBuffer[2] = 1;
//     g_txBuffer[3] = 0;
//     g_txBuffer[4] = 0;
//     g_txBuffer[5] = cal_check_sum(g_txBuffer, 6);
// }
// //
// //读取当前湿度07
// void comm_replay_current_wet()
// {
//     g_txBuffer[0] = 0xb0;
//     g_txBuffer[1] = g_rxBuffer[1];
//     // g_txBuffer[2] = wet; //和上面一样，后面改一下
//     // g_txBuffer[3] = wet;
//     g_txBuffer[4] = 0;
//     g_txBuffer[5] = cal_check_sum(g_txBuffer, 6);
// }
// //设置湿度08
// void comm_set_alarm_wetness()
// {
//     // wet = g_rxBuffer[2]; //高字节
//     // wet = g_rxBuffer[3]; //低字节
//     g_txBuffer[0] = 0xb0;
//     g_txBuffer[1] = g_rxBuffer[1];
//     g_txBuffer[2] = 1;
//     g_txBuffer[3] = 0;
//     g_txBuffer[4] = 0;
//     g_txBuffer[5] = cal_check_sum(g_txBuffer, 6);
// }
// //通讯数据解析

// void parse_commdata_exec()
// {
//     if (s_rcv_package) //如果接收到包
//     {
//         switch (g_rxBuffer[1])
//         {
//         case CMD_READ_CURRENT_TIME:
//             comm_replay_current_time();
//             break;
//         case CMD_SET_ALARM_TIME:
//             comm_change_alarm_time();
//             break;
//         case CMD_SET_LED_FULL:
//             g_work_mode = MODE_DISPLAY_FULL;
//             break;
//         case CMD_SET_LED_BLANK:
//             g_work_mode = MODE_DISPLAY_BLANK;
//             break;
//         case CMD_READ_CURRENT_TEM:
//             comm_replay_current_tem();
//             break;
//         case CMD_SET_ALARM_TEMPR:
//             comm_set_alarm_temprature();
//             break;
//         case CMD_READ_CURRENT_HUMIDITY:
//             comm_replay_current_wet();
//             break;
//         case CMD_SET_ALARM_HUMIDITY:
//             comm_set_alarm_wetness();
//             break;
//         default:;
//         }

//     }
// }
