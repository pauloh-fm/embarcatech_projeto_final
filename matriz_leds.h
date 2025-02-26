#define LED_COUNT 25
#define LED_PIN_WS2812 7

void npInit(uint pin);
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b);
void npClear();
void npWrite();
int obterIndice(uint x, uint y);
void exibirGaiola();
void exibirLavoura();