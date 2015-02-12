/* $Id$ */
/* RadioFaro wirless/serial bridge. */

/* data captured from UART */
int serial_inbyte = 0;

/* data received from Radio */
int serial_outbyte = 0;

/* next time the radio buffer will flushed */
unsigned long tx_time;

void setup() {
    Radio.begin();
    Serial.begin(38400);
    tx_time = 0;
}

void loop() {

    if ((Serial.available() > 0))
    {
        serial_inbyte = Serial.read();
        Radio.write(serial_inbyte);
    }

    if (millis() > tx_time){
        tx_time = millis() + 25;
        Radio.flush();
    }

    if (Radio.available() > 0)
    {
        serial_outbyte = Radio.read();
        if (serial_outbyte == DATA_INT)
        {
            int d;
            Radio.get_int(d);
            Serial.print(d);        
        }
        else if (serial_outbyte >= 0)
        {
            Serial.print((char)serial_outbyte);
        }
    }
}
