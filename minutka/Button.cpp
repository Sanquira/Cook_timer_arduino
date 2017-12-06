#include "Button.h"

ButtonEventArgs::ButtonEventArgs(void* sender) {
  m_Sender = sender;
}

void Button::registerButton(int pin, Event* event) {
  pinID = pin;
  setPressedButtonEvent(event);
  pinMode(pinID, INPUT);
}

void Button::setPressedButtonEvent(Event* event) {
  m_pressedButtonEvent = event;
}

void Button::pressed() {
  ButtonEventArgs args(this);
  m_pressedButtonEvent->invoke(&args);
}

void Button::process(unsigned long currentMillis) {
  int reading = digitalRead(pinID);

  if (reading != lastButtonState) {
    lastDebounceTime = currentMillis;
  }

  if ((currentMillis - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == BUTTON_ENA) {
        pressed();
      }
    }
  }

  lastButtonState = reading;
}

