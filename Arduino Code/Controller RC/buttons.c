#include "buttons.h"

void init_clock(void){
  TCCR0A |= (1 << WGM01);  // Enabling CTC mode
  OCR0A = 249;                            // Top value is 255 since its 8bit reg
  TIMSK0 |= (1 << OCIE0A);
  TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler 64
  sei();
}

ISR(TIMER0_COMPA_vect){
  milliseconds_passed++;
  check_time = 1;
}

void time_to_check(void){
  if (milliseconds_passed == 1000){
    seconds_passed++;
    if (seconds_passed == 60){
      minutes_passed++;
      seconds_passed = 0;
    }
    milliseconds_passed = 0;
  }
}

void init_buttons(void){
  // Configure buttons as inputs
  DDRB &= ~(1 << BT_RST);
  DDRB |= (1 << PB4);

  DDRD &= ~((1 << BT_UP)
          | (1 << BT_DOWN)
          | (1 << BT_LFT)
          | (1 << BT_RGT)
          | (1 << BT_MID)
          | (1 << BT_SET));

  PORTB |= (1 << BT_RST);
  PORTD |= (1 << BT_UP) 
          | (1 << BT_DOWN) 
          | (1 << BT_LFT) 
          | (1 << BT_RGT) 
          | (1 << BT_MID) 
          | (1 << BT_SET);
}

uint8_t add_to_list(LinkedList* ll, uint8_t btn){
  Node* curNode = (Node*)malloc(sizeof(Node));
  if (curNode == NULL){
    return 1;
  }

  // Create structure;
  Button* curBtn = (Button*)malloc(sizeof(Button));
  if (curBtn == NULL){
    return 1;
  }
  
  curBtn ->button_pin = btn;
  curBtn ->button_state = 1;
  curBtn ->last_button_state = 1;
  curBtn ->last_debounce_time = 0;
  curBtn ->button_action_mode = 0;

  // Add the structure to the node
  curNode->button_node = curBtn;

  if (ll->head == NULL){
    ll->head = curNode;
  }
  else{
    ll->tail->next = curNode;
  }
  ll->tail = curNode;
  curNode->next = NULL;

  return 0;
}

uint8_t init_list(LinkedList* ll){
  uint8_t result;
  for(int i = 0; i < 7; i++){
    result = add_to_list(ll,  all_button_defines[i]);
    if (result & 1){
      return 1;
    }
  }
  return 0;
}

void read_button(Button* btn){
  uint8_t reading = 0;
  if (_DebugMode & 1){
    Serial.print("Current btn -> ");
    Serial.println(btn->button_pin);
  }
  
  while (reading == 0){
    if (btn->button_pin == PB0){
    reading = PINB & (1 << btn->button_pin) ? 1:0;
    }
    else{
      reading = PIND & (1 << btn->button_pin) ? 1:0;
    }
    _delay_ms(10);
    if (_DebugMode & 1){
      Serial.print("Current Btn State -> ");
      Serial.println(reading);
      Serial.print("Last Btn State -> ");
      Serial.println(btn->last_button_state);
    }

    if (reading != btn->last_button_state) {
      btn->last_debounce_time = milliseconds_passed; // Record the time of this change
      if (_DebugMode & 1){
        Serial.print("last_debounce_time = ");
        Serial.println(btn->last_debounce_time);
      }
    }
    if ((milliseconds_passed - btn->last_debounce_time) > DEBOUNCE_TIME) {
    // Debounce time has passed, check if the state has stabilized
      if(_DebugMode & 1){
        Serial.println("Passed debounced time");
      }
      if (reading != btn->button_state) {
        if(_DebugMode & 1){
          Serial.println("Reading is different than state");
        }
        btn->button_state = reading;
        if (reading == 0) {
            // Button press is confirmed, take appropriate action
            btn->button_action_mode = 1;
            if (_DebugMode & 1){
              Serial.print("button_action_mode = ");
              Serial.println(btn->button_action_mode);
               //_delay_ms(2000);
            }
        }
        else{
          btn->button_action_mode = 0;
        }
      }
    }
    btn->last_button_state = reading;
  }
}

void print_button_action(LinkedList *ll){
  Node* curNode = ll->head;
  if (ll->head == NULL){
    Serial.println("No list!");
    return;
  }

  while(curNode->next != NULL){
    Serial.print("Btn -> ");
    Serial.print(curNode->button_node->button_pin);
    Serial.print("    State Action -> ");
    Serial.println(curNode->button_node->button_action_mode);
    curNode = curNode->next;
  }
}


