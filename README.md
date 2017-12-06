# Cook_timer_arduino

Project of Cooking timer based on Arduino Nano ATmega328.

Project is designed with focus on modularity. Therefore there are few independent modules:

Event       - Simple event template. Used for firing events like button pressed, buzzer melody ended (or was ended),
              timer timed out,
Button      - Module which allows you to register button on pin with debounce feature and event response in program.
Buzzer      - Module for sound output with support of melody and/or cycle play.
TimerLogic  - Module for time measure designed on minutes scale. Fire event when timer ends.
              Inner timer count in millis. There is support for "live" add/remove time.
              It will return you remaining time in minutes and in seconds and second ticking indicator.
              Timer reset itself when stopped.
LedDisp     - Module for 4 digits 7-segments LED display. It uses multiplex technology so it needs 8 + 4 pins when registering.
