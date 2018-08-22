#include "radio.h"
#include <avr/sleep.h>
#include <avr/interrupt.h>

void sleepNow();

#define MAX_IDLE 1000

int idleReset = 0;

void setupSleep() {
  //Power down various bits of hardware to lower power usage
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); //Power down everything, wake up from WDT
  sleep_enable();
  ADCSRA &= ~(1 << ADEN); //Disable ADC, saves ~230uA
  ACSR = (1 << ACD); //Turn off Analog Comparator
}

//void loop() {
// watchdogSleep1s();

// do stuff with sleepTime
//}

void idle() {
  if (idleReset < 1) {
    sleepNow();
  } else {
    idleReset--;
  }
}

void resetIdle() {
  idleReset = MAX_IDLE;
}

void sleepNow() {
  _radio.powerDown();

  ADCSRA &= ~(1 << ADEN); //Disable ADC, saves ~230uA
  // Configure all ports as output
  // (does not seem to make any difference) //neet to setup all again
  DDRB = 0b0011111;
  PORTB = 0;

  ACSR = (1 << ACD); //Turn off Analog Comparator

  GIMSK |= _BV(PCIE);                     // Enable Pin Change Interrupts
  PCMSK |= _BV(PCINT2);                   // Use PB2 as interrupt pin
  PCMSK |= _BV(PCINT1);                   // Use PB2 as interrupt pin
  PCMSK |= _BV(PCINT0);                   // Use PB2 as interrupt pin

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // replaces above statement

  sleep_enable();
  sei();                                  // Enable interrupts
  sleep_cpu();

  cli();                                  // Disable interrupts
  PCMSK &= ~_BV(PCINT0);                  // Turn off PB3 as interrupt pin
  PCMSK &= ~_BV(PCINT1);                  // Turn off PB3 as interrupt pin
  PCMSK &= ~_BV(PCINT2);                  // Turn off PB3 as interrupt pin
  sleep_disable();                        // Clear SE bit

  sei();
  //ADCSRA |= (1<<ADEN); //Enable ADC



}

//This runs each time the watch dog wakes us up from sleep
ISR(PCINT0_vect) {
}
//This runs each time the watch dog wakes us up from sleep
ISR(PCINT1_vect) {
}
//This runs each time the watch dog wakes us up from sleep
ISR(PCINT2_vect) {
}

