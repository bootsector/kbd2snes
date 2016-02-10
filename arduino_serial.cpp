#include <Arduino.h>

// C interface (wrapper) functions
extern "C" void Serial_begin(unsigned long baud) {
	Serial.begin(baud);
}

extern "C" void Serial_println(const char data[]) {
	Serial.println(data);
}

