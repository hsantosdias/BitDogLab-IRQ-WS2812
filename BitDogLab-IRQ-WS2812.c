#include <stdio.h> // Inclui a biblioteca padrão de entrada e saída
#include "pico/stdlib.h" // Inclui a biblioteca de funções padrão do Pico
#include "hardware/gpio.h" // Inclui a biblioteca de funções de GPIO
#include "hardware/clocks.h" // Inclui a biblioteca de funções de clock
#include "hardware/irq.h" // Inclui a biblioteca de funções de interrupção
#include "hardware/timer.h" // Inclui a biblioteca de funções de timer 
#include "ws2818b.pio.h" // Inclui a biblioteca de funções do WS2812B
#include "hardware/sync.h" // Inclui a biblioteca de funções de sincronização

#define MATRIX_LED_PIN 7  // Pino da matriz de LEDs WS2812B
#define BUTTON_PIN_A 5    // Pino do botão A 
#define BUTTON_PIN_B 6    // Pino do botão B
#define LED_RED 13        // LED RGB piscando 5 vezes por segundo e em vermelho

#define LED_COUNT 25  // Número total de LEDs

#define ROWS 5        // Linhas da matriz
#define COLS 5        // Colunas da matriz

// Variáveis globais para controle de estado
PIO np_pio;
uint sm;
volatile int numero_atual = 0; // Número atual exibido na matriz
volatile int numero_exibido = 0; // Número exibido na matriz
static absolute_time_t last_time; // Último tempo de interrupção
bool led_state = false; // Estado do LED vermelho

// Definição de pixel GRB 
typedef struct {
    uint8_t G, R, B;
} pixel_t;

// Definição de tipo de pixel para matriz de LEDs
typedef pixel_t npLED_t;
npLED_t leds[LED_COUNT];

// Mapeamento correto dos LEDs na matriz 5x5 (corrigido para garantir o mapeamento correto)
int mapearIndice(int linha, int coluna) {
    return linha * COLS + coluna;  // Ajustado para garantir o mapeamento correto
}

// Função de mapeamento de cores RGB (corrigido para GRB)
uint32_t matriz_rgb(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(g) << 16) | ((uint32_t)(r) << 8) | (uint32_t)(b);
}

// Função para definir o LED na matriz de LEDs
void npSetLED(uint index, uint8_t r, uint8_t g, uint8_t b) {
    if (index < LED_COUNT) {
        leds[index] = (npLED_t){g, r, b};
    }
}

// Função para inicializar a matriz de LEDs, vem depois da definição de npSetLED para evitar erros
void npInit(uint pin) {
    uint offset = pio_add_program(pio0, &ws2818b_program);
    if (offset == -1) {
        printf("Erro ao adicionar programa PIO\n"); // Exibe mensagem de erro na serial
        return;
    }

    np_pio = pio0;
    sm = pio_claim_unused_sm(np_pio, true);
    if (sm == -1) {
        printf("Erro ao alocar máquina de estado PIO\n"); // Exibe mensagem de erro na serial
        return;
    }

    ws2818b_program_init(np_pio, sm, offset, pin);
    for (uint i = 0; i < LED_COUNT; i++) npSetLED(i, 0, 0, 0);
}

// Função para limpar a matriz de LEDs
void npClear() {
    for (uint i = 0; i < LED_COUNT; i++) {
        npSetLED(i, 0, 0, 0);
    }
}

// Função para escrever na matriz de LEDs
void npWrite() {
    uint32_t save = save_and_disable_interrupts();  // Desativa interrupções temporariamente

    for (uint i = 0; i < LED_COUNT; i++) {
        uint32_t valor_led = matriz_rgb(leds[i].R, leds[i].G, leds[i].B);
        pio_sm_put_blocking(np_pio, sm, valor_led);
    }

    busy_wait_us(300);  // Tempo de espera mais preciso

    restore_interrupts(save);  // Reativa interrupções
}

