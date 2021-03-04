#ifndef SButton_h
#define SButton_h
#include "Arduino.h"

//#define DEBUG_SERIAL 1

typedef enum {
   SB_NONE = 0,
   SB_CLICK,
   SB_AUTO_CLICK,
   SB_LONG_CLICK,
   SB_MULTI_CLICK,
}SBUTTON_CLICK;

//
// Abstract Base Button Class
//
class SButtonVirtual {
  private :
     bool     State;
     bool     Long_press_state;
     bool     Auto_press_state;
     uint32_t Ms1;
     uint32_t Ms2;
     uint32_t Ms_auto_click;
     uint16_t TM_bounce;
     uint16_t TM_long_click;
     uint16_t TM_auto_click;
     uint16_t TM_seq_click;
     uint16_t Period_auto_click;
     uint16_t MultiClickCount;
     uint16_t AutoClickCount;
     void (*callbackClick)(void) = NULL;
     void (*callbackLongClick)(uint16_t tm) = NULL;
     void (*callbackAutoClick)(uint16_t num) = NULL;
     void (*callbackMultiClick)(uint16_t num) = NULL;

  public :
     SButtonVirtual();
     SButtonVirtual(bool (*is_click)(void));
     void begin();
     void SetBounce( uint16_t tm );
     void SetClick( uint16_t bounce_tm, void (*func)(void) = NULL );
     void SetLongClick( uint16_t tm,    void (*func)(uint16_t tm) = NULL );
     void SetAutoClick( uint16_t tm1, uint16_t tm2, void (*func)(uint16_t num) = NULL );
     void SetMultiClick( uint16_t tm,   void (*func)(uint16_t num) = NULL );
     bool (*isClickPtr)(void);
     virtual bool isClick(void);
     SBUTTON_CLICK Loop();
     uint16_t Count;
     uint16_t Time;
};     

//
// Digital Button Class.
//
class SButton : public SButtonVirtual {
  private:
  public:
     uint8_t Pin;
     bool PressState;
     bool isClick();
     SButton(uint8_t pin, bool state = LOW);  
};
/*
//
// Analog Button Class.
//
class SAnalogButton : public SButtonVirtual {
  private:
     uint8_t Pin;
     uint16_t MinValue;
     uint16_t MaxValue;
     bool PressState;
  public:
     bool isAnalogClick();
     SAnalogButton(uint8_t pin, uint16_t min, uint16_t max);  
};
*/
#endif
