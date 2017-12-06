//=================================
// include guard
#ifndef _TIMERLOGIC_H_
#define _TIMERLOGIC_H_

//=================================
// included dependencies
#include <Arduino.h>
#include "Events.h"

#define CHANGE_AMOUNT 60000
#define SECOND_MARK_PERIODE 1000

class TimerLogic {
  private:

    long desiredTime = 0;
    long currentTime = 0;
    long startMillis = 0;
    bool m_running = false;
    Event* m_timeoutEvent;

  public:
    void setTimeoutEvent(Event* event);
    int getDisplayTimeMinutes();
    int getDisplayTimeSeconds();
    int getSecondMark();
    int getSecondMarkAsync(unsigned long value);
    void process(unsigned long currentMillis);
    void startTimer();
    void stopTimer();
    void resetTimer();
    bool isRunning();
    void setDesiredTime(long value);
    void increaseTime();
    void decreaseTime();

  private:
    int getTimeInMinute(unsigned long value);
    int getTimeInSeconds(unsigned long value);
    void timeout();

};

class TimeoutEventArgs : public EventArgs {
  public:
    TimeoutEventArgs(void* sender);
};

#endif //_TIMERLOGIC_H_
