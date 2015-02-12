
#ifndef MESSAGE_CONTROL_H
#define MESSAGE_CONTROL_H
#include <stdint.h>

#define MAX_PACKET_DATA 19

typedef struct{
  int16_t x;
  int16_t y;
  int16_t z;
}data_t;

typedef struct{
  uint8_t node_id;
  uint16_t frame_index;
  uint8_t nr;
  data_t data[MAX_PACKET_DATA];
}frame_t;
 
#endif // MESSAGE_CONTROL_H //
