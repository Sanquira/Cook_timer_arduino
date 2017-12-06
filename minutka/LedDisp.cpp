#include "LedDisp.h"

LedDisp::LedDisp() {}

LedDisp::~LedDisp() {
  delete [] arr;
}

void LedDisp::init(int segmsLed[8], int minuteD, int minuteU, int secondD, int secondU) {
  for (int i = 0; i < 8; i++) {
    arr[i] = segmsLed[i];
  }
  drawPins[0] = minuteD;
  drawPins[1] = minuteU;
  drawPins[2] = secondD;
  drawPins[3] = secondU;

  currentDraw = 0;
  for (int i = 0; i < 8; i++) {
    pinMode(arr[i], OUTPUT);
    pinMode(arr[i], LED_RST);
  }
  for (int i = 0; i < 4; i++) {
    pinMode(drawPins[i], OUTPUT);
  }
  enableDigits(false);
  setEnabled(true);
}

void LedDisp::draw(unsigned long currentMillis) {
  if (!m_enabled)return;
  if (currentMillis - previousDraw_ms >= DRAW_INTERVAL) {
    previousDraw_ms = currentMillis;

    setDisp(currentDisp[currentDraw], drawPins[currentDraw]);
    currentDraw = (currentDraw + 1) % 4;
  }
}

void LedDisp::dispDP(int index, int value) {
  if (index < 0 || index >= 4) return;
  currentDisp[index] = currentDisp[index] ^ ((-value ^ currentDisp[index]) & (1 << 7));
}

void LedDisp::dispDP1(int value) {
  currentDisp[0] = currentDisp[0] ^ ((-value ^ currentDisp[0]) & (1 << 7));
}

void LedDisp::dispDP2(int value) {
  currentDisp[1] = currentDisp[1] ^ ((-value ^ currentDisp[1]) & (1 << 7));
}

void LedDisp::dispDP3(int value) {
  currentDisp[2] = currentDisp[2] ^ ((-value ^ currentDisp[2]) & (1 << 7));
}

void LedDisp::dispDP4(int value) {
  currentDisp[3] = currentDisp[3] ^ ((-value ^ currentDisp[3]) & (1 << 7));
}

void LedDisp::dispNumber(int minuteNumber, int secondNumber) {
  //minutes
  if (minuteNumber > 99)
  {
    currentDisp[0] = ERR;
    currentDisp[1] = ERR;
//  } else  if (minuteNumber == 0) {
//    currentDisp[0] = NONE;
//    currentDisp[1] = NONE;
  } else  if (minuteNumber < 10) {
    currentDisp[0] = NONE;
    currentDisp[1] = decodeNumber(minuteNumber);
  }  else {
    int decimal = minuteNumber / 10;
    int unimal = minuteNumber - (decimal * 10);
    currentDisp[0] = decodeNumber(decimal);
    currentDisp[1] = decodeNumber(unimal);
  }

  //seconds
  if (secondNumber > 99)
  {
    currentDisp[2] = ERR;
    currentDisp[3] = ERR;
  } else {
    int decimalS = secondNumber / 10;
    int unimalS = secondNumber - (decimalS * 10);
    currentDisp[2] = decodeNumber(decimalS);
    currentDisp[3] = decodeNumber(unimalS);
  }
}

void LedDisp::setDisp(char value, int digit) {
  char mask = 0x00000001;
  enableDigits(false);
  for (int i = 0; i < 8; i++) {
    if ((mask & (value >> i)) == 1)
      digitalWrite(arr[i], LED_ENA);
    else
      digitalWrite(arr[i], LED_RST);
  }
  digitalWrite(digit, DIGIT_ENA);
}

void LedDisp::enableDigits(bool ena) {
  if (ena) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(drawPins[i], DIGIT_ENA);
    }
  } else {
    for (int i = 0; i < 4; i++) {
      digitalWrite(drawPins[i], DIGIT_RST);
    }
  }
}

char LedDisp::decodeNumber(int number) {
  switch (number) {
    case 0:
      return ZERO;
    case 1:
      return ONE;
    case 2:
      return TWO;
    case 3:
      return THREE;
    case 4:
      return FOUR;
    case 5:
      return FIVE;
    case 6:
      return SIX;
    case 7:
      return SEVEN;
    case 8:
      return EIGHT;
    case 9:
      return NINE;
    default:
      return NONE;
  }
}

bool LedDisp::isEnabled() {
  return m_enabled;
}
void LedDisp::setEnabled(bool value) {
  m_enabled = value;
  enableDigits(m_enabled);
}

