#include <ringbuffer.h>

#define BUFFER_SIZE 255

static uint8_t buffer[BUFFER_SIZE];

static volatile int head = 0;
static volatile int tail = 0;

int rb_getcount() {
	return (BUFFER_SIZE + head - tail) % BUFFER_SIZE;
}

uint8_t rb_hasitem(void) {
	return rb_getcount() > 0;
}

void rb_add(uint8_t item) {
	buffer[head] = item;

	head = (head + 1) % BUFFER_SIZE;

	// Head pushes Tail forward if full...
	if (head == tail) {
		tail = (tail + 1) % BUFFER_SIZE;
	}
}

uint8_t rb_get() {
	uint8_t element = buffer[tail];

	if(rb_hasitem())
		tail = (tail + 1) % BUFFER_SIZE;

	return element;
}

