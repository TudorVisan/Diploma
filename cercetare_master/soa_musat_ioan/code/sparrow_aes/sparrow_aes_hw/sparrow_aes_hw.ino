#include <transceiver.h>
#include <stdint.h>

#define KEYBITS 128
#define LENGTH 18
#define SPI_WAITFOR()        do { while((SPSR & _BV(SPIF)) == 0);  } while(0)

#define AES_BLOCKSIZE   16
#define AES_KEYSIZE     16
#define AES_ENCRYPT     0
#define AES_DECRYPT     1
#define AES_ECB         0
#define AES_KEY         1
#define AES_CBC         2
#define AES_NO_REQ      0
#define AES_START_REQ   1

#define AES_BASE_ADDR       0x80
#define AES_STATUS          0x82
#define AES_CTRL            0x83
#define AES_STATE_KEY_0     0x84


uint8_t key[LENGTH] = {0,4, 74, 21, 233, 126, 94, 213, 208, 12, 215, 170, 25, 160, 142, 223, 199,0};
uint8_t ciphered[LENGTH];
uint8_t dummy[LENGTH];
uint8_t deckey[LENGTH];
uint8_t padded_length;

uint32_t start, finish, time,iteratii;

uint8_t frame[LENGTH]={0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,0};
 
uint8_t tip = 0;
    
uint8_t data[LENGTH];

void setcfg(uint8_t *v,uint8_t dir,uint8_t mode,uint8_t request)
{
   v[0] = v[17] = (mode << 4) | (dir << 3);
   v[17] |= (request << 7); 
}

void sram_write(uint8_t length, uint8_t *data,uint8_t *rdata)
{

    // Select transceiver
    SPI_SELN_LOW();
ca
    SPI_DATA_REG = TRX_CMD_SW;
    SPI_WAITFOR();
    
    SPI_DATA_REG = AES_CTRL;
    SPI_WAITFOR();
    uint8_t i = 0;
    do
    {
        SPI_DATA_REG = *data++;
        SPI_WAITFOR();
        if(i != 0)
          *rdata++ = SPI_DATA_REG;
      i++;
    }
    while (--length > 0);

    // Deselect Slave
    SPI_SELN_HIGH();
}


void sram_read( uint8_t length, uint8_t *data)
{

    // Select transceiver
    SPI_SELN_LOW();

    SPI_DATA_REG = TRX_CMD_SR;
    SPI_WAITFOR();
    SPI_DATA_REG = AES_CTRL;
    SPI_WAITFOR();
    do
    {
        SPI_DATA_REG = 0;   /* dummy out */
        SPI_WAITFOR();
        *data++ = SPI_DATA_REG;
    }
    while (--length > 0);

    // Deselect Slave
    SPI_SELN_HIGH();
}


void setup() {
 
    Radio.begin(17,STATE_OFF);
    Serial.begin(9600);
    Serial.print("Pornit ");
  
  
  
  /* 
     setcfg(key, AES_ENCRYPT, AES_KEY, AES_NO_REQ);
     sram_write(LENGTH,key,dummy);
     setcfg(dummy,AES_ENCRYPT, AES_ECB, AES_START_REQ);
     sram_write(LENGTH,dummy,ciphered);
     setcfg(dummy,AES_DECRYPT, AES_KEY, AES_NO_REQ);
     sram_write(LENGTH,dummy,deckey);
     sram_read(LENGTH,deckey);
     
    
    setcfg(key, AES_ENCRYPT, AES_KEY, AES_NO_REQ);
     sram_write(LENGTH,key,dummy);
    setcfg(frame,AES_ENCRYPT, AES_ECB, AES_START_REQ);
     sram_write(LENGTH,frame,dummy);
     sram_read(LENGTH,ciphered);
   for(int i = 0; i< LENGTH;i++)
  {
     Serial.print(ciphered[i]);
    Serial.print(" "); 
  }
  Serial.println("");
   for(int i = 0; i< LENGTH;i++)
  {
     Serial.print(dummy[i]);
    Serial.print(" "); 
  }
  Serial.println("");
  
   setcfg(deckey, AES_DECRYPT, AES_KEY, AES_NO_REQ);
     sram_write(LENGTH,deckey,dummy);
    setcfg(ciphered,AES_DECRYPT, AES_ECB, AES_START_REQ);
     sram_write(LENGTH,ciphered,dummy);
     sram_read(LENGTH,dummy);
   for(int i = 0; i< LENGTH;i++)
  {
     Serial.print(dummy[i]);
    Serial.print(" "); 
  }
  Serial.println("");
  */
  
}

void xor_sw(const uint8_t* text, const uint8_t* vector, uint8_t* rezultat)
{
	int i;

	for(i=0; i<16; i++)
		rezultat[i] = text[i] ^ vector[i];
}

 
void loop() {

    //nu merg in acelasi timp, iese grav din ram
    //873 iteratii
    //padded_length = encrypt_CBC_sw((const uint8_t *)key, (const uint8_t *)frame, length, ciphered);
   
    //509 iteratii secunda
    //decrypt_CBC_sw((const uint8_t *)key, (const uint8_t *)ciphered, length, (uint8_t *)frame);
    
    
    //3552 criptari pe secunda, ECB,  la 5 secunde 17724
    //3550 cripari CBC la secunda,  la 5 secunde 17716
     setcfg(key, AES_ENCRYPT, AES_KEY, AES_NO_REQ);
     sram_write(LENGTH,key,dummy);
     setcfg(frame,AES_ENCRYPT, AES_ECB, AES_START_REQ);
     sram_write(LENGTH,frame,dummy);
     sram_read(LENGTH,ciphered);
    
    //3550 decriptari ECB, 17719 la 5 secunde
    //3380 decritpari CBC, la secunda, 16864 la 5 secundae
    
  //CBC HARDWARE
//     setcfg(key, AES_DECRYPT, AES_KEY, AES_NO_REQ);
//     sram_write(LENGTH,key,dummy);
//     setcfg(frame,AES_DECRYPT, AES_ECB, AES_START_REQ);
//     sram_write(LENGTH,frame,dummy);
//     sram_read(LENGTH,ciphered);

  //CBC HARDWARE
//     xor_sw(ciphered,ciphered,ciphered);
     
    iteratii++;
    finish = millis();
    time = finish - start;
   // Serial.print(iteratii);
    if(time>  5000)
    {
    
      Serial.print(" secunda cu ");
      Serial.print(iteratii);
      Serial.print("\n\r");
      iteratii = 0;
      start = millis();
     // tip = (tip +1 ) %2;
    }
   

} 