// Função para exibir o número na matriz 5x5 de LEDs (corrigido para exibir o número correto)
void exibirNumero(int numero) {
    npClear(); // Limpa a matriz de LEDs
    static const uint numeros[10][5][5] = {
        {
        {0, 1, 1, 1, 0}, 
        {0, 1, 0, 1, 0}, 
        {0, 1, 0, 1, 0}, 
        {0, 1, 0, 1, 0}, 
        {0, 1, 1, 1, 0}}, // 0

        {
        {0, 1, 1, 1, 0}, 
        {0, 0, 1, 0, 0}, 
        {0, 0, 1, 0, 0}, 
        {0, 1, 1, 0, 0}, 
        {0, 0, 1, 0, 0}}, // 1

        {
        {0, 1, 1, 1, 0}, 
        {0, 1, 0, 0, 0}, 
        {0, 0, 1, 0, 0}, 
        {0, 1, 0, 1, 0}, 
        {0, 0, 1, 0, 0}}, // 2

        {
          {0, 1, 1, 1, 0}, 
          {0, 0, 0, 1, 0}, 
          {0, 0, 1, 0, 0}, 
          {0, 0, 0, 1, 0}, 
          {0, 1, 1, 1, 0}}, // 3
      
        {
         {0, 1, 0, 0, 0}, 
         {0, 0, 0, 1, 0}, 
         {0, 1, 1, 1, 0}, 
         {0, 1, 0, 1, 0}, 
         {0, 1, 0, 1, 0}}, // 4
      
        {
          {0, 1, 1, 1, 0}, 
          {0, 0, 0, 1, 0}, 
          {0, 1, 1, 1, 0}, 
          {0, 1, 0, 0, 0}, 
          {0, 1, 1, 1, 0}}, // 5
        {
          {0, 1, 1, 1, 0}, 
          {0, 1, 0, 1, 0}, 
          {0, 1, 1, 1, 0}, 
          {0, 1, 0, 0, 0}, 
          {0, 1, 1, 1, 0}}, // 6
        {
          {0, 1, 0, 0, 0}, 
          {0, 0, 0, 1, 0}, 
          {0, 1, 0, 0, 0}, 
          {0, 0, 0, 1, 0}, 
          {0, 1, 1, 1, 0}}, // 7
        {
          {0, 1, 1, 1, 0}, 
          {0, 1, 0, 1, 0}, 
          {0, 1, 1, 1, 0}, 
          {0, 1, 0, 1, 0}, 
          {0, 1, 1, 1, 0}}, // 8
        {
          {0, 1, 1, 1, 0}, 
          {0, 0, 0, 1, 0}, 
          {0, 1, 1, 1, 0}, 
          {0, 1, 0, 1, 0}, 
          {0, 1, 1, 1, 0}}  // 9
    };

    printf("Número: %d\n", numero);  // Exibe o número correspondente na serial

    // Exibe a matriz 5x5 na serial
    printf("Matriz 5x5:\n");
    for (uint i = 0; i < 5; i++) {
        for (uint j = 0; j < 5; j++) {
            int led_status = numeros[numero][i][j];
            if (led_status) {
                // Acende o LED correspondente
                npSetLED(mapearIndice(i, j), 255, 255, 255);  // LED branco
                // Imprime 1 para indicar o LED aceso e seus valores RGB
                printf("1(R: 255, G: 255, B: 255) ");
            } else {
                // Imprime 0 para LED apagado
                printf("0 ");
            }
        }
        printf("\n");
    }
    npWrite();
}

// Callback de interrupção corrigido para debounce de 200ms e exibição correta do número
void gpio_callback(uint gpio, uint32_t events) {
    static absolute_time_t last_time = 0;
    absolute_time_t now = get_absolute_time();
    if (absolute_time_diff_us(last_time, now) < 200000) return;  // Debounce de 200ms (200000us)
    last_time = now; // Atualiza o tempo da última interrupção

    // Atualiza o número exibido na matriz de LEDs com base no botão pressionado
    if (gpio == BUTTON_PIN_A) numero_atual = (numero_atual + 1) % 10;
    if (gpio == BUTTON_PIN_B) numero_atual = (numero_atual + 9) % 10;
    numero_exibido = numero_atual;
    printf("Número exibido: %d\n", numero_exibido); // Exibe o número exibido na serial

    // Atualiza a exibição do número na matriz de LEDs
    exibirNumero(numero_exibido); // Exibe o número na matriz de LEDs
}

// Função de interrupção para piscar LED vermelho 5 vezes por segundo
bool blink_led_callback(struct repeating_timer *t) {
    led_state = !led_state;
    gpio_put(LED_RED, led_state);
    return true;
}

// Função principal para inicialização de GPIOs, matriz de LEDs e interrupções
int main() {
    stdio_init_all();

    // Inicialização de GPIOs (sem verificações condicionais)
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    // Inicialização de botões A e B com interrupção de borda de descida e callback
    gpio_init(BUTTON_PIN_A);
    gpio_set_dir(BUTTON_PIN_A, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_A);
    gpio_set_irq_enabled_with_callback(BUTTON_PIN_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    gpio_init(BUTTON_PIN_B);
    gpio_set_dir(BUTTON_PIN_B, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_B);
    gpio_set_irq_enabled_with_callback(BUTTON_PIN_B, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    // Inicialização da matriz de LEDs e interrupção de GPIO
    npInit(MATRIX_LED_PIN);
    last_time = get_absolute_time();

    // Configurar interrupção para piscar LED vermelho 5 vezes por segundo
    struct repeating_timer timer; // Timer para piscar LED vermelho
    add_repeating_timer_ms(-200, blink_led_callback, NULL, &timer); // Piscar LED vermelho 5 vezes por segundo

    while (true) {
        sleep_ms(50);  // Apenas mantenha o loop rodando para as interrupções funcionarem corretamente
    }
    return 0;
}
