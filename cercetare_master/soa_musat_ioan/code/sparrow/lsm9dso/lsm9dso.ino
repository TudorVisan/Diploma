
#include <SPI.h> // Included for SFE_LSM9DS0 library
#include <Wire.h>
#include "SFE_LSM9DS0.h" 
#include "message_control.h"

///////////////////////
// Example I2C Setup //
///////////////////////
// Comment out this section if you're using SPI
// SDO_XM and SDO_G are both grounded, so our addresses are:
#define LSM9DS0_XM  0x1D // Would be 0x1E if SDO_XM is LOW
#define LSM9DS0_G   0x6B // Would be 0x6A if SDO_G is LOW
// Create an instance of the LSM9DS0 library called `dof` the
// parameters for this constructor are:
// [SPI or I2C Mode declaration],[gyro I2C address],[xm I2C add.]
LSM9DS0 dof(MODE_I2C, LSM9DS0_G, LSM9DS0_XM);

///////////////////////
// Example SPI Setup //
///////////////////////
/* // Uncomment this section if you're using SPI
#define LSM9DS0_CSG  9  // CSG connected to Arduino pin 9
#define LSM9DS0_CSXM 10 // CSXM connected to Arduino pin 10
LSM9DS0 dof(MODE_SPI, LSM9DS0_CSG, LSM9DS0_CSXM);
*/

#define STATUS_IDLE 0
#define STATUS_ACTIVE 1

#define PRINT_SPEED 10 // 500 ms between prints
#define READ_SPEED 5

#define MAX_IDLE_READS 18


uint32_t nr_pozitive_reads;

int x_raw,y_raw,z_raw;
double x_calc,y_calc,z_calc;

int16_t max_x = -32768, max_y =32768,max_z =-32768,
      min_x = 32767, min_y = 32767,min_z = 32767;
      
uint8_t g_status;
uint16_t g_frame_index;
uint16_t g_id=666;



uint16_t g_max_idle_reads;

frame_t g_frame; 

void setup()
{
  Serial.begin(9600); // Start serial at 115200 bps
  // Use the begin() function to initialize the LSM9DS0 library.
  // You can either call it with no parameters (the easy way):
  uint16_t status = dof.begin();
  dof.setAccelScale(dof.A_SCALE_2G);
  dof.setAccelODR(dof.A_ODR_800);
  
  g_frame.node_id = g_id;
  
  Radio.begin(17, STATE_OFF);
  
  // Or call it with declarations for sensor scales and data rates:  
  //uint16_t status = dof.begin(dof.G_SCALE_2000DPS, 
  //                          dof.A_SCALE_2G, dof.M_SCALE_2GS);
  
  // begin() returns a 16-bit value which includes both the gyro 
  // and accelerometers WHO_AM_I response. You can check this to
  // make sure communication was successful.
  Serial.print("LSM9DS0 WHO_AM_I's returned: 0x");
  Serial.println(status, HEX);
  Serial.println("Should be 0x49D4");
  Serial.println();
  
  
  calibrate();
  g_status = STATUS_IDLE;
}

void calibrate()
{ 
  delay(3000); 
  
  int time = millis();
  
  dof.setAccelODR(dof.A_ODR_200);//200
  double x = 0, y = 0, z = 0,nr =400;//400;
//  int max_x = 0, max_y =0,max_z =0,
//      min_x = 100000, min_y = 100000,min_z = 100000;
  
  int i; 
  for(i = 0;i < nr;i++)
  {
    dof.readAccel();
    x_raw+=dof.ax/nr;
    y_raw+=dof.ay/nr;
    z_raw+=dof.az/nr; 
    
    if(dof.ax > max_x)
    {
      max_x = dof.ax;
    } else {
       if(dof.ax < min_x)
      {
        min_x = dof.ax;
      } 
    }
    
    if(dof.ay > max_y)
    {
      max_y = dof.ay;
    } else {
       if(dof.ay < min_y)
      {
        min_y = dof.ay;
      } 
    }
    
    if(dof.az > max_z)
    {
      max_z = dof.az;
    } else {
       if(dof.az < min_z)
      {
        min_z = dof.az;
      } 
    }
    
    delay(READ_SPEED);  
    time = millis();
  } 
  
  dof.setAccelODR(dof.A_ODR_50);
  
  Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.print(", ");
  Serial.println(z);
  Serial.print(", ");
  Serial.println(nr);
  
  x_calc = dof.calcAccel(x_raw);
  y_calc = dof.calcAccel(y_raw);
  z_calc = dof.calcAccel(z_raw); 
   
  Serial.print(x_calc, 2);
  Serial.print(", ");
  Serial.print(y_calc, 2);
  Serial.print(", ");
  Serial.print(z_calc, 2);
  Serial.println("");
   
  
  Serial.print(min_x);
  Serial.print(", ");
  Serial.print(min_y);
  Serial.print(", ");
  Serial.println(min_z);
  
  
  Serial.print(max_x);
  Serial.print(", ");
  Serial.print(max_y);
  Serial.print(", ");
  Serial.println(max_z);
  Serial.println("");
}

