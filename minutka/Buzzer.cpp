#include "Buzzer.h"

Buzzer::Buzzer(int melodyLength = 1) {
  m_melody = new Tone[melodyLength];
  m_melodyLength = melodyLength;
  m_melodyIdxFill = 0;
  m_melodyIdxCurr = 0;
}

Buzzer::~Buzzer() {
  delete [] m_melody;
}

BuzzerEventArgs::BuzzerEventArgs(void* sender, int source) {
  m_Sender = sender;
  m_source = source;
}

BuzzerEventArgs::getSource() {
  return m_source;
}

void Buzzer::setEndEvent(Event* event) {
  m_event = event;
}

void Buzzer::ended(int source) {
  if (m_event == NULL)
    return;
  BuzzerEventArgs args(this, source);
  m_event->invoke(&args);
}

void Buzzer::registerBuzzer(int pin, Event* event, bool cycle = false) {
  pinMode(pin, OUTPUT);
  m_buzzerPin = pin;
  m_cycle = cycle;
  setEndEvent(event);
}

bool Buzzer::addTone(int frequency, int duration) {
  if (m_melodyIdxFill >= m_melodyLength) {
    return false;
  }
  m_melody[m_melodyIdxFill] = Tone(frequency, duration);
  m_melodyIdxFill++;
  return true;
}

void Buzzer::startBuzzer() {
  m_running = true;
  m_melodyIdxCurr = 0;
  m_toneChanged = true;
}

void Buzzer::stopBuzzer() {
  m_running = false;
  noTone(m_buzzerPin);
  ended(1);
}

bool Buzzer::isRunning() {
  return m_running;
}

void Buzzer::process(unsigned long currentTime) {
  if (!m_running) {
    return;
  }

  int dur = m_melody[m_melodyIdxCurr].duration;

  if (m_toneChanged) {
    int freq = m_melody[m_melodyIdxCurr].frequency;
    if (freq != 0)
      tone(m_buzzerPin, freq, dur);
    m_toneChanged = false;
    startMillis = currentTime;
  }

  long passed = currentTime - startMillis;

  if (passed > dur) {
    m_melodyIdxCurr++;
    startMillis = currentTime;
    m_toneChanged = true;
  }

  if ( m_melodyIdxCurr >= m_melodyIdxFill) {
    if (m_cycle) {
      m_melodyIdxCurr = 0;
    } else {
      m_running = false;
      noTone(m_buzzerPin);
      ended(0);
    }
  }
}



