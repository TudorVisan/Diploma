#include "message_control.h"

/* $Id$ */
/**
 * Use of the radio functions
 */

#define REG(name, reg) do{uint8_t _b_ = reg; Serial.print(name);Serial.print(" : "); Serial.println(_b_,HEX);}while(0)

unsigned long tx_time;
int cnt = 0;

frame_t g_frame,g_old_frame;

uint32_t wrong_nr = 0, total_lost, total_sent;

void setup() {

    /* operating on channel 17, not receiving when idle. */
    Radio.begin();
    Serial.begin(9600);
    Serial.println("HelloRadio V$Release$");
    wrong_nr = 0;
    total_sent = 1; 
    total_lost = 1; 
}
void loop() {

    //if (millis() > tx_time){
    //    tx_time = millis() + 25;
        // write string
        //Serial.print("Radio Available ");
        
        int available = Radio.available();
        if(available >0)
        {
          if(available != sizeof(frame_t))
          {
              for(int i = 0;i < available; i++)
              Radio.read();
              wrong_nr ++;
              //Serial.println("wrong packet");
              return;
          }
        
           // write integer
          
//           Serial.print(" data available ");
//           Serial.print(wrong_nr);
           uint32_t val;
           uint8_t* aux = (uint8_t *)(&g_frame);
           for(int i = 0;i < available; i++)
           {
             aux[i] = Radio.read();
           }
            if(g_old_frame.frame_index + 1 != g_frame.frame_index)
            {
                
//                Serial.print(" ");
//                Serial.print(g_frame.frame_index - g_old_frame.frame_index);
//                Serial.print(" ");
//                Serial.print(g_frame.frame_index); 
//                Serial.println();
                  if(g_old_frame.frame_index != 0)
                  {
                        total_lost +=  g_frame.frame_index -1 -  g_old_frame.frame_index;
                  }
            }
            if(g_old_frame.frame_index != 0)
            {
              total_sent += g_frame.frame_index -  g_old_frame.frame_index;
            }              
            g_old_frame = g_frame;
            Serial.print(wrong_nr);
           
           Serial.print(" ");
           Serial.print(total_lost);
           Serial.print(" ");
           Serial.print(total_sent);
           Serial.print(" ");
           Serial.print(wrong_nr *100.0/total_sent);
//           Serial.print(" ");
//           Serial.print(g_frame.nr);
           Serial.println();
           wrong_nr = 0;
      }
}


