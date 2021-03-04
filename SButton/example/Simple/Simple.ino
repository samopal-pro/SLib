/**
 * SButton example
 * GPIO digital button vcc pullup
 */
#include "SButton.h"

#define PIN_BUTTON  3

SButton my_button( PIN_BUTTON );

void setup(){
   Serial.begin(115200);
   Serial.println("SButton example start ...");   
   my_button.SetLongClick(2000);
// Click and Multiple Click is alternative   
   my_button.SetMultiClick(500);
   my_button.SetAutoClick(5000,500);
}
 
void loop(){
  switch(my_button.Loop()){
    case SB_CLICK:
       Serial.println("On Click");
       break;
    case SB_LONG_CLICK:   
       Serial.println("On Long Click");
       break;
    case SB_AUTO_CLICK:   
       Serial.println("On Auto Click");
       break;
    case SB_MULTI_CLICK:   
       Serial.print("On Multi Click: ");
       Serial.println(my_button.Count);
       break;
  }
}
