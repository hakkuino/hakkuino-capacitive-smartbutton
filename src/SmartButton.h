#ifndef HAKKUINO_SMART_BUTTON_H
#define HAKKUINO_SMART_BUTTON_H

#include <CapacitiveSensor.h>
#include "Arduino.h"

class SmartButton
{
private:
  void (*_clickCallback)(byte);
  void (*_longPressCallback)(byte, unsigned int);
  void (*_pressedCallback)();
  void (*_releaseCallback)();
  byte state;
  byte previousState;
  byte clickCount;
  long timer;
  CapacitiveSensor* cs;
  
  byte buttonState;         // current state of the button
  byte lastButtonState;   


public:
  ~SmartButton();
  SmartButton(short from, short to);
  void setClickCallback(void (*clickCallback)(byte));
  void setLongPressCallback(void (*longPressCallback)(byte,unsigned int));
  void setPressedCallback(void (*pressedCallback)());
  void setReleasedCallback(void (*releaseCallback)());
  void tick();
};
#endif