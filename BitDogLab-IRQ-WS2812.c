#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "pio_config.h"
#include "pico/bootrom.h"

// Biblioteca gerada pelo arquivo ws2818b.pio 
#include "ws2818b.pio.h"

// Definições gerais do projeto
#include "libs/definicoes.h"

//Reeaproveitamento de codigo Hugo S. Dias
#include "libs/animacao_hugo.c"

#define TEMPO 2500

struct ButtonPins {
    uint button_a;
    uint button_b;
};

// Instância da estrutura
struct ButtonPins button_pins = {5, 6};

struct LEDPins {
    uint red;
    uint green;
    uint blue;
};

struct MatrixLedWS2812 {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

// Instância dos LEDs
struct LEDPins led_pins = {13, 11, 12};
struct MatrixLedWS2812 matrix_leds[5][5];

// Variável para armazenar o número exibido
volatile int numero_exibido = 0;
static volatile uint a = 1;
static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)
bool led_on = false;

// Prototótipos das funções
void gpio_irq_handler(uint gpio, uint32_t events);
void exibir_numero(int numero);
void set_led_color(struct LEDPins leds, bool R, bool G, bool B);
bool repeating_timer_callback(struct repeating_timer *t);

// Função de interrupção para os botões
void gpio_irq_handler(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    printf("A = %d\n", a);
    
    if (current_time - last_time > 200000) { // 200 ms de debouncing
        last_time = current_time;
        printf("Mudanca de Estado do Led. A = %d\n", a);
        gpio_put(led_pins.red, !gpio_get(led_pins.red));
        a++;
    }
    
    if (gpio == button_pins.button_a) {
        numero_exibido = (numero_exibido + 1) % 10;
    } else if (gpio == button_pins.button_b) {
        numero_exibido = (numero_exibido - 1 + 10) % 10;
    }
    
    exibir_numero(numero_exibido);
}

// Função para exibir o número na saída serial
void exibir_numero(int numero) {
    printf("Número exibido: %d\n", numero);
}

/*
void exibir_numero_matriz(int numero) {
    const uint8_t numeros[10][5][5] = {
        // 0
        {{1, 1, 1, 1, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 1, 1, 1, 1}},
        // 1
        {{0, 0, 1, 0, 0},
         {0, 1, 1, 0, 0},
         {1, 0, 1, 0, 0},
         {0, 0, 1, 0, 0},
         {1, 1, 1, 1, 1}},
        // 2
        {{1, 1, 1, 1, 1},
         {0, 0, 0, 0, 1},
         {1, 1, 1, 1, 1},
         {1, 0, 0, 0, 0},
         {1, 1, 1, 1, 1}},
        // 3
        {{1, 1, 1, 1, 1},
         {0, 0, 0, 0, 1},
         {0, 1, 1, 1, 1},
         {0, 0, 0, 0, 1},
         {1, 1, 1, 1, 1}},
        // 4
        {{1, 0, 0, 1, 1},
         {1, 0, 0, 1, 1},
         {1, 1, 1, 1, 1},
         {0, 0, 0, 1, 1},
         {0, 0, 0, 1, 1}},
        // 5
        {{1, 1, 1, 1, 1},
         {1, 0, 0, 0, 0},
         {1, 1, 1, 1, 1},
         {0, 0, 0, 0, 1},
         {1, 1, 1, 1, 1}},
        // 6
        {{1, 1, 1, 1, 1},
         {1, 0, 0, 0, 0},
         {1, 1, 1, 1, 1},
         {1, 0, 0, 0, 1},
         {1, 1, 1, 1, 1}},
        // 7
        {{1, 1, 1, 1, 1},
         {0, 0, 0, 0, 1},
         {0, 0, 0, 1, 1},
         {0, 0, 1, 1, 1},
         {0, 0, 1, 1, 1}},
        // 8
        {{1, 1, 1, 1, 1},
         {1, 0, 0, 0, 1},
         {1, 1, 1, 1, 1},
         {1, 0, 0, 0, 1},
         {1, 1, 1, 1, 1}},
        // 9
        {{1, 1, 1, 1, 1},
         {1, 0, 0, 0, 1},
         {1, 1, 1, 1, 1},
         {0, 0, 0, 0, 1},
         {1, 1, 1, 1, 1}}
    };
    
    printf("Número na matriz 5x5:\n");
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            printf("%d ", numeros[numero][i][j]);
        }
        printf("\n");
    }
}

*/
void exibir_numero_matriz(int numero) {
    const uint8_t numeros[10][5][5] = {
        {{1, 1, 1, 1, 1}, {1, 0, 0, 0, 1}, {1, 0, 0, 0, 1}, {1, 0, 0, 0, 1}, {1, 1, 1, 1, 1}},
        {{0, 0, 1, 0, 0}, {0, 1, 1, 0, 0}, {1, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {1, 1, 1, 1, 1}},
        {{1, 1, 1, 1, 1}, {0, 0, 0, 0, 1}, {1, 1, 1, 1, 1}, {1, 0, 0, 0, 0}, {1, 1, 1, 1, 1}},
        {{1, 1, 1, 1, 1}, {0, 0, 0, 0, 1}, {0, 1, 1, 1, 1}, {0, 0, 0, 0, 1}, {1, 1, 1, 1, 1}},
        {{1, 0, 0, 1, 1}, {1, 0, 0, 1, 1}, {1, 1, 1, 1, 1}, {0, 0, 0, 1, 1}, {0, 0, 0, 1, 1}},
        {{1, 1, 1, 1, 1}, {1, 0, 0, 0, 0}, {1, 1, 1, 1, 1}, {0, 0, 0, 0, 1}, {1, 1, 1, 1, 1}},
        {{1, 1, 1, 1, 1}, {1, 0, 0, 0, 0}, {1, 1, 1, 1, 1}, {1, 0, 0, 0, 1}, {1, 1, 1, 1, 1}},
        {{1, 1, 1, 1, 1}, {0, 0, 0, 0, 1}, {0, 0, 0, 1, 1}, {0, 0, 1, 1, 1}, {0, 0, 1, 1, 1}},
        {{1, 1, 1, 1, 1}, {1, 0, 0, 0, 1}, {1, 1, 1, 1, 1}, {1, 0, 0, 0, 1}, {1, 1, 1, 1, 1}},
        {{1, 1, 1, 1, 1}, {1, 0, 0, 0, 1}, {1, 1, 1, 1, 1}, {0, 0, 0, 0, 1}, {1, 1, 1, 1, 1}}
    };
    
    printf("Número na matriz 5x5:\n");
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            matrix_leds[i][j].red = numeros[numero][i][j] ? 255 : 0;
            printf("%d ", numeros[numero][i][j]);
        }
        printf("\n");
    }
}



