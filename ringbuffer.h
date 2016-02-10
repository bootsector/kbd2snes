#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include <stdint.h>

int rb_getcount();
uint8_t rb_hasitem(void);
void rb_add(uint8_t item);
uint8_t rb_get();

#endif /* RINGBUFFER_H_ */
