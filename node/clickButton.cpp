#include "clickButton.h"

ClickButton::ClickButton(uint8_t buttonPin)
{
  _quickState    = 0;
  _pin           = buttonPin;
  _activeHigh    = LOW;           // Assume active-low button
  btnState      = !_activeHigh;  // initial button state in active-high logic
  _lastState     = btnState;
  _clickCount    = 0;
  clicks         = 0;
  longPress      = 0;
  quickPressed   = 0;         //for quick simgle press detection
  quickDebounceTime  = qDT;
  depressed      = false;
  _lastBounceTime = 0;
  debounceTime   = dT;            // Debounce timer in ms
  multiclickTime = mcT;           // Time limit for multi clicks
  longClickTime  = lcT;          // time until long clicks register
  working = 0;
  pinMode(_pin, INPUT);
}


ClickButton::ClickButton(uint8_t buttonPin, boolean activeType)
{
  _quickState    = 0;
  _pin           = buttonPin;
  _activeHigh    = activeType;
  btnState      = !_activeHigh;  // initial button state in active-high logic
  _lastState     = btnState;
  _clickCount    = 0;
  clicks         = 0;
  quickPressed   = 0;         //for quick simgle press detection
  quickDebounceTime  = qDT;
  longPress      = 0;
  depressed      = 0;
  _lastBounceTime = 0;
  debounceTime   = dT;            // Debounce timer in ms
  multiclickTime = mcT;           // Time limit for multi clicks
  longClickTime  = lcT;          // time until long clicks register
  working = 0;
  pinMode(_pin, INPUT);
}

ClickButton::ClickButton(uint8_t buttonPin, boolean activeType, boolean internalPullup)
{
  _quickState    = 0;
  _pin           = buttonPin;
  _activeHigh    = activeType;
  btnState      = !_activeHigh;  // initial button state in active-high logic
  _lastState     = btnState;
  _clickCount    = 0;
  clicks         = 0;
  longPress      = 0;
  quickPressed   = 0;         //for quick simgle press detection
  quickDebounceTime  = qDT;
  depressed      = 0;
  _lastBounceTime = 0;
  debounceTime   = dT;            // Debounce timer in ms
  multiclickTime = mcT;           // Time limit for multi clicks
  longClickTime  = lcT;          // time until "long" click register
  working = 0;
  // Turn on internal pullup resistor if applicable
  if (_activeHigh == LOW && internalPullup == CLICKBTN_PULLUP)
    pinMode(_pin, INPUT_PULLUP);
  else
    pinMode(_pin, INPUT);
}



void ClickButton::Update()
{
  working = 0;
  long now = (long)millis();      // get current time
  btnState = digitalRead(_pin);  // current appearant button state

  // Make the button logic active-high in code
  if (!_activeHigh) btnState = !btnState;

  // If the switch changed, due to noise or a button press, reset the debounce timer
  if (btnState != _lastState) {
    _lastBounceTime = now;
    working = 1;
  }


  // debounce the button (Check if a stable, changed state has occured)
  if (now - _lastBounceTime > debounceTime && btnState != depressed)
  {
    depressed = btnState;
    if (depressed) _clickCount++;
    working = 1;
    longPress = 0;
  }

  quickPressed = 0;
  if ((now - _lastBounceTime) > quickDebounceTime) {
    if (btnState != _quickState) {
      _quickState = btnState;

      if (_quickState != _activeHigh) {
        quickPressed = 1;
        working = 1;
      }
    }
  }

  // If the button released state is stable, report nr of clicks and start new cycle
  if (!depressed && (now - _lastBounceTime) > multiclickTime)
  {
    // positive count for released buttons
    clicks = _clickCount;
    _clickCount = 0;
  }

  // Check for "long click"
  if (depressed && (now - _lastBounceTime > longClickTime))
  {
    // negative count for long clicks
    if (clicks < 0) {
      longPress = clicks;
    }
    clicks = 0 - _clickCount;
    working = 1;
    _clickCount = 0;
  }


  _lastState = btnState;
}

