#include "LedDisp.h"
#include "Button.h"
#include "TimerLogic.h"
#include "Buzzer.h"
#include <avr/sleep.h>

void wakeUpNow();

bool m_isWakingUp;
LedDisp led;
Buzzer buzzer = Buzzer(4);
TimerLogic timer;
TimerLogic sleepTimer;
class timers {
  public:
    void timers::init() {
      timer.setTimeoutEvent(new EventHandler<timers, TimeoutEventArgs>(this, &timers::onTimeout));
      sleepTimer.setTimeoutEvent(new EventHandler<timers, TimeoutEventArgs>(this, &timers::onSleepTimeout));
      sleepTimer.setDesiredTime(10000);
    }

    void timers::onTimeout(TimeoutEventArgs* args) {
      buzzer.startBuzzer();
      sleepTimer.stopTimer();
    }

    void timers::onSleepTimeout(TimeoutEventArgs* args) {
      led.setEnabled(false);
      sleep_enable();
      attachInterrupt(digitalPinToInterrupt(2), wakeUpNow, HIGH);
      sleep_mode();
      // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
      sleep_disable();
      detachInterrupt(digitalPinToInterrupt(2));
      led.setEnabled(true);
    }

    void timers::onStupidTimeout(TimeoutEventArgs* args) {
      timer.startTimer();
      sleepTimer.stopTimer();
    }

    void timers::process(unsigned long currentMillis) {
      timer.process(currentMillis);
      sleepTimer.process(currentMillis);
    }

} timers;
class Buttons {
  public:
    Button startButton, plusButton, minusButton;

    void Buttons::init() {
      plusButton.registerButton(A0, new EventHandler<Buttons, ButtonEventArgs>(this, &Buttons::onPlusPressed));
      startButton.registerButton(A2, new EventHandler<Buttons, ButtonEventArgs>(this, &Buttons::onStartPressed));
      minusButton.registerButton(A1, new EventHandler<Buttons, ButtonEventArgs>(this, &Buttons::onMinusPressed));
    }

    void Buttons::onPlusPressed(ButtonEventArgs* args) {
      if (m_isWakingUp) {
        m_isWakingUp = false;
        sleepTimer.resetTimer();
        return;
      }
      timer.increaseTime();
      if (!timer.isRunning())
        sleepTimer.resetTimer();
    }

    void Buttons::onMinusPressed(ButtonEventArgs* args) {
      if (m_isWakingUp) {
        m_isWakingUp = false;
        sleepTimer.resetTimer();
        return;
      }
      timer.decreaseTime();
      if (!timer.isRunning())
        sleepTimer.resetTimer();
    }

    void Buttons::onStartPressed(ButtonEventArgs* args) {
      if (m_isWakingUp) {
        m_isWakingUp = false;
        sleepTimer.resetTimer();
        return;
      }
      if (buzzer.isRunning()) {
        buzzer.stopBuzzer();
        sleepTimer.resetTimer();
        return;
      }
      if (timer.isRunning()) {
        timer.stopTimer();
        sleepTimer.resetTimer();
      }
      else {
        timer.startTimer();
        sleepTimer.stopTimer();
      }
    }

    void process(unsigned long currentMillis) {
      plusButton.process(currentMillis);
      startButton.process(currentMillis);
      minusButton.process(currentMillis);
    }

} buttons;
class Buzzers {
  public:
    void Buzzers::init() {
      buzzer.registerBuzzer(A3, new EventHandler<Buzzers, BuzzerEventArgs>(this, &Buzzers::onEnded), true);
      // fur elise 20
      //      buzzer.addTone(NOTE_E6, 250);
      //      buzzer.addTone(NOTE_DS6, 250);
      //      buzzer.addTone(NOTE_E6, 250);
      //      buzzer.addTone(NOTE_DS6, 250);
      //      buzzer.addTone(NOTE_E6, 250);
      //      buzzer.addTone(NOTE_B5, 250);
      //      buzzer.addTone(NOTE_D6, 250);
      //      buzzer.addTone(NOTE_C6, 250);
      //      buzzer.addTone(NOTE_A5, 250);
      //      buzzer.addTone(0, 50);
      //      buzzer.addTone(NOTE_C5, 250);
      //      buzzer.addTone(NOTE_E5, 250);
      //      buzzer.addTone(NOTE_A5, 250);
      //      buzzer.addTone(NOTE_B5, 250);
      //      buzzer.addTone(0, 50);
      //      buzzer.addTone(NOTE_E5, 250);
      //      buzzer.addTone(NOTE_GS5, 250);
      //      buzzer.addTone(NOTE_B5, 250);
      //      buzzer.addTone(NOTE_C6, 250);
      //      buzzer.addTone(0, 50);
      // annoying 4
      buzzer.addTone(1600, 500);
      buzzer.addTone(1500, 500);
      buzzer.addTone(1400, 500);
      buzzer.addTone(1500, 500);
    }

    void Buzzers::onEnded(BuzzerEventArgs* args) {
      sleepTimer.resetTimer();
      led.dispDP2(1);
    }
} buzzers;

void setup() {
  int arr[] = {3, 4, 5, 6, 7, 8, 9, 10};
  led.init(arr, 11, 12, A5, A4);
  buttons.init();
  timers.init();
  buzzers.init();

  led.dispDP2(1);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  m_isWakingUp = false;
  sleepTimer.startTimer();
}

void loop() {
  unsigned long currentMillis = millis();
  timers.process(currentMillis);
  led.dispNumber(timer.getDisplayTimeMinutes(), timer.getDisplayTimeSeconds());
  led.dispDP2(timer.getSecondMark());
  led.draw(currentMillis);
  buttons.process(currentMillis);
  buzzer.process(currentMillis);
}

void wakeUpNow()
{
  m_isWakingUp = true;
  // execute code here after wake-up before returning to the loop() function
  // timers and code using timers (serial.print and more...) will not work here.
}

