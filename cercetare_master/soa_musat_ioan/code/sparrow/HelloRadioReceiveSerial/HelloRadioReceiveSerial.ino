#include "message_control.h"

/* $Id$ */
/**
 * Use of the radio functions
 */

#define REG(name, reg) do{uint8_t _b_ = reg; Serial.print(name);Serial.print(" : "); Serial.println(_b_,HEX);}while(0)

unsigned long tx_time;
int cnt = 0;

frame_t g_frame;
 

void setup() {

    /* operating on channel 17, not receiving when idle. */
    Radio.begin();
    Serial.begin(1000000);//500000);//250000);//115200);//9600);
    Serial.println("HelloRadio V$Release$"); 
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
              return;
          }
           
//           Serial.print("data available ");
//           Serial.println(available);
           // write integer
          
           uint32_t val;
           uint8_t* aux = (uint8_t *)(&g_frame);
           for(int i = 0;i < available; i++)
           {
             aux[i] = Radio.read();
           }
           Serial.print(g_frame.node_id);
           Serial.print(" ");
           Serial.print(g_frame.frame_index);
           Serial.print(" ");
           Serial.print(g_frame.nr);
           Serial.print(" ");
           for(int i = 0; i < g_frame.nr;i++)
           {
             Serial.print(g_frame.data[i].x);
             Serial.print(" ");
             Serial.print(g_frame.data[i].y);
             Serial.print(" ");
             Serial.print(g_frame.data[i].z);
             if(i  < g_frame.nr - 1)
             {
               Serial.print(" ");
             }
           }
           Serial.println();
         
//          val = aux<<16;
//          Radio.get_int(aux);
//          val +=aux;
//          
//          Radio.get_int(aux);
//          Serial.print("Valoarea este: ");
//          Serial.print(aux);
        
   //     }
        // Serial.println(available);
        //Radio.put(cnt);
        // write byte
        //Radio.write('\n');
        //Radio.flush();
        //Serial.println("Data read");
         
    }

}


