#include <avr/io.h>
#include <util/delay.h>

#define DEBOUNCE_TIME 50
#define TIMER_OCRA    62      // Rounded up so we dont need to use float math. This is 0.0000000625 * 10^9
#define BT_UP      PD2
#define BT_DOWN    PD3
#define BT_LFT     PD4
#define BT_RGT     PD5
#define BT_MID     PD6
#define BT_SET     PD7
#define BT_RST     PB0      // Uses PORTB

uint8_t _DebugMode = 0; 
uint16_t milliseconds_passed = 0;
uint16_t seconds_passed = 0;
uint16_t minutes_passed = 0;
uint8_t check_time = 0;

uint8_t all_button_defines[7] = {BT_UP, BT_DOWN, 
                                 BT_LFT, BT_RGT, 
                                 BT_MID, BT_SET, BT_RST};

typedef struct button_track{
  uint8_t button_pin;
  uint8_t button_state;
  uint8_t last_button_state;
  unsigned long last_debounce_time;
  uint8_t button_action_mode;             // if 1 == action, 0 == no action;
} Button;

typedef struct _node{
  Button* button_node;
  struct _node *next;
}Node;

typedef struct _ll_circular{
  Node* head;
  Node* tail;
}LinkedList;

void init_clock(void);
void init_buttons(void);
void time_to_check(void);
void read_button(Button* btn);
void print_button_action(LinkedList *ll);
uint8_t add_to_list(void);
uint8_t init_list(LinkedList* ll);
void init_buttons(void);