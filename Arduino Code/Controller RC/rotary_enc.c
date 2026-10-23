#include "rotary_enc.h"

void init_pins_enc(void){
  DDRD &=~ (1 << ENC_IN_A) | (1 << ENC_IN_A);
  PORTD |= (1 << ENC_IN_A) | (1 << ENC_IN_A);
}

void init_interrupts_enc(void){
  EICRA |= (1 << ISC11) | (1 << ISC10);
  EICRA |= (1 << ISC01) | (1 << ISC00);
  EIMSK |= (1 << INT1) | (1 << INT0);
  sei();
}

ISR(INT0_vect){
  reading = PIND & 0xC;
  if (reading == PINS_HIGH && aFlag){
    encPos--;
    bFlag = 0;
    aFlag = 0;
  }
  else if (reading == 00000100)
    bFlag = 1;
}

ISR(INT1_vect){
  reading = PIND & 0xC;
  if (reading == PINS_HIGH && bFlag){
    encPos++;
    bFlag = 0;
    aFlag = 0;
  }
  else if (reading == 00001000)
    aFlag = 1;
}