void loop()
{
  
  printAccel(); // Print "A: ax, ay, az"
  
  // Print the heading and orientation for fun!
//  printOrientation(dof.calcAccel(dof.ax), dof.calcAccel(dof.ay), 
//                   dof.calcAccel(dof.az));

  
  delay(PRINT_SPEED);
}


void printAccel()
{
  dof.readAccel();
  uint8_t send_data = 0;
  
          g_frame.node_id = g_id;
            Radio.flush();
  if(inLimits(dof.ax,min_x,max_x) == false ||
      inLimits(dof.ay,min_y,max_y) == false ||
      inLimits(dof.az,min_z,max_z) == false)
      {
        
          if(g_status == STATUS_IDLE)
          {
            g_frame.frame_index = 0;
            g_frame.nr = 0;
          }
          send_data = 1;
           
//        Serial.print("A: ");
//        Serial.print(millis());
//        Serial.print(" "); 
//        Serial.print(dof.calcAccel(dof.ax), 2);
//        Serial.print(", ");
//        Serial.print(dof.calcAccel(dof.ay), 2);
//        Serial.print(", ");
//        Serial.println(dof.calcAccel(dof.az), 2); 


//          nr_pozitive_reads ++;
//          uint32_t aux;
//          aux = 2*2147483647;//millis();
//          for(int i =0; i < 4; i++)
//          {
//            
//              Radio.write((int8_t)(aux%256));
//              aux/=256;
//          } 
//          aux = nr_pozitive_reads;
//          for(int i =0; i < 16; i++)
//          {
//            
//              Radio.write((int8_t)(aux%256));
//              aux/=256;
//          } 
//          
//          Radio.flush();

          g_status = STATUS_ACTIVE;
          g_max_idle_reads = 0;
      }else {
         if(g_status == STATUS_ACTIVE)
         {
             send_data = 1;
             g_max_idle_reads ++;
           
             if(g_max_idle_reads >= MAX_IDLE_READS)
             {
               send_data = 0;
               g_status = STATUS_IDLE;
             
              g_frame.timestamp = millis();
              uint8_t *aux = (uint8_t*)(&g_frame);
              
              for(int i =0; i < sizeof(frame_t);i++)
              {
                Radio.write(aux[i]);
              }
             
//                Serial.print("stoped sending data ");
//                Serial.print(g_frame.frame_index);
//                Serial.print(" ");
//                Serial.println(g_frame.nr);
        
             }
         }
      }
      
      if(send_data == 1)
      {
        if(g_frame.nr == 0)
        {
         // Serial.print("started gathering data ");
         // Serial.println(g_frame.frame_index);
        }
          g_frame.data[g_frame.nr].x = dof.ax;
          g_frame.data[g_frame.nr].y = dof.ay;
          g_frame.data[g_frame.nr].z = dof.az;
          g_frame.nr ++;
          g_frame.timestamp = millis();
          if(g_frame.nr == MAX_PACKET_DATA)
          {
             
          Serial.print("started sending data ");
          Serial.print(millis());
          Serial.print(" ");
          Serial.print(g_frame.timestamp);
          Serial.print(" ");
          Serial.println(g_frame.frame_index);
          
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
}

boolean inLimits(int value,int min_l,int max_l)
{
   if(value < min_l)
     return false; 
   return value < max_l; 
}

// Another fun function that does calculations based on the
// acclerometer data. This function will print your LSM9DS0's
// orientation -- it's roll and pitch angles.
void printOrientation(float x, float y, float z)
{
  float pitch, roll;
  
  pitch = atan2(x, sqrt(y * y) + (z * z));
  roll = atan2(y, sqrt(x * x) + (z * z));
  pitch *= 180.0 / PI;
  roll *= 180.0 / PI;
  
  Serial.print("Pitch, Roll: ");
  Serial.print(pitch, 2);
  Serial.print(", ");
  Serial.println(roll, 2);
}
