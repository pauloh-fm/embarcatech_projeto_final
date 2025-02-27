#include "pico/stdlib.h"
#include "matriz_leds.h"
#include "botao.h"
#include "joystick.h"
#include "dados_lavouras.h"
#include "inc/ssd1306.h"   // Biblioteca do display OLED
#include "inc/font.h"      // Fonte do display OLED
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Definições do I2C para o OLED
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define OLED_ADDR 0x3C
#define WIDTH 128
#define HEIGHT 64

// Objeto global do display OLED
ssd1306_t ssd;

// Protótipo da função de configuração do display OLED
void oled_setup(void) {
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, OLED_ADDR, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}

// Função auxiliar para remover espaços em branco
void trim(char *str) {
    char *start = str;
    while (isspace((unsigned char)*start))
        start++;
    if (start != str)
        memmove(str, start, strlen(start) + 1);
    char *end = str + strlen(str) - 1;
    while (end >= str && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }
}

// Atualiza o display OLED com o status de cada gaiola
void update_display_animais(void) {
    char line[32];
    ssd1306_fill(&ssd, false);
    for (int i = 0; i < 4; i++) {
        snprintf(line, sizeof(line), "Gaiola %d: %s", i + 1,
                 (ANIMAIS_DETECTADOS[i] == 1) ? "Detectado" : "Nao");
        ssd1306_draw_string(&ssd, line, 0, i * 12);
    }
    ssd1306_send_data(&ssd);
}

int main(void) {
    stdio_init_all();
    while (!stdio_usb_connected())
        sleep_ms(100);
    
    oled_setup();
    update_display_animais();
    
    // Exibe o menu via serial
    printf("Menu:\n");
    printf("1 - Exibir Lavoura\n");
    printf("2 - Exibir opcoes da Gaiola\n");
    fflush(stdout);
    
    init_botoes();
    npInit(LED_PIN_WS2812);
    setup_joystick();
    
    char input_char;
    while (true) {
        if (scanf(" %c", &input_char) == 1) {
            input_char = toupper((unsigned char)input_char);
            printf("Recebido: %c\n", input_char);
            fflush(stdout);
            if (input_char == '1') {
                exibirLavoura();
                printf("Exibindo lavoura...\n");
                fflush(stdout);
            }
            else if (input_char == '2') {
                char num_char;
                printf("Digite o numero da gaiola (1-4): ");
                fflush(stdout);
                if (scanf(" %c", &num_char) == 1) {
                    int num = num_char - '0';
                    if (num >= 1 && num <= 4) {
                        gaiola_atual = (uint8_t)num;
                        exibirGaiola();
                        printf("Exibindo Gaiola %d\n", gaiola_atual);
                        fflush(stdout);
                    } else {
                        printf("Numero invalido. Tente novamente.\n");
                        fflush(stdout);
                    }
                }
            }
            else {
                printf("Opcao invalida. Digite 1 para Lavoura ou 2 para Gaiola.\n");
                fflush(stdout);
            }
        }
        // Se estivermos exibindo a gaiola, atualiza o estado do portão via joystick
        update_joystick_portao();
        // Atualiza o display OLED com o status de detecção de cada gaiola
        update_display_animais();
        sleep_ms(200);
    }
    
    return 0;
}
