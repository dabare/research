#include <Arduino.h>



#define CLICKBTN_PULLUP HIGH
#define qDT 50
#define dT 20
#define mcT 250
#define lcT 500

class ClickButton
{
  public:
    ClickButton(uint8_t buttonPin);
    ClickButton(uint8_t buttonPin, boolean active);
    ClickButton(uint8_t buttonPin, boolean active, boolean internalPullup);
    void Update();
    int clicks;                   // button click counts to return
    int longPress;                //long pressing click count
    boolean depressed;            // the currently debounced button (press) state (presumably it is not sad :)
    boolean quickPressed;         //for quick simgle press detection
    long quickDebounceTime;
    long debounceTime;
    long multiclickTime;
    long longClickTime;
    boolean btnState;            // Current appearant button state
    byte working;
  private:
    boolean _quickState;
    uint8_t _pin;                 // Arduino pin connected to the button
    boolean _activeHigh;          // Type of button: Active-low = 0 or active-high = 1
    boolean _lastState;           // previous button reading
    int _clickCount;              // Number of button clicks within multiclickTime milliseconds
    long _lastBounceTime;         // the last time the button input pin was toggled, due to noise or a press
};
