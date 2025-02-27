#include "pico/stdlib.h"
#include "matriz_leds.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> // Necessário para toupper()
#include "botao.h"
// Variável global para indicar qual gaiola está sendo exibida (1 a 4)
uint8_t gaiola_atual = 1;

// Função auxiliar para remover espaços em branco no início e fim da string
void trim(char *str)
{
    // Remove espaços iniciais
    char *start = str;
    while (isspace((unsigned char)*start))
        start++;
    if (start != str)
    {
        memmove(str, start, strlen(start) + 1);
    }
    // Remove espaços finais
    char *end = str + strlen(str) - 1;
    while (end >= str && isspace((unsigned char)*end))
    {
        *end = '\0';
        end--;
    }
}

int main(void)
{
    stdio_init_all();

    // Aguarda até que a conexão USB esteja estabelecida
    while (!stdio_usb_connected())
    {
        sleep_ms(100);
    }

    // Exibe o menu numérico no serial
    printf("Menu:\n");
    printf("1 - Exibir Lavoura\n");
    printf("2 - Exibir opcoes da Gaiola\n");
    fflush(stdout);
    init_botoes();
    npInit(LED_PIN_WS2812);
    // // interrupções para o sensor PIR ( DENTRO DA FUNÇÃO BOTAO.C)
    // gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, gpio_callback);
    // gpio_set_irq_enabled(BOTAO_B, GPIO_IRQ_EDGE_FALL, true);

    char input_char;

    while (true)
    {
        // O espaço antes de %c descarta eventuais espaços em branco
        if (scanf(" %c", &input_char) == 1)
        {
            printf("Recebido: %c\n", input_char);
            fflush(stdout);

            if (input_char == '1')
            {
                exibirLavoura();
                printf("Exibindo lavoura...\n");
                fflush(stdout);
            }
            else if (input_char == '2')
            {
                char num_char;
                printf("Digite o numero da gaiola (1-4): ");
                fflush(stdout);
                if (scanf(" %c", &num_char) == 1)
                {
                    int num = num_char - '0'; // Converte caractere para número
                    if (num >= 1 && num <= 4)
                    {
                        gaiola_atual = (uint8_t)num;
                        exibirGaiola();
                        printf("Exibindo Gaiola %d\n", gaiola_atual);
                        fflush(stdout);
                    }
                    else
                    {
                        printf("Numero invalido. Tente novamente.\n");
                        fflush(stdout);
                    }
                }
            }
            else
            {
                printf("Opcao invalida. Digite 1 para Lavoura ou 2 para Gaiola.\n");
                fflush(stdout);
            }
        }
        sleep_ms(100);
    }

    return 0;
}
