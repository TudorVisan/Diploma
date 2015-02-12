

/* $Id$ */
/**
 * Use of the radio functions
 */

#define REG(name, reg) do{uint8_t _b_ = reg; Serial.print(name);Serial.print(" : "); Serial.println(_b_,HEX);}while(0)

unsigned long tx_time;
int cnt = 0;
void setup() {

    /* operating on channel 17, not receiving when idle. */
    Radio.begin();
    Serial.begin(9600);
    Serial.println("HelloRadio V$Release$");
}

void loop() {

    if (millis() > tx_time){
        tx_time = millis() + 25;
        // write string
        //Serial.print("Radio Available ");
        
        int available = Radio.available();
        if(available)
        {
          Serial.println("data available");
          // write integer
          int16_t val;
          Radio.get_int(val);
          Serial.print("Valoarea este: ");
          Serial.println(val);
        }
        //Radio.put(cnt);
        // write byte
        //Radio.write('\n');
        //Radio.flush();
        //Serial.println("Data read");
         
    }

}


