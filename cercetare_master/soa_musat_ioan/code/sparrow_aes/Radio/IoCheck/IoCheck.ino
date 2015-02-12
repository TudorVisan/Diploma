/* $Id$ */
/**
 * I/O Check sketch for ZigBit adaptation
 */

/* meshbean digital pins */
#define LED1 0
#define LED2 1
#define LED3 2

#define SW1 11
#define SW2 12
#define SW3 13

#define KEY1_IRQ 6
#define KEY2_IRQ 5


void blink(void)
{
    Serial.println("KEY1");
}



int incomingByte = 0;	// for incoming serial data
uint8_t val = 0;
void setup()
{
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  //pinMode(SW1, INPUT);
  Serial.begin(9600);

  /* configure input pins */
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);

  digitalWrite(SW1, HIGH);   // enable internal pull up
  digitalWrite(SW2, HIGH);   // sets the LED on
  digitalWrite(SW3, HIGH);   // sets the LED on

  attachInterrupt(KEY1_IRQ, blink, LOW);
  digitalWrite(21, HIGH);   // sets the LED on
  pinMode(21, INPUT);


  Serial.println("Sketch IoCheck");
}

void loop()
{

    /* check serial IO */
    if (Serial.available() > 0) {
		// read the incoming byte:
		incomingByte = Serial.read();
        if (incomingByte == '+')
        {
            Serial.print("dimm + ");

            val += 64;
            analogWrite(LED1, val);
            Serial.println(val, DEC);
        }
        if (incomingByte == '-')
        {
            Serial.print("dimm - ");
            val -= 64;
            analogWrite(LED1, val);
            Serial.println(val, DEC);

        }
        if (incomingByte == 'i')
        {
            Serial.print("i: SW1: ");
            Serial.print(digitalRead(SW1));
            Serial.print(" SW2: ");
            Serial.print(digitalRead(SW2));
            Serial.print(" SW3: ");
            Serial.print(digitalRead(SW3));
            Serial.print(" A0: ");
            Serial.print(analogRead(0));
            Serial.print(" A1: ");
            Serial.print(analogRead(1));
            Serial.print(" A2: ");
            Serial.print(analogRead(2));
            Serial.print(" A3: ");
            Serial.print(analogRead(3));
            Serial.println("<<==");
        }
        if (incomingByte == '?' || incomingByte == 'h')
        {
            Serial.print("help\n\r"
                "   + : increase dimmed value\n\r"
                "   - : decrease dimmed value\n\r"
                "   i : show value of switches SW[1...3] and ADC's A[0...3]\n\r"
                );
        }
	}


}


