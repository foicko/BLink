//当然不可能自己编曲了
/* Author: Waao666
 * Created on 2020-1-7  
 * Version 1.0 
 */
/* 以上部分是定义是把每个音符和频率值对应起来 */
#define NOTE_D0 -1
#define NOTE_D1 294
#define NOTE_D2 330
#define NOTE_D3 350
#define NOTE_D4 393
#define NOTE_D5 441
#define NOTE_D6 495
#define NOTE_D7 556

#define NOTE_DL1 147
#define NOTE_DL2 165
#define NOTE_DL3 175
#define NOTE_DL4 196
#define NOTE_DL5 221
#define NOTE_DL6 248
#define NOTE_DL7 278

#define NOTE_DH1 589
#define NOTE_DH2 661
#define NOTE_DH3 700
#define NOTE_DH4 786
#define NOTE_DH5 882
#define NOTE_DH6 990
#define NOTE_DH7 112
/**************************************/

/********** 这部分是用英文对应了拍子 **********/
#define WHOLE 1
#define HALF 0.5
#define QUARTER 0.25
#define EIGHTH 0.25
#define SIXTEENTH 0.625
/******************************************/

int length;            /* 这里定义一个变量，后面用来表示共有多少个音符 */
const int tonePin = 3; /* 蜂鸣器的pin */
                       /* 这部分就是整首曲子的音符部分,用了一个序列定义为tune，整数 */

class Song
{
public:
    void setup()
    {
        /* 设置蜂鸣器的pin为输出模式 */
        pinMode(tonePin, OUTPUT);
        /* 这里用了一个sizeof函数， 可以查出tone序列里有多少个音符 */
        length = sizeof(tune) / sizeof(tune[0]);
    }

    Song()
    {
        setup();
    }

    int tune[] = {
        NOTE_D0, NOTE_D0, NOTE_D0, NOTE_D6, NOTE_D7, NOTE_DH1, NOTE_D7, NOTE_DH1, NOTE_DH3, NOTE_D7, NOTE_D7, NOTE_D7, NOTE_D3, NOTE_D3,
        NOTE_D6, NOTE_D5, NOTE_D6, NOTE_DH1, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D3, NOTE_D4, NOTE_D3, NOTE_D4, NOTE_DH1,
        NOTE_D3, NOTE_D3, NOTE_D0, NOTE_DH1, NOTE_DH1, NOTE_DH1, NOTE_D7, NOTE_D4, NOTE_D4, NOTE_D7, NOTE_D7, NOTE_D7, NOTE_D0, NOTE_D6, NOTE_D7,
        NOTE_DH1, NOTE_D7, NOTE_DH1, NOTE_DH3, NOTE_D7, NOTE_D7, NOTE_D7, NOTE_D3, NOTE_D3, NOTE_D6, NOTE_D5, NOTE_D6, NOTE_DH1,
        NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D2, NOTE_D3, NOTE_D4, NOTE_DH1, NOTE_D7, NOTE_D7, NOTE_DH1, NOTE_DH1, NOTE_DH2, NOTE_DH2, NOTE_DH3, NOTE_DH1, NOTE_DH1, NOTE_DH1,
        NOTE_DH1, NOTE_D7, NOTE_D6, NOTE_D6, NOTE_D7, NOTE_D5, NOTE_D6, NOTE_D6, NOTE_D6, NOTE_DH1, NOTE_DH2, NOTE_DH3, NOTE_DH2, NOTE_DH3, NOTE_DH5,
        NOTE_DH2, NOTE_DH2, NOTE_DH2, NOTE_D5, NOTE_D5, NOTE_DH1, NOTE_D7, NOTE_DH1, NOTE_DH3, NOTE_DH3, NOTE_DH3, NOTE_DH3, NOTE_DH3,
        NOTE_D6, NOTE_D7, NOTE_DH1, NOTE_D7, NOTE_DH2, NOTE_DH2, NOTE_DH1, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_DH4, NOTE_DH3, NOTE_DH2, NOTE_DH1,
        NOTE_DH3, NOTE_DH3, NOTE_DH3, NOTE_DH3, NOTE_DH6, NOTE_DH6, NOTE_DH5, NOTE_DH5, NOTE_DH3, NOTE_DH2, NOTE_DH1, NOTE_DH1, NOTE_D0, NOTE_DH1,
        NOTE_DH2, NOTE_DH1, NOTE_DH2, NOTE_DH2, NOTE_DH5, NOTE_DH3, NOTE_DH3, NOTE_DH3, NOTE_DH3, NOTE_DH6, NOTE_DH6, NOTE_DH5, NOTE_DH5,
        NOTE_DH3, NOTE_DH2, NOTE_DH1, NOTE_DH1, NOTE_D0, NOTE_DH1, NOTE_DH2, NOTE_DH1, NOTE_DH2, NOTE_DH2, NOTE_D7, NOTE_D6, NOTE_D6, NOTE_D6, NOTE_D6, NOTE_D7};
    /****************************************************/

    /* 这部分是整首曲子的接拍部分，也定义个序列duration，浮点（数组的个数和前面音符的个数是一样的） */
    float duration[] = {
        1, 1, 1, 0.5, 0.5, 1 + 0.5, 0.5, 1, 1, 1, 1, 1, 0.5, 0.5,
        1 + 0.5, 0.5, 1, 1, 1, 1, 1, 1, 1 + 0.5, 0.5, 1, 1,
        1, 1, 0.5, 0.5, 0.5, 0.5, 1 + 0.5, 0.5, 1, 1, 1, 1, 1, 0.5, 0.5,
        1 + 0.5, 0.5, 1, 1, 1, 1, 1, 0.5, 0.5, 1 + 0.5, 0.5, 1, 1,
        1, 1, 1, 0.5, 0.5, 1, 0.5, 0.25, 0.25, 0.25, 0.5, 0.5, 0.5, 0.5, 0.25, 0.5, 1,
        0.5, 0.5, 0.5, 0.5, 1, 1, 1, 1, 1, 0.5, 0.5, 1 + 0.5, 0.5, 1, 1,
        1, 1, 1, 0.5, 0.5, 1.5, 0.5, 1, 1, 1, 1, 1, 1,
        0.5, 0.5, 1, 1, 0.5, 0.5, 1.5, 0.25, 0.5, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 0.5, 0.5, 1, 1, 0.5, 0.5,
        1, 0.5, 0.5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0.5, 0.5, 1, 1, 0.5, 0.5, 1, 0.5, 0.25, 0.5, 1, 1, 1, 1, 0.5, 0.5

    };
    /**********************************************************************************/
    void start()
    {
        /* 循环音符的次数 */
        for (int x = 0; x < length; x++)
        {
            /* 此函数依次播放tune序列里的数组，即每个音符 */
            tone(tonePin, tune[x]);
            /* 每个音符持续的时间，即节拍duration，是调整时间的越大，曲子速度越慢 */
            delay(400 * duration[x]);
            /* 停止当前音符，进入下一音符 */
            noTone(tonePin);
        }
        // delay(5000);
    }
};
