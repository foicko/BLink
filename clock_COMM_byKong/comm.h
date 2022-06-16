#ifndef _COMM_H_
#define _COMM_H_
#define RX_HEADER 0XA0
#define TX_HEADER 0XB0
#define PACKAGE_LENGTH 6

#define MODE_RUN                  0
#define MODE_MODIFY_HOUR          1
#define MODE_MODIFY_MINUTE        2
#define MODE_SET_HOUR_ALARM       3
#define MODE_SET_MINUTE_ALARM     4
#define MODE_DISPLAY_BLANK        5
#define MODE_DISPLAY_FULL         6
#define MODE_SET_ALARM_ON_TEST    7
#define MODE_SET_ALARM_OFF_TEST   8

#define CMD_READ_CURRENT_TIME  1
#define CMD_SET_ALARM_TIME     2
#define CMD_SET_LED_FULL       3
#define CMD_SET_LED_BLANK      4
#define CMD_READ_CURRENT_TEM   5
#define CMD_SET_ALARM_TEMPR    6
#define CMD_READ_CURRENT_HUMIDITY  7
#define CMD_SET_ALARM_HUMIDITY     8



extern bool s_rcv_package;
extern int g_work_mode;
extern unsigned char g_rxBuffer[];
extern unsigned char g_txBuffer[];
void  parse_commdata_exec();
#endif
