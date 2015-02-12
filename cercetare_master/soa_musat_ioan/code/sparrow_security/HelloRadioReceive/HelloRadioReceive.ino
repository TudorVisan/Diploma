#include "message_control.h"
#include "at86rf212.h"
#include "transceiver.h"

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
    
    //Radio.begin(17,STATE_RXAUTO);
    Radio.begin();
    
    //trx_bit_write(SR_TRX_CMD,CMD_PLL_ON);
    
    trx_bit_write(SR_SHORT_ADDR_0,6);
    trx_bit_write(SR_SHORT_ADDR_1,7);
    trx_bit_write(SR_PAN_ID_0,6);
    trx_bit_write(SR_PAN_ID_1,6);
    trx_bit_write(SR_IEEE_ADDR_0,6);
    trx_bit_write(SR_IEEE_ADDR_1,6);
    trx_bit_write(SR_IEEE_ADDR_2,6);
    trx_bit_write(SR_IEEE_ADDR_3,6);
    trx_bit_write(SR_IEEE_ADDR_4,6);
    trx_bit_write(SR_IEEE_ADDR_5,6);
    trx_bit_write(SR_IEEE_ADDR_6,6);
    trx_bit_write(SR_IEEE_ADDR_7,7);
    trx_bit_write(SR_AACK_I_AM_COORD,1);
    
    trx_bit_write(SR_AACK_FLTR_RES_FT,1);
//    trx_bit_write(SR_AACK_PROM_MODE,1);
//    trx_bit_write(SR_AACK_UPLD_RES_FT,1);
    //trx_bit_write(SR_AACK_SET_PD,1);
     
    //trx_bit_write(SR_AACK_DIS_ACK,1);
    
    radio_set_state((radio_state_t)STATE_RX);
    
    Serial.begin(9600);
    
    Serial.print(trx_bit_read(SR_TRX_STATUS));
    
    Serial.println("Receiver started");
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
        
           // write integer
          
           Serial.print(" data available "); 
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
           Serial.println(); 
      }
}


