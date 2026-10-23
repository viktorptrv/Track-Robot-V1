#include <avr/io.h>
#include <util/delay.h>

#define ENC_IN_A    PD2       // INT0
#define ENC_IN_B    PD3       // INT1
#define MAX_SPD     100       // this is in %
#define MIN_SPD     0         // this is in %
#define PINS_HIGH   00001100 
#define EXP_A       00000100
#define EXP_B       00001000

int8_t speed = MIN_SPD;
volatile int8_t aFlag = 0;
volatile int8_t bFlag = 0;
volatile int8_t encPos = 0;
volatile int8_t oldEncPos = 0;
volatile int8_t reading = 0;

void init_pins_enc(void);
void init_interrupts_enc(void);
