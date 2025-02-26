#define LED_COUNT 25
#define LED_PIN_WS2812 7

void npInit(uint pin);
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b);
void npClear();
void npWrite();
int obterIndice(uint x, uint y);
void imprimir_numero(const uint8_t numero[][2], size_t tamanho, uint8_t r, uint8_t g, uint8_t b);
void atualizar_matriz_leds(char c);