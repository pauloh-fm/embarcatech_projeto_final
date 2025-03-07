#include "pico/stdlib.h"
#include "ws2818b.pio.h"
#include "matriz_leds.h"
#include "hardware/pio.h"
#include <stdio.h>
#include "dados_lavouras.h"

// Declaramos as variáveis globais que foram definidas em outro módulo (global_vars.c)
extern uint8_t ANIMAIS_DETECTADOS[4];
extern uint8_t gaiola_atual;
extern uint8_t estado_portao[4];

// Mapeamento para as gaiolas na visualização da lavoura (índices no array lavoura_leds)
static const uint8_t lavoura_gaiola_indices[4] = {2, 10, 14, 22};
// Mapeamento para as gaiolas na visualização da gaiola (índices no array gaiola_leds)
static const uint8_t gaiola_indices[4] = {7, 11, 13, 17};

// Definição de pixel no formato GRB (ordem exigida pelo WS2812B)
struct pixel_t {
    uint8_t G, R, B;
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t;

// Buffer de pixels que formam a matriz LED
npLED_t leds[LED_COUNT];

// Variáveis para uso da máquina PIO
PIO np_pio;
uint sm;

// Constantes de cores (valores decimais de 0 a 255)
// Cerca ('C'): verde
#define COLOR_C_R  0
#define COLOR_C_G  128
#define COLOR_C_B  0

// Gaiola em estado normal ('G'): azul
#define COLOR_G_IDLE_R  0
#define COLOR_G_IDLE_G  0
#define COLOR_G_IDLE_B  255

// Portão ('P'): azul (fechado)
#define COLOR_P_R  0
#define COLOR_P_G  0
#define COLOR_P_B  255

// Detecção de animal ('A'): vermelho
#define COLOR_A_R  255
#define COLOR_A_G  0
#define COLOR_A_B  0

// Sensor ('T'): amarelo
#define COLOR_T_R  255
#define COLOR_T_G  255
#define COLOR_T_B  0

// Cor OFF: preto
#define COLOR_OFF_R  0
#define COLOR_OFF_G  0
#define COLOR_OFF_B  0

// Inicializa o sistema de LEDs (WS2818B) na pinagem indicada
void npInit(uint pin) {
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;
    sm = pio_claim_unused_sm(np_pio, false);
    if (sm < 0) {
        np_pio = pio1;
        sm = pio_claim_unused_sm(np_pio, true);
    }
    ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);
    for (uint i = 0; i < LED_COUNT; ++i) {
        leds[i].R = 0;
        leds[i].G = 0;
        leds[i].B = 0;
    }
}

// Atribui uma cor (R,G,B) a um LED (índice)
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
    leds[index].R = r;
    leds[index].G = g;
    leds[index].B = b;
}

// Limpa o buffer de LEDs (apaga todos)
void npClear(void) {
    for (uint i = 0; i < LED_COUNT; ++i)
        npSetLED(i, COLOR_OFF_R, COLOR_OFF_G, COLOR_OFF_B);
}

