
#ifndef MESSAGE_CONTROL_H
#define MESSAGE_CONTROL_H
#include <stdint.h>

#define MAX_PACKET_DATA 18

typedef struct __attribute__((packed)){
  int16_t x;
  int16_t y;
  int16_t z;
}data_t;

typedef struct __attribute__((packed)){
  uint8_t node_id;
  uint16_t frame_index;
  uint8_t nr;
  data_t data[MAX_PACKET_DATA];
}frame_t;
 
#endif // MESSAGE_CONTROL_H //
