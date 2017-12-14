#include "TimerLogic.h"

TimeoutEventArgs::TimeoutEventArgs(void* sender) {
  m_Sender = sender;
}

void TimerLogic::setTimeoutEvent(Event* event) {
  delete m_timeoutEvent;
  m_timeoutEvent = event;
}

void TimerLogic::timeout() {
  TimeoutEventArgs args(this);
  m_timeoutEvent->invoke(&args);
}

int TimerLogic::getTimeInMinute(unsigned long value) {
  return value / 60000;
}

int TimerLogic::getTimeInSeconds(unsigned long value) {
  return (value / 1000) % 60;
}

int TimerLogic::getSecondMark() {
  return currentTime % SECOND_MARK_PERIODE > SECOND_MARK_PERIODE / 2 ? 1 : 0;
}

int TimerLogic::getSecondMarkAsync(unsigned long value) {
  return value % SECOND_MARK_PERIODE > SECOND_MARK_PERIODE / 2 ? 1 : 0;
}

int TimerLogic::getDisplayTimeMinutes() {
  return getTimeInMinute(currentTime);
}

int TimerLogic::getDisplayTimeSeconds() {
  return getTimeInSeconds(currentTime);
}

void TimerLogic::process(unsigned long currentMillis) {
  if (m_running) {
    currentTime = desiredTime - currentMillis + startMillis;
  } else {
    currentTime = desiredTime;
  }
  if (currentTime <= 0 && m_running) {
    currentTime = 0;
    m_running = false;
    timeout();
  }
}

void TimerLogic::startTimer() {
  startMillis = millis();
  m_running = true;
}

void TimerLogic::stopTimer() {
  m_running = false;
  currentTime = desiredTime;
}

void TimerLogic::resetTimer() {
  stopTimer();
  startTimer();
}

bool TimerLogic::isRunning() {
  return m_running;
}

void TimerLogic::setDesiredTime(long value) {
  desiredTime = value;
  if (!m_running)
    currentTime = value;
}

void TimerLogic::increaseTime() {
  desiredTime += CHANGE_AMOUNT;
  if (m_running)
    currentTime += CHANGE_AMOUNT;
}

void TimerLogic::decreaseTime() {
  desiredTime -= CHANGE_AMOUNT;
  if (desiredTime < 0)desiredTime = 0;
  if (m_running) {
    currentTime -= CHANGE_AMOUNT;
    if (currentTime < 0)currentTime = 0;
  }
}

