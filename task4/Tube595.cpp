#ifndef TUBE595_cpp
#define TUBE595_cpp

#include "Tube595.h"

unsigned char Tube595::LED_MODEL[17] =
    {
        // 0    1     2   3    4    5    6    7    8     9   A    B    C    D    E    F    -
        0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x8C, 0xBF, 0xC6, 0xA1, 0x86, 0x8E, 0xbf};

unsigned char Tube595::LedData[4] = {0xFF, 0xFF, 0xFF, 0xFF};

Tube595::Tube595(int dataPin, int sclkPin, int rclkPin)
{
    DIO = dataPin;
    SCLK = sclkPin;
    RCLK = rclkPin;

    pinMode(SCLK, OUTPUT);
    pinMode(RCLK, OUTPUT);
    pinMode(DIO, OUTPUT);
}

void Tube595::setLetter(int place, char letter)
{
    if ((letter >= 'A') & (letter <= 'F'))
        LedData[place] = LED_MODEL[letter - 55];
    if (letter == '-')
        LedData[place] = LED_MODEL[16];
}

void Tube595::setDigital(int place, int digital)
{
    Tube595::LedData[place] = Tube595::LED_MODEL[digital];
}

// display a int number
void Tube595::displayInt(int num)
{
    char string[5] = {0};
    sprintf(string, "%4d", num);

    for (int i = 3; i >= 0; i--)
    {
        switch (string[i])
        {
        case ' ':
            break;
        case '-':
            LedData[i] = LED_MODEL[16];
            break;
        default:
            LedData[i] = LED_MODEL[string[i] - '0'];
            // LedData[7-i] = LED_MODEL[1];
            break;
        }
    }
    update();
}
//prec 小数位数
void Tube595::displayFloat(bool pre,float number, int prec)
{
    // Tube595::LedData[0] = 0xFF;
    Tube595::LedData[1] = 0xFF;
    Tube595::LedData[2] = 0xFF;
    Tube595::LedData[3] = 0xFF;
    char str[6] = {0}; //多一位用来放'\0'
    int j = 4;
    int point = 0;
    dtostrf(number, 4, prec, str);

    while (str[j] == 0)
        j--;

    for (int i = 3; i >= 0; i--)
    {

        switch (str[j])
        {
        case ' ':
            break;
        case '-':
            LedData[i] = LED_MODEL[16];
            break;
        case '.':
            LedData[i] &= 0x7F;
            i++;
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            LedData[i] &= LED_MODEL[str[j] - '0'];
            //LedData[7-i] = LED_MODEL[3];
            break;
        }
        // LedData[7-i] = LED_MODEL[i];
        j--;
    }
    if(pre){
        LedData[0] = LED_MODEL['F' - 55];
    }else
        LedData[0] = LED_MODEL['C' - 55];
    update();
}

void Tube595::setNoPoint(int place)
{
    LedData[place] |= 0x80;
}

void Tube595::setPoint(int place)
{
    LedData[place] &= 0x7F;
}

void Tube595::closeDisplay()
{
    for (int i = 0; i < 4; i++)
        LedData[i] = 0xFF;

    update();
}
void Tube595::displayAll()
{
    for (int i = 0; i < 4; i++)
        LedData[i] = 0x00;

    update();
}

void Tube595::update()
{
    for (int i = 0; i < 4; i++)
    {
        shiftOut(DIO, SCLK, MSBFIRST, LedData[i]);
        shiftOut(DIO, SCLK, MSBFIRST, 1 << i);
        digitalWrite(RCLK, LOW);
        digitalWrite(RCLK, HIGH);
    }
}

#endif
