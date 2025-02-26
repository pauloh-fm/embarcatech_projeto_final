#include "pico/stdlib.h"
#include "ws2818b.pio.h"
#include "matriz_leds.h"
#include "hardware/pio.h"
#include "numeros.h"
#include <stdio.h>
// Definição de pixel GRB
struct pixel_t {
  uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t" por clareza.

// Declaração do buffer de pixels que formam a matriz.
npLED_t leds[LED_COUNT];

// Variáveis para uso da máquina PIO.
PIO np_pio;
uint sm;

void npInit(uint pin)
{

    // Cria programa PIO.
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;

    // Toma posse de uma máquina PIO.
    sm = pio_claim_unused_sm(np_pio, false);
    if (sm < 0)
    {
        np_pio = pio1;
        sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
    }

    // Inicia programa na máquina PIO obtida.
    ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

    // Limpa buffer de pixels.
    for (uint i = 0; i < LED_COUNT; ++i)
    {
        leds[i].R = 0;
        leds[i].G = 0;
        leds[i].B = 0;
    }
}

/**
 * Atribui uma cor RGB a um LED.
 */
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b)
{
    leds[index].R = r;
    leds[index].G = g;
    leds[index].B = b;
}

/**
 * Limpa o buffer de pixels.
 */
void npClear()
{
    for (uint i = 0; i < LED_COUNT; ++i)
        npSetLED(i, 0, 0, 0);
}

/**
 * Escreve os dados do buffer nos LEDs.
 */
void npWrite()
{
    // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
    for (uint i = 0; i < LED_COUNT; ++i)
    {
        pio_sm_put_blocking(np_pio, sm, leds[i].G);
        pio_sm_put_blocking(np_pio, sm, leds[i].R);
        pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
    sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}

int obterIndice(uint x, uint y) {
    int dicionario_indice[] = {24, 23, 22, 21, 20, 15, 16, 17, 18, 19, 14, 13 , 12, 11, 10, 5, 6, 7, 8, 9, 4, 3, 2, 1, 0};
    return dicionario_indice[x + y * 5];
}

void imprimir_numero(const uint8_t numero[][2], size_t tamanho, uint8_t r, uint8_t g, uint8_t b) {
    npClear();
    for (size_t i = 0; i < tamanho; i++) {
        int x = numero[i][0];
        int y = numero[i][1];
        int indice = obterIndice(x, y);
        npSetLED(indice, r, g, b);
    }
    npWrite();
}



void atualizar_matriz_leds(char c) {
    if (c >= '0' && c <= '9') {
        printf("Exibindo %c na matriz de leds\n", c);
       imprimir_numero(numeros[c - '0'].numero, numeros[c - '0'].tamanho, 128, 0, 0);
        // Adicionar controle da matriz WS2812
    }
}