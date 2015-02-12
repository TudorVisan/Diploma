
#include <SPI.h> // Included for SFE_LSM9DS0 library
#include <Wire.h>
#include "message_control.h"
 

#define REG(name, reg) do{uint8_t _b_ = reg; Serial.print(name);Serial.print(" : "); Serial.println(_b_,HEX);}while(0)

 
unsigned long tx_time;

frame_t g_frame; 

void setup()
{
  Serial.begin(9600); 
  
  Radio.begin(17, STATE_OFF);
  
  // Or call it with declarations for sensor scales and data rates:  
  //uint16_t status = dof.begin(dof.G_SCALE_2000DPS, 
  //                          dof.A_SCALE_2G, dof.M_SCALE_2GS);
  
  // begin() returns a 16-bit value which includes both the gyro 
  // and accelerometers WHO_AM_I response. You can check this to
  // make sure communication was successful.
  Serial.print("LSM9DS0 WHO_AM_I's returned: 0x"); 
  Serial.println();
   tx_time = 0;
}
 
void loop()
{
  
  
    if (millis() > tx_time){
        tx_time = millis() + 250;
        Serial.print("started sending data ");
          Serial.println(g_frame.frame_index);
            g_frame.node_id = 0;
            uint8_t *aux = (uint8_t*)(&g_frame);
            for(int i =0; i < sizeof(frame_t);i++)
            {
              Radio.write(aux[i]);
            }
            Radio.flush();
            g_frame.frame_index ++;
            g_frame.nr = 0;
                     
      }      
      
}
 
