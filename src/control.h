    #ifndef CONTROL_H
    #define CONTROL_H
    #include <Arduino.h>
    
    class Control {
      
      private:
        //byte pin;
        
        
      public:
        Control();
        Control(int id, int pin, String command, bool toggle);
        void init();
        void wasPressed();
        void sendCommand();

    };
    #endif