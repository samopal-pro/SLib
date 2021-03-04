#include "SButton.h"


//******************************************************************************************************************************************
// Abstract Base Button Class
//
/**
 * Constructor
 * @param is_click - pointer on click function
 */
SButtonVirtual::SButtonVirtual(){
   State             = false;
   Long_press_state  = false;
   Auto_press_state  = false;
   isClickPtr        = NULL;
   Ms1               = 0;
   Ms2               = 0;
   Ms_auto_click     = 0;
   TM_bounce         = 50;
   TM_long_click     = 0;
   TM_auto_click     = 0;
   Period_auto_click = 500;
   TM_seq_click      = 0;  
   MultiClickCount   = 0;
   AutoClickCount    = 0;
   Time              = 0;
}

/**
 * Constructor
 * @param is_click - pointer on click function
 */
SButtonVirtual::SButtonVirtual(bool (*is_click)(void)){
   State             = false;
   Long_press_state  = false;
   Auto_press_state  = false;
   isClickPtr        = is_click;
   Ms1               = 0;
   Ms2               = 0;
   Ms_auto_click     = 0;
   TM_bounce         = 50;
   TM_long_click     = 0;
   TM_auto_click     = 0;
   Period_auto_click = 500;
   TM_seq_click      = 0;  
   MultiClickCount   = 0;
   Time              = 0;
}

bool SButtonVirtual::isClick(void){
  if( isClickPtr == NULL )return false;
  return (*isClickPtr)();
}

/**
 * This method for comatable old verions
 */
void SButtonVirtual::begin() {}

/**
 * Bounce of contacts timeout set.
 * @param tm - тBounce of contacts timeout, ms
 */
void SButtonVirtual::SetBounce( uint16_t tm ){
   TM_bounce         = tm;  
}

/**
 * Set bouns tm and click callback
* @param tm - Bounce of contacts timeout, ms
* @param func - Callback function
*/ 
void SButtonVirtual::SetClick( uint16_t bounce_tm, void (*func)(void)){
   TM_bounce         = bounce_tm;  
   callbackClick     = func;
  
}

/**
 * Long button hold time set
 * @param tm - Long button hold time, ms. ( tm = 0 - mode is off )
 */
void SButtonVirtual::SetLongClick( uint16_t tm, void (*func)(uint16_t tm) ){
   TM_long_click     = tm;
   callbackLongClick = func;
}

/**
 * Auto trigger mode set
 * @param tm1 - Auto trigger mode time, ms ( tm1 = 0 - mode is off )
 * @param tm2 - Auto trigger interval, ms
 */
void SButtonVirtual::SetAutoClick( uint16_t tm1, uint16_t tm2, void (*func)(uint16_t num) ){
   TM_auto_click     = tm1;
   Period_auto_click = tm2;
   callbackAutoClick = func;
}

/**
 * Multiple puth interval set
 * @param tm - Multiple puth interval, ms ( tm = 0 - mode is off )
 */
void SButtonVirtual::SetMultiClick( uint16_t tm, void (*func)(uint16_t num) ){
   TM_seq_click       = tm;  
   callbackMultiClick = func;
}

