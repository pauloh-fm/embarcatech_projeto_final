#include "dados_lavouras.h"

// Definição única das variáveis globais:
uint8_t ANIMAIS_DETECTADOS[4] = {0, 0, 0, 0};
uint8_t atual_animais_detectados = 0;
uint8_t gaiola_atual = 1;
uint8_t estado_portao[4] = {0, 0, 0, 0};  // Inicialmente, todos abertos (0)
