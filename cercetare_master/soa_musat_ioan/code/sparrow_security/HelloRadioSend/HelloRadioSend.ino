#include "message_control.h"
#include "at86rf212.h"
#include "transceiver.h"
 

#define REG(name, reg) do{uint8_t _b_ = reg; Serial.print(name);Serial.print(" : "); Serial.println(_b_,HEX);}while(0)

 
unsigned long tx_time;

frame_t g_frame; 

void setup()
{
    Serial.begin(9600); 
    
    Radio.begin(17,STATE_OFF);//STATE_TXAUTO);// STATE_RXAUTO);//STATE_OFF);
    
    
//    trx_bit_write(SR_TRX_CMD,CMD_PLL_ON);
    
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
    trx_bit_write(SR_AACK_DIS_ACK,1);
//    trx_bit_write(SR_RX_SAFE_MODE,1);
    
    radio_set_state((radio_state_t)STATE_TX);
    
    Serial.begin(9600);
    
    Serial.print(trx_bit_read(SR_TRX_STATUS));
   // radio_set_state((radio_state_t)STATE_TXAUTO);

    Serial.print("Sender started"); 
    Serial.println();
    tx_time = 0;
}
 
void loop()
{
  
  
    if (millis() > tx_time){
        tx_time = millis() + 250;
        Serial.print("started sending data ");
          Serial.println(g_frame.frame_index);
            g_frame.node_id = 1;
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
 
