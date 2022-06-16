#ifndef TUBE595_h
#define TUBE595_h

#include <avr/io.h>
#include <Arduino.h>
#include <avr/interrupt.h>

class Tube595
{
public:
    Tube595(int dataPin, int sclkPin, int rclkPin);

    void setLetter(int place, char letter);
    void setDigital(int place, int digital);

    void setPoint(int place);
    void setNoPoint(int place);

    void displayInt(int num);
	void displayFloat(bool pre,float number,int prec);

    void closeDisplay();
    void displayAll();
    void update();

private:
    static unsigned char LED_MODEL[17];

    // Default not display
    static unsigned char LedData[4];

    int DIO;  // Arduino pin for Data	   数据引脚
    int SCLK; // Arduino pin for CLK     时钟引脚，高电平将数据串行移入
    int RCLK; // Arduino pin for LATCH   锁存引脚，高电平将锁存数据送到595引脚上
};

#endif
