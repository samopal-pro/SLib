/**
 * SButton example
 * GPIO digital button vcc pullup
 */
#include "SButton.h"

#define PIN_BUTTON  3

SButton my_button( PIN_BUTTON );

void onClick(void);
void onLongClick(uint16_t t);
void onAutoClick(uint16_t n);
void onMultiClick(uint16_t n);

void setup(){
   Serial.begin(115200);
   Serial.println("SButton example start ...");   
   my_button.SetClick(200, onClick);
///   my_button.SetLongClick(2000, onLongClick);
// Click and Multiple Click is alternative   
//   my_button.SetMultiClick(500, onMultiClick);
   my_button.SetAutoClick(5000,500,onAutoClick);
}
 
void loop(){
  my_button.Loop(); 
}

void onClick(void){
  Serial.println("On Click");
}

void onLongClick(uint16_t t){
  Serial.print("On Long Click:");
  Serial.println(t);
}

void onAutoClick(uint16_t n){
  Serial.print("On Auto Click:");
  Serial.println(n);
}

void onMultiClick(uint16_t n){
  Serial.print("On Multi Click:");
  Serial.println(n);
  
}