// Função para atualizar os estados dos LEDs
void set_led_color(struct LEDPins leds, bool R, bool G, bool B) {
    gpio_put(leds.red, R);
    gpio_put(leds.green, G);
    gpio_put(leds.blue, B);
}

// Callback para o temporizador repetitivo
bool repeating_timer_callback(struct repeating_timer *t) {
    printf("1 segundo passou\n");
    led_on = !led_on;
    gpio_put(led_pins.red, led_on);
    return true;
}

int main() {
    stdio_init_all();
    gpio_init(button_pins.button_a);
    gpio_set_dir(button_pins.button_a, GPIO_IN);
    gpio_pull_up(button_pins.button_a);
    gpio_set_irq_enabled_with_callback(button_pins.button_a, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    
    gpio_init(button_pins.button_b);
    gpio_set_dir(button_pins.button_b, GPIO_IN);
    gpio_pull_up(button_pins.button_b);
    gpio_set_irq_enabled_with_callback(button_pins.button_b, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    
    while (true) {
        exibir_numero_matriz(numero_exibido);
        sleep_ms(TEMPO);
    }
    return 0;
}


/*

int main() {
    stdio_init_all();

    // Inicializa os LEDs
    gpio_init(led_pins.red);
    gpio_set_dir(led_pins.red, GPIO_OUT);
    gpio_init(led_pins.green);
    gpio_set_dir(led_pins.green, GPIO_OUT);
    gpio_init(led_pins.blue);
    gpio_set_dir(led_pins.blue, GPIO_OUT);

    // Inicializa os botões
    gpio_init(button_pins.button_a);
    gpio_set_dir(button_pins.button_a, GPIO_IN);
    gpio_pull_up(button_pins.button_a);
    gpio_set_irq_enabled_with_callback(button_pins.button_a, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    gpio_init(button_pins.button_b);
    gpio_set_dir(button_pins.button_b, GPIO_IN);
    gpio_pull_up(button_pins.button_b);
    gpio_set_irq_enabled_with_callback(button_pins.button_b, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    while (true) {
        gpio_put(led_pins.green, true);
        sleep_ms(TEMPO);
        gpio_put(led_pins.green, false);
        sleep_ms(TEMPO);
        printf("Rotina de repetição\n");
    }
    return 0;
}


*/