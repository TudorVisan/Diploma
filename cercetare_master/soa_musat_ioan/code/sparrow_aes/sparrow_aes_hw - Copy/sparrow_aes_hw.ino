#include <transceiver.h>
#include "chb_aes.c"
#include "chb_aes.h"

#define KEYBITS 128
#define LENGTH 16
uint8_t key[LENGTH] = {4, 74, 21, 233, 126, 94, 213, 208, 12, 215, 170, 25, 160, 142, 223, 199};
uint8_t ciphered[16]; 
uint8_t padded_length;

uint32_t start, finish, time,iteratii;

uint8_t frame[]={1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6};
uint8_t length = 16;
uint8_t tip = 0;
    
uint8_t data[LENGTH];

void setup() {
 
    Radio.begin();
    Serial.begin(9600);
    Serial.print("Pornit ");
    
    chb_aes_init(key);
    
  chb_aes_encrypt(AES_ENCRYPT,16,frame,ciphered);
  
   for(int i = 0; i< 16; i++)
  {
     Serial.print(ciphered[i]);
    Serial.print(" "); 
  }
  Serial.println("");
  
  
  chb_aes_decrypt(AES_DECRYPT,16,frame,ciphered);
  
  for(int i = 0; i< 16; i++)
  {
     Serial.print(ciphered[i]);
    Serial.print(" "); 
  }
  Serial.println("");
  
  
}

 
void loop() {

    //nu merg in acelasi timp, iese grav din ram
    //873 iteratii
    //padded_length = encrypt_CBC_sw((const uint8_t *)key, (const uint8_t *)frame, length, ciphered);
   
    //509 iteratii secunda
    //decrypt_CBC_sw((const uint8_t *)key, (const uint8_t *)ciphered, length, (uint8_t *)frame);
    
    
    iteratii++;
    finish = millis();
    time = finish - start;
    //Serial.print("o iteratie\n\r");
    if(time> 1000)
    {
    
      //Serial.print(" secunda cu ");
      //Serial.print(iteratii);
      //Serial.print("\n\r");
      iteratii = 0;
      start = millis();
     // tip = (tip +1 ) %2;
    }
   
   

} 
