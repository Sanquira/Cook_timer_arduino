//=================================
// include guard
#ifndef _BUTN_H_
#define _BUTN_H_

//=================================
// included dependencies
#include <Arduino.h>
#include "Events.h"

#define DEBOUNCE_DELAY 50

#define BUTTON_ENA HIGH

class Button {
  private:
    int pinID;
    int buttonState;
    int lastButtonState = LOW;
    long lastDebounceTime = 0;
    Event* m_pressedButtonEvent;

  public:
    void registerButton(int pin, Event* event);
    void process(unsigned long currentMillis);
  private:
    void setPressedButtonEvent(Event* event);
    void pressed();
};

class ButtonEventArgs : public EventArgs {
  public:
    ButtonEventArgs(void* sender);
};

#endif //_BUTN_H_
