#include "rijndael.h"
#include "aes_software.h"



#define KEYBITS 128
unsigned char key[KEYLENGTH(KEYBITS)] = {4, 74, 21, 233, 126, 94, 213, 208, 12, 215, 170, 25, 160, 142, 223, 199};
uint8_t ciphered[128];
uint8_t array[128];
uint8_t padded_length;

uint32_t start, finish, time,iteratii;

char frame[]={"1234567890123456"};
uint8_t length = 16;
uint8_t tip = 0;
    
  
void setup() {
   Radio.begin(17,STATE_SLEEP);
  Serial.begin(9600);
  Serial.print("Pornit ");
}

 
void loop() {

    //nu merg in acelasi timp, iese grav din ram
    //CBC 873 iteratii secunda, 1747 la 2 secunde, 4368 la 5 secunde
    //ECB 901, iteratii la secunda , 4501 la 5 secunde
    //padded_length = encrypt_ECB_sw((const uint8_t *)key, (const uint8_t *)frame, length, ciphered);
   
    //CBC 509 iteratii secunda, la 3 secunde, 2545 la 5 secunde
    //ECB 520 iteratii pe secunda, 2595 la 5 secunde
    decrypt_ECB_sw((const uint8_t *)key, (const uint8_t *)ciphered, length, (uint8_t *)frame);
    
    
    iteratii++;
    finish = millis();
    time = finish - start;
    //Serial.print("o iteratie\n\r");
    if(time> 5000)
    {
    
      Serial.print(" secunda cu ");
      Serial.print(iteratii);
      Serial.print("\n\r");
      iteratii = 0;
      start = millis();
     // tip = (tip +1 ) %2;
    }
   
   

} 
