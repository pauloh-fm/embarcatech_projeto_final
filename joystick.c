#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "joystick.h"
#include "dados_lavouras.h"   // Para acessar gaiola_atual
#include "matriz_leds.h"      // Para chamar exibirGaiola()
#include <stdio.h>

#define VRX_PIN 26
#define ADC_CHANNEL_VRX 0

// Limiar para movimentos (ajuste conforme necessário)
#define THRESHOLD_RIGHT 3000
#define THRESHOLD_LEFT  1000

void setup_joystick(void) {
    adc_init();
    adc_gpio_init(VRX_PIN);
}

uint16_t read_joystick_x(void) {
    adc_select_input(ADC_CHANNEL_VRX);
    sleep_us(2);
    return adc_read();
}

void update_joystick_portao(void) {
    uint16_t x_val = read_joystick_x();
    // A variável global gaiola_atual é definida em global_vars.c (declarada extern em dados_lavouras.h)
    if (x_val > THRESHOLD_RIGHT) {
        estado_portao[gaiola_atual - 1] = 1;  // Fecha o portão
        printf("Portao da gaiola %d fechado (x=%d)\n", gaiola_atual, x_val);
        exibirGaiola();
    }
    else if (x_val < THRESHOLD_LEFT) {
        estado_portao[gaiola_atual - 1] = 0;  // Abre o portão
        printf("Portao da gaiola %d aberto (x=%d)\n", gaiola_atual, x_val);
        exibirGaiola();
    }
}