/**
 * This method is called in a loop or by timer
*/
SBUTTON_CLICK SButtonVirtual::Loop() {
   uint32_t ms = millis();
   bool is_click = isClick();
// Press button fixed 
   if( is_click == true && State == false && (ms-Ms1)>TM_bounce ){
       uint16_t dt = ms - Ms1;
       Long_press_state = false;
       Auto_press_state = false;
#ifdef DEBUG_SERIAL      
       Serial.print(">>>Event button press ON, tm=");
       Serial.print(dt);
       Serial.println(" ms");
#endif      
       State = true;
       if(TM_seq_click > 0 && ( dt < TM_seq_click || MultiClickCount == 0) )MultiClickCount++;
       else MultiClickCount = 0;
       Ms2    = ms;
       if( TM_long_click == 0 && TM_auto_click == 0 && MultiClickCount == 0 && TM_seq_click == 0 ){
          if( callbackClick != NULL )callbackClick();
          return SB_CLICK;
       }
   }

// Long hold button fixed 
   if( is_click == true && !Long_press_state && TM_long_click>0 && ( ms - Ms2 )>TM_long_click ){
      Long_press_state = true;
      uint16_t dt      = ms - Ms2;
#ifdef DEBUG_SERIAL      
      Serial.print(">>>Event button, pin=");
      Serial.print(Pin);
      Serial.print(",long hild, tm=");
      Serial.print(dt);
      Serial.println(" ms");
#endif 
      if( callbackLongClick != NULL )callbackLongClick(dt);
      return SB_LONG_CLICK;
   }

// Auto trigger button fixed  
   if( is_click == true && TM_auto_click > 0 
       && ( ms - Ms2 ) > TM_auto_click 
       && ( ms - Ms_auto_click ) > Period_auto_click ){
      uint16_t dt      = ms - Ms2;
      Auto_press_state = true;
      Ms_auto_click    = ms;
#ifdef DEBUG_SERIAL      
      Serial.print(">>>Event button, pin=");
      Serial.print(Pin);
      Serial.print(",auto press, tm=");
      Serial.print(dt);
      Serial.println(" ms");
#endif 
      if( callbackAutoClick != NULL )callbackAutoClick(++AutoClickCount);
      return SB_AUTO_CLICK;
   }
   else {
    if( TM_auto_click > 0 
       && ( ms - Ms2 ) > TM_auto_click 
       && ( ms - Ms_auto_click ) > Period_auto_click )AutoClickCount = 0;      
   }

   
// Release button fixed 
   if( is_click == false && State == true  && (ms-Ms2)>TM_bounce ){
       uint16_t dt      = ms - Ms2;
//       if( (ms - Ms1) < TM_bounce )return SB_NONE;
// All flags reset       
       State            = false;
#ifdef DEBUG_SERIAL      
       Serial.print(">>>Event button, pin=");
       Serial.print(Pin);
       Serial.print(",press OFF, tm=");
       Serial.print(dt);
       Serial.println(" ms");
#endif 
      Ms1    = ms;
      Time   = ms - Ms2;
 // Short click fixed
      if( Long_press_state )MultiClickCount = 0;
      if( TM_seq_click == 0 && (TM_long_click != 0 || TM_auto_click != 0 || TM_seq_click != 0 ) && !Long_press_state && !Auto_press_state ){
          if( callbackClick != NULL )callbackClick();
          return SB_CLICK;
      }
   } 
   
// Serial press mode realise fixed
   if( is_click == false && State == false  &&  TM_seq_click > 0 && ms - Ms2 > TM_seq_click ){
       if( MultiClickCount > 0 ){
         Count = MultiClickCount;
         MultiClickCount = 0;
         if( callbackMultiClick != NULL )callbackMultiClick(Count);
         return SB_MULTI_CLICK;    
     }
   }
   if( Time <20000 )Time = ms - Ms2;
// End serial press fixed
   return SB_NONE;
}     

//**************************************************************************************************************************************
// Digital Button Class (GPIO Button)
//
/**
 * Конструктор
 * @param pin - GPIO number
 * @param state - Optional. HIGH/LOW press state. LOW default.
 * Optional tm1 - tm5 (SButtonVirtual)
 */
 
SButton::SButton(uint8_t pin, bool state ): SButtonVirtual() {
  Pin = pin;
  PressState = state;
  if( PressState == LOW )pinMode( INPUT_PULLUP, Pin );
  else pinMode( INPUT, Pin );
}

/**
 * Return true - press button
 */

bool SButton::isClick(){
  bool pin_state = digitalRead( Pin );
  if( pin_state == PressState )return true;
  else return false;  
}

//**************************************************************************************************************************************
// Analog Button Class (ADC Button)
//
/**
 * Конструктор
 * @param pin - GPIO number
 * @param min <= ADC value < max - press button
 * Optional tm1 - tm5 (SButtonVirtual)
 */
/*
SAnalogButton::SAnalogButton(uint8_t pin, uint16_t min, uint16_t max): SButtonVirtual(is_click) {
  bool (*is_click)(void) = isAnalogClick; 
  Pin      = pin;
  MinValue = min;
  MaxValue = max;
}
*/
/**
 * Return true - press button
 */
/* 
bool SAnalogButton::isAnalogClick(void){
  uint16_t val = analogRead( Pin );
//  Serial.print("ADC=");
//  Serial.println(val);
  if( val >= MinValue && val < MaxValue )return true;
  else return false;  
}
*/
