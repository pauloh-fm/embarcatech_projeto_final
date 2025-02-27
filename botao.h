#include "pico/stdlib.h"
#include "dados_lavouras.h"
// Definições dos pinos
#define JOYSTICK_BUTTON 22
#define BOTAO_A 5
#define BOTAO_B 6
#define DEBOUNCE_TIME 250  // Debounce em milissegundos
// Variável global para indicar qual gaiola (índice 0 a 3) está sendo atualizada
// Vetor global que indica se há animal detectado em cada gaiola (0 = não, 1 = sim)

void init_botoes();
void gpio_callback(uint gpio, uint32_t events);
bool button_debounce(uint botao_pin);