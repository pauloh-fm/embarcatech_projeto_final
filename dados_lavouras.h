#ifndef DADOS_LAVOURAS_H
#define DADOS_LAVOURAS_H

#include <stdint.h>
#include <stddef.h>

#define LED_COUNT 25

// Variáveis globais (declaradas com extern)
extern uint8_t ANIMAIS_DETECTADOS[4];         // 0 = sem animal, 1 = animal detectado (por gaiola)
extern uint8_t atual_animais_detectados;       // Índice para atualização cíclica (0 a 3)
extern uint8_t gaiola_atual;                   // Gaiola atualmente selecionada (1 a 4)
extern uint8_t estado_portao[4];               // Estado do portão de cada gaiola: 0 = aberto, 1 = fechado

// Estrutura que define um elemento da matriz: coordenadas e função
typedef struct {
    uint8_t x;
    uint8_t y;
    char tipo; // 'C' = Cerca, 'G' = Gaiola, 'T' = Sensor, ' ' = Off, 'P' = Portão, 'A' = Detecção animal
} led_def_t;

// Layout da lavoura (matriz 5x5)
static const led_def_t lavoura_leds[LED_COUNT] = {
    {0, 0, 'C'}, {1, 0, 'C'}, {2, 1, 'G'}, {3, 0, 'C'}, {4, 0, 'C'},
    {0, 1, 'C'}, {1, 1, ' '}, {2, 1, ' '}, {3, 1, ' '}, {4, 1, 'C'},
    {0, 2, 'G'}, {1, 2, ' '}, {2, 2, 'T'}, {3, 2, ' '}, {4, 2, 'G'},
    {0, 3, 'C'}, {1, 3, ' '}, {2, 3, ' '}, {3, 3, ' '}, {4, 3, 'C'},
    {0, 4, 'C'}, {1, 4, 'C'}, {2, 4, 'G'}, {3, 4, 'C'}, {4, 4, 'C'}
};

// Layout da gaiola (matriz 5x5)
static const led_def_t gaiola_leds[LED_COUNT] = {
    {0, 0, 'C'}, {1, 0, 'P'}, {2, 1, 'P'}, {3, 0, 'P'}, {4, 0, 'C'},
    {0, 1, 'C'}, {1, 1, ' '}, {2, 1, ' '}, {3, 1, ' '}, {4, 1, 'C'},
    {0, 2, 'C'}, {1, 2, ' '}, {2, 2, 'A'}, {3, 2, ' '}, {4, 2, 'C'},
    {0, 3, 'C'}, {1, 3, ' '}, {2, 3, ' '}, {3, 3, ' '}, {4, 3, 'C'},
    {0, 4, 'C'}, {1, 4, 'C'}, {2, 4, 'C'}, {3, 4, 'C'}, {4, 4, 'C'}
};

#endif // DADOS_LAVOURAS_H
