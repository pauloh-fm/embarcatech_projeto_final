# Sistema Microcontrolado de Controle e Monitoramento de Lavouras

**Autor:** Paulo Henrique de Farias Martins

## Funcionalidades

✅ **Detecção de Animais nas Gaiolas via Sensores Simulados**  
- Sensores de movimento (botões) simulam a detecção de animais em quatro gaiolas.  
- Cada detecção atualiza um vetor global, permitindo visualizar se há animal presente em cada gaiola.

✅ **Controle do Portão das Gaiolas via Joystick**  
- Movimentos do joystick para a direita fecham o portão (estado fechado indicado por LED azul).  
- Movimentos para a esquerda abrem o portão (estado aberto).  
- O estado de cada portão é armazenado e exibido na matriz de LEDs.

✅ **Exibição em Matriz de LEDs**  
- Uma matriz de LEDs WS2818B (5x5) exibe, de forma gráfica, os estados da lavoura.  
- Cada LED representa uma função (cerca, gaiola, sensor, portão), com cores indicativas:  
  - Verde para a cerca;  
  - Azul para a gaiola em estado normal ou portão fechado;  
  - Vermelho para detecção de animal;  
  - Amarelo para o sensor de temperatura e umidade.

✅ **Display OLED I2C para Monitoramento**  
- O display OLED exibe informações gráficas sobre o status de detecção de animais em cada uma das quatro gaiolas.  
- O monitoramento é atualizado em tempo real, permitindo o acompanhamento do sistema.

✅ **Implementação de Interrupções (IRQ) e Debounce**  
- O firmware utiliza interrupções para os botões, garantindo respostas rápidas e evitando travamentos.  
- Foi implementado debounce por software para prevenir acionamentos múltiplos indesejados.

✅ **Comunicação via USB Serial para Debug e Monitoramento**  
- Os dados de controle e debug são enviados via USB serial, facilitando o monitoramento local do sistema.

## Instruções

Siga estes 3 passos simples para executar o projeto:

1. **Clonar o Projeto:**  
   Clone este repositório para sua máquina local:
   ```bash
   git clone https://github.com/pauloh-fm/embarcatech_projeto_final.git
