
#ifndef MESSAGE_CONTROL_H
#define MESSAGE_CONTROL_H
#include <stdint.h>

#define MAX_PACKET_DATA 17 ////15 for safety

typedef struct __attribute__((packed)){
  int16_t x;
  int16_t y;
  int16_t z;
}data_t;

typedef struct __attribute__((packed)){
  uint16_t node_id; //node unique id based on the cpu id
  uint32_t timestamp; // timestamp in millis since the node has started
  uint16_t frame_index;// the unique transmision id
  uint16_t nr; //the number of frame from the current transmision
  data_t data[MAX_PACKET_DATA]; //actual accel data
}frame_t;
 //asa avem 112 bytes, numar multiplu de 128 de biti
#endif // MESSAGE_CONTROL_H //
