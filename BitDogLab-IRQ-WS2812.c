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

struct ButtonPins {
    uint button_a;
    uint button_b;
};

// Instância da estrutura
struct ButtonPins button_pins = {5, 6};

// Variável para armazenar o número exibido
volatile int numero_exibido = 0;

// Prototótipos das funções
void gpio_irq_handler(uint gpio, uint32_t events);
void exibir_numero(int numero);



// Função de interrupção para os botões
void gpio_irq_handler(uint gpio, uint32_t events) {
    sleep_ms(50); // Debounce simples
    if (gpio == button_pins.button_a) {
        numero_exibido = (numero_exibido + 1) % 10; // Incrementa e mantém no intervalo 0-9
    } else if (gpio == button_pins.button_b) {
        numero_exibido = (numero_exibido - 1 + 10) % 10; // Decrementa e mantém no intervalo 0-9
    }
    exibir_numero(numero_exibido); // Mostra o número na saída serial
}

// Função para exibir o número na saída serial
void exibir_numero(int numero) {
    printf("Número exibido: %d\n", numero);
}


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


struct LEDPins led_pins = {13, 11, 12};
struct MatrixLedWS2812 matrix_leds[5][5];

// Função para atualizar os estados dos LEDs
void set_led_color(struct LEDPins leds, bool R, bool G, bool B) {
    gpio_put(leds.red, R);   // Configura o estado do LED vermelho
    gpio_put(leds.green, G); // Configura o estado do LED verde
    gpio_put(leds.blue, B);  // Configura o estado do LED azul
}

bool led_on = false;

bool repeating_timer_callback(struct repeating_timer *t) {
    printf("1 segundo passou\n");
    led_on = !led_on;
    gpio_put(led_pins.red, led_on);
    return true;
}

int main() {
    stdio_init_all();

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
        sleep_ms(1000); // Mantém o loop principal rodando
        printf("Rotina de repetição\n");
    }
    return 0;
}


/*
int main() {
    stdio_init_all();
    
    gpio_init(led_pins.red);
    gpio_set_dir(led_pins.red, GPIO_OUT);
   
    // Configuração dos pinos GPIO
    gpio_init(led_pins.green);
    gpio_init(led_pins.blue);

    gpio_set_dir(led_pins.green, GPIO_OUT);
    gpio_set_dir(led_pins.blue, GPIO_OUT);
   
    // Inicializa a máquina PIO
    // 5 frames em 1 segundo
    // Inicializa a matriz de LEDs neoPixel
    //npInit(MATRIX_LED_PIN);
    // Limpa a matriz de LEDs
    //npClear();
    //animacao5frames1seg();

    struct repeating_timer timer;
    add_repeating_timer_ms(1000, repeating_timer_callback, NULL, &timer);
    
    while (true) {


        sleep_ms(10000);
        printf("Rotina de repetição\n");
    }
    
    return 0;
}

*/