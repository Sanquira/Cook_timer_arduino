#ifndef _DISP_H_
#define _DISP_H_

#include <Arduino.h>

#define NONE  0b00000000
#define ZERO  0b00111111
#define ONE   0b00000110
#define TWO   0b01011011
#define THREE 0b01001111
#define FOUR  0b01100110
#define FIVE  0b01101101
#define SIX   0b01111101
#define SEVEN 0b00000111
#define EIGHT 0b01111111
#define NINE  0b01101111
#define ERR   0b01111001

#define DIGIT_ENA LOW
#define DIGIT_RST HIGH
#define LED_ENA LOW
#define LED_RST HIGH

#define DRAW_INTERVAL 3

class LedDisp {

    int arr[8];
    int currentDisp[4] = {ZERO, ZERO, ZERO, ZERO};
    int currentDraw;
    int drawPins[4];
    bool m_enabled;
    unsigned long previousDraw_ms = 0;

  public:
    LedDisp();
    ~LedDisp();

    void init(int segmsLed[8], int minuteD, int minuteU, int secondD, int secondU);
    void draw(unsigned long currentMillis);
    void dispNumber(int minuteNumber, int secondNumber);
    void dispDP(int index, int value);
    void dispDP1(int value);
    void dispDP2(int value);
    void dispDP3(int value);
    void dispDP4(int value);
    bool isEnabled();
    void setEnabled(bool value);
  private:
    void enableDigits(bool ena);
    void setDisp(char value, int digit);
    char decodeNumber(int number);
};

#endif //_DISP_H_
