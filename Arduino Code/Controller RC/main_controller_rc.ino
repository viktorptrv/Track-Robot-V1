#include <avr/io.h>
#include <util/delay.h>
#include "rotary_enc.h"

typedef struct _msg{

}ControllerInfo;

int main(void){
  // main init
  init_clock();
  init_pins_enc();
  init_buttons();

  int reading;
  LinkedList* LL = (LinkedList*) malloc(sizeof(LinkedList));
  if (LL == NULL){
    Serial.println("Cannot allocate memory for linked list");
    return 0;
  }
  else{
    LL->head = NULL;
    LL->tail = NULL;
  }
  
  result = init_list(LL);
  if (result & 1){
    Serial.println("Could not init the list");
    return 0;
  }

  Node* curNode = LL->head;
  if (curNode == NULL){
    Serial.println("Error!");
    return 0;
  }

  while(1){

  }
}