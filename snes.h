#ifndef SNES_H_
#define SNES_H_

#define SNES_B		0
#define SNES_Y		1
#define SNES_SELECT	2
#define SNES_START	3
#define SNES_UP		4
#define SNES_DOWN	5
#define SNES_LEFT	6
#define SNES_RIGHT	7
#define SNES_A		8
#define SNES_X		9
#define SNES_L		10
#define SNES_R		11

void snes_init(void);
void snes_press(uint8_t button);
void snes_release(uint8_t button);

#endif /* SNES_H_ */
