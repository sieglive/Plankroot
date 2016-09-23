# include "acllib.h"
# include <windows.h>
# include <math.h>
# include <time.h>
# include <stdio.h>
# include <direct.h>
# include <string.h>

# define PI 3.141592953
# define GRAY RGB(160, 160, 160)
# define ANTIQUE_WHITE RGB(250,235,215)

# define WINDOW_WIDTH 200
# define WINDOW_HEIGHT 200
# define DIAL_RADIUS 90

int timerflag = 0;

typedef enum {
    TIMER_booting,
    TIMER_working,
    TIMER_shutting,
    TIMER_free,
}TIMER_STAT;

void timerEvent(int tid);
void picture_a_minute(int tid);
void keyEvent(int key, int event);

int Setup()
{
    initWindow("MyWindow", DEFAULT, DEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    registerKeyboardEvent(keyEvent);
    registerTimerEvent(timerEvent);
    timerflag = TIMER_working;
    startTimer(0, 20);

    return 0;
}

void keyEvent(int key, int event)
{
    if (key == VK_SPACE) {
        if      ((timerflag == TIMER_working) && (event == KEY_DOWN)) {
            timerflag = TIMER_shutting;
            cancelTimer(0);
        }
        else if ((timerflag == TIMER_shutting) && (event == KEY_UP)) {
            timerflag = TIMER_free;
        }
        else if ((timerflag == TIMER_free) && (event == KEY_DOWN)) {
            timerflag = TIMER_booting;
            startTimer(0, 100);
        }
        else if ((timerflag == TIMER_booting) && (event == KEY_UP)) {
            timerflag = TIMER_working;
        }
    }
}

void timerEvent(int tid)
{
    picture_a_minute(tid);
}

void picture_a_minute(int tid)
{
    int x, y;
    // 获取表中心点坐标
    int cntX = getWidth()/2, cntY = getHeight()/2;
    // 表盘半径
    int r_dial = DIAL_RADIUS;
    // 计算刻度大圆和小圆半径
    int r = (double)r_dial/15, r2 = (double)r_dial/25;
    // 计算时针分针秒针的长度
    int r_sec = (double)r_dial * 4 / 5, r_min = r_dial * 5 / 8, r_h = r_dial / 2;

    // 时分秒的中间变量
    double sec, min, h;
    // 存储时间文本
    char timetext[40];
    // 绘制指针时的顶点存储变量
    int second[4][2], minute[4][2], hour[4][2];

    // 获取本地时间
    time_t timer = time(NULL);
    struct tm *timel = localtime(&timer);

    beginPaint();
    clearDevice();

    // 画表盘
    setBrushColor(ANTIQUE_WHITE);
    setPenColor(ANTIQUE_WHITE);
    ellipse(cntX - r_dial, cntY - r_dial, cntX + r_dial, cntY + r_dial);

    // 大刻度
    setBrushColor(RGB(231, 61, 105));
    setPenColor(RGB(231, 61, 105));
    for (int j = 0; j < 12; j++)
    {
        if (!(j % 3))
        {
            x = -r_dial * sin((double)j * 30 / 180 * PI) + cntX;
            y = r_dial * cos((double)j * 30 / 180 * PI) + cntY;
            ellipse(x - r, y - r, x + r, y + r);
        }
    }

    // 小刻度
    setBrushColor(RGB(0, 138, 212));
    setPenColor(RGB(0, 138, 212));
    for (int j = 0; j < 12; j++)
    {
        if (j % 3)
        {
            x = -r_dial * sin((double)j * 30 / 180 * PI) + cntX;
            y = r_dial * cos((double)j * 30 / 180 * PI) + cntY;
            ellipse(x - r2, y - r2, x + r2, y + r2);
        }
    }

    // 时针
    setBrushColor(GRAY);
    setPenColor(GRAY);
    timel->tm_hour;
    h = (double)(timel->tm_hour % 12 * 30 + timel->tm_min / 2) / 180 * PI;
    x = r_h * sin(h) + cntX;
    y = -r_h * cos(h) + cntY;
    hour[0][0] = x;
    hour[0][1] = y;
    hour[2][0] = cntX - (x - cntX) / 10;
    hour[2][1] = cntY - (y - cntY) / 10;
    hour[1][0] = cntX + (hour[2][1] - cntY) / 2;
    hour[1][1] = cntY - (hour[2][0] - cntX) / 2;
    hour[3][0] = cntX - (hour[2][1] - cntY) / 2;
    hour[3][1] = cntY + (hour[2][0] - cntX) / 2;
    polygon(hour, 4);

    // 分针
    setBrushColor(GRAY);
    setPenColor(GRAY);
    min = (double)(timel->tm_min * 6 + timel->tm_sec / 10) / 180 * PI;
    x = r_min * sin(min) + cntX;
    y = -r_min * cos(min) + cntY;
    minute[0][0] = x;
    minute[0][1] = y;
    minute[2][0] = cntX - (x - cntX) / 10;
    minute[2][1] = cntY - (y - cntY) / 10;
    minute[1][0] = cntX + (minute[2][1] - cntY) / 2;
    minute[1][1] = cntY - (minute[2][0] - cntX) / 2;
    minute[3][0] = cntX - (minute[2][1] - cntY) / 2;
    minute[3][1] = cntY + (minute[2][0] - cntX) / 2;
    polygon(minute, 4);

    // 秒针
    setBrushColor(GREEN);
    setPenColor(GREEN);
    sec = (double)timel->tm_sec * 6 / 180 * PI;
    x = r_sec * sin(sec) + cntX;
    y = -r_sec * cos(sec) + cntY;
    second[0][0] = x;
    second[0][1] = y;
    second[2][0] = cntX - (x - cntX) / 10;
    second[2][1] = cntY - (y - cntY) / 10;
    second[1][0] = cntX + (second[2][1] - cntY) / 4;
    second[1][1] = cntY - (second[2][0] - cntX) / 4;
    second[3][0] = cntX - (second[2][1] - cntY) / 4;
    second[3][1] = cntY + (second[2][0] - cntX) / 4;
    polygon(second, 4);

    // 日期
    /*setBrushColor(GREEN);
    setPenColor(GREEN);
    setTextSize(24);
    sprintf(timetext, "%d年%d月%d日 %02d:%02d:%02d", timel->tm_year + 1900, timel->tm_mon + 1, timel->tm_mday, timel->tm_hour, timel->tm_min, timel->tm_sec);
    paintText(20, 20, timetext);*/

    endPaint();
    return;
}
