# BitDogLab-IRQ-WS2812
Controle de LEDs e Interrupções no RP2040


Este projeto demonstra o uso de interrupções no microcontrolador RP2040 para controle eficiente de botões e LEDs. Utilizando a placa BitDogLab, exploramos conceitos essenciais como debouncing de botões via software, controle de LEDs WS2812 (matriz 5x5) e uso de resistores de pull-up internos.

⚙️ Funcionalidades Implementadas
✅ LED vermelho piscando 5 vezes por segundo.
✅ Botão A: incrementa o número exibido na matriz de LEDs.
✅ Botão B: decrementa o número exibido na matriz de LEDs.
✅ Exibição de números de 0 a 9 na matriz WS2812 com efeitos visuais.

📌 Tecnologias e Conceitos Utilizados
🔹 Interrupções externas (IRQ) para resposta rápida aos botões.
🔹 Debouncing via software para evitar leituras incorretas.
🔹 Controle de LEDs comuns e endereçáveis.
🔹 Uso eficiente dos resistores de pull-up internos.

📂 Organização do Código
O código está estruturado e comentado para facilitar a compreensão e reutilização.

🚀 Demonstração
Além do código-fonte, será disponibilizado um vídeo demonstrando o funcionamento do projeto na placa BitDogLab.

📌 Requisitos

Placa de desenvolvimento BitDogLab (RP2040).
LEDs WS2812 (Matriz 5x5).
LED RGB (GPIOs 11, 12, 13).
Botões para interação (GPIOs 5 e 6).
🔗 Como Usar
Clone o repositório, faça o upload do código para a placa e teste as funcionalidades! 🎯
