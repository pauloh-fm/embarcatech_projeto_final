#include <stdint.h>

void setup_joystick(void);
uint16_t read_joystick_x(void);
void update_joystick_portao(void);

// Variável global para armazenar o estado do portão de cada gaiola:
// 0 = aberto, 1 = fechado
extern uint8_t estado_portao[4];