#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>
#include "botao.h"

uint8_t ANIMAIS_DETECTADOS[4] = {0, 0, 0, 0};
uint8_t atual_animais_detectados = 0;


// Função de debounce por software para os botões
bool button_debounce(uint botao_pin) {
    static uint32_t last_press_time[30] = {0};  // Tempo da última ativação para cada pino
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    if (gpio_get(botao_pin) == 0) {  // Botão pressionado (nível baixo)
        if (current_time - last_press_time[botao_pin] > DEBOUNCE_TIME) {
            last_press_time[botao_pin] = current_time;
            return true;
        }
    }
    return false;
}

// Função de interrupção para os botões
void gpio_callback(uint gpio, uint32_t events) {
    if (button_debounce(gpio)) {
        if (gpio == JOYSTICK_BUTTON) {
            printf("Botão do joystick pressionado\n");
        }
        else if (gpio == BOTAO_A) {
            // Simula a detecção de animal na gaiola:
            // Define ANIMAIS_DETECTADOS no índice atual como 1 (detectado)
            ANIMAIS_DETECTADOS[atual_animais_detectados] = 1;
            printf("Animal detectado na gaiola %d\n", atual_animais_detectados + 1);
            // Incrementa com wrap-around (0 a 3)
            atual_animais_detectados++;
            if (atual_animais_detectados >= 4) {
                atual_animais_detectados = 0;
            }
        }
    }
}

// Inicializa os botões e configura as interrupções
void init_botoes() {
    gpio_init(JOYSTICK_BUTTON);
    gpio_set_dir(JOYSTICK_BUTTON, GPIO_IN);
    gpio_pull_up(JOYSTICK_BUTTON);

    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);

    // Configura as interrupções com a mesma callback para os botões relevantes
    gpio_set_irq_enabled_with_callback(JOYSTICK_BUTTON, GPIO_IRQ_EDGE_FALL, true, gpio_callback);
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, gpio_callback);
}
