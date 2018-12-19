#include "SmartButton.h"
#include <Logger.h>

SmartButton::~SmartButton()
{
  delete this->cs;
}

SmartButton::SmartButton(short from, short to)
{
  this->cs = new CapacitiveSensor(from,to);
  this->clickCount = 0;
  this->buttonState = 0;
  this->lastButtonState = 0;
}
 
void SmartButton::setClickCallback(void (*clickCallback)(byte))
{
  _clickCallback = clickCallback;
}
void SmartButton::setLongPressCallback(void (*longPressCallback)(byte,unsigned int))
{
  _longPressCallback = longPressCallback;
}

void SmartButton::setPressedCallback(void (*pressedCallback)())
{
  _pressedCallback = pressedCallback;
}
void SmartButton::setReleasedCallback(void (*releaseCallback)())
{
  _releaseCallback = releaseCallback;
}

void SmartButton::tick()
{
  boolean pressed = false;
  boolean released = false;
  
  // Events calculations
  if(this->cs->capacitiveSensor(30)>500){
    this->buttonState = HIGH;
  }
  else{
    this->buttonState = LOW;
  }

  if (this->buttonState != this->lastButtonState) {
    if (this->buttonState == HIGH) {
      pressed = true;
      DEBUG("[SmartButton] pressed");
      if (this->_pressedCallback)
      {
        this->_pressedCallback();
      }
    } else {
      released = true;
      DEBUG("[SmartButton] released");
      if (this->_releaseCallback)
      {
        this->_releaseCallback();
      }
    }
    this->lastButtonState = this->buttonState;
  }

  byte currentState = this->state;
  boolean stateChanged = this->previousState != this->state;
  
  if (stateChanged){
    TRACE("[SmartButton] State changed.");
  }
  
  TRACE("[SmartButton] State : " + String(this->state) + " - clicks : " + String(this->clickCount));

  switch(this->state)
  {
    case 0:
      this->clickCount = 0;
      if (pressed)
      {
        this->previousState = this->state;
        this->state = 1;
      }
      break;
    case 1:
      if (stateChanged)
      {
        this->clickCount++;
        this->timer=millis();
      }
      if (released && (millis() - this->timer>10))
      {
        this->state = 2;
      }
      if (millis() - this->timer>500){
        this->state = 3;
      }
      break;
    case 2:
      if (stateChanged)
      {
        this->timer=millis();
      }
      if (pressed)
      {
        this->state = 1;
      }
      if (millis() - this->timer>300){
        this->state = 4;
      }      
      break;
    case 3:
      if (stateChanged)
      {
        this->timer=millis();
      }
      if (released)
      {
        this->state = 0;
      }
      if(this->_longPressCallback)
      {
        this->_longPressCallback(this->clickCount, millis() - this->timer);
      }
      break;
    case 4:
      if(this->_clickCallback)
      {
        this->_clickCallback(this->clickCount);
      }
      this->state=0;
      break;
    default:
      
      break;
  }
  
  this->previousState = currentState;
}