// Escreve o buffer de pixels nos LEDs (via PIO)
void npWrite(void) {
    for (uint i = 0; i < LED_COUNT; ++i) {
        pio_sm_put_blocking(np_pio, sm, leds[i].G);
        pio_sm_put_blocking(np_pio, sm, leds[i].R);
        pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
    sleep_us(100); // Intervalo para sinal de RESET
}

// Mapeia coordenadas (x, y) da matriz 5x5 para o índice correto do LED
int obterIndice(uint x, uint y) {
    int dicionario_indice[] = {
        24, 23, 22, 21, 20,
        15, 16, 17, 18, 19,
        14, 13, 12, 11, 10,
         5,  6,  7,  8,  9,
         4,  3,  2,  1,  0
    };
    return dicionario_indice[x + y * 5];
}

/**
 * Exibe o padrão completo da lavoura usando o array "lavoura_leds":
 *  - 'C': Cerca (verde)
 *  - 'G': Gaiola (estado normal, azul)
 *  - 'T': Sensor (amarelo)
 *  - Outros (incluindo ' '): LED apagado
 *
 * Após exibir o padrão base, para cada gaiola (índices definidos em lavoura_gaiola_indices),
 * se ANIMAIS_DETECTADOS[i] for 1, o LED correspondente é sobrescrito com vermelho (detecção animal).
 */
void exibirLavoura(void) {
    npClear();
    // Exibe o padrão base da lavoura
    for (int i = 0; i < LED_COUNT; i++) {
        uint8_t r, g, b;
        switch (lavoura_leds[i].tipo) {
            case 'C':
                r = COLOR_C_R; g = COLOR_C_G; b = COLOR_C_B;
                break;
            case 'G':
                r = COLOR_G_IDLE_R; g = COLOR_G_IDLE_G; b = COLOR_G_IDLE_B;
                break;
            case 'T':
                r = COLOR_T_R; g = COLOR_T_G; b = COLOR_T_B;
                break;
            default:
                r = COLOR_OFF_R; g = COLOR_OFF_G; b = COLOR_OFF_B;
        }
        int indice = obterIndice(lavoura_leds[i].x, lavoura_leds[i].y);
        npSetLED(indice, r, g, b);
    }
    
    // Para cada gaiola, se houver detecção, sobrescreve o LED correspondente com vermelho
    for (int i = 0; i < 4; i++) {
        if (ANIMAIS_DETECTADOS[i] == 1) {
            int idx = lavoura_gaiola_indices[i];
            int indice = obterIndice(lavoura_leds[idx].x, lavoura_leds[idx].y);
            npSetLED(indice, COLOR_A_R, COLOR_A_G, COLOR_A_B);
        }
    }
    npWrite();
}

/**
 * Exibe o padrão da gaiola usando o array "gaiola_leds":
 *  - 'C': Cerca (verde)
 *  - 'P': Portão (azul, fechado)
 *  - 'A': Sensor de detecção de animal (vermelho se ativo, apagado se inativo)
 *  - Outros: LED apagado
 *
 * Nesta função, o LED do tipo 'A' é exibido em vermelho somente se ANIMAIS_DETECTADOS[gaiola_atual - 1] for 1.
 * Além disso, atualiza-se um LED específico (aqui usamos o índice 22) para indicar o estado do portão da gaiola atual:
 * se estado_portao[gaiola_atual - 1] for 1, esse LED é azul (portão fechado); se 0, permanece apagado.
 */
void exibirGaiola(void) {
    npClear();
    // Exibe o padrão base da gaiola
    for (int i = 0; i < LED_COUNT; i++) {
        uint8_t r, g, b;
        switch (gaiola_leds[i].tipo) {
            case 'C':
                r = COLOR_C_R; g = COLOR_C_G; b = COLOR_C_B;
                break;
            case 'P':
                r = COLOR_P_R; g = COLOR_P_G; b = COLOR_P_B;
                break;
            case 'A':
                if (ANIMAIS_DETECTADOS[gaiola_atual - 1] == 1)
                    r = COLOR_A_R, g = COLOR_A_G, b = COLOR_A_B;
                else
                    r = COLOR_OFF_R, g = COLOR_OFF_G, b = COLOR_OFF_B;
                break;
            default:
                r = COLOR_OFF_R; g = COLOR_OFF_G; b = COLOR_OFF_B;
        }
        int indice = obterIndice(gaiola_leds[i].x, gaiola_leds[i].y);
        npSetLED(indice, r, g, b);
    }
    // Atualiza o LED da posição 22 para indicar o estado do portão da gaiola atual:
    if (estado_portao[gaiola_atual - 1] == 1)
         npSetLED(22, COLOR_P_R, COLOR_P_G, COLOR_P_B);
    else
         npSetLED(22, COLOR_OFF_R, COLOR_OFF_G, COLOR_OFF_B);
    npWrite();
}
