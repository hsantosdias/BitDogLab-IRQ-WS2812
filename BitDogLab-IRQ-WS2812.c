#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define BUTTON_PIN_A 5
#define BUTTON_PIN_B 6
#define LED_PIN_RED 13 // Definição do pino do LED vermelho

// Função para atualizar os estados dos LEDs
void set_led_color(uint red_pin, uint green_pin, uint blue_pin, bool R, bool G, bool B) {
    gpio_put(red_pin, R);   // Configura o estado do LED vermelho
    gpio_put(green_pin, G); // Configura o estado do LED verde
    gpio_put(blue_pin, B);  // Configura o estado do LED azul
}

bool led_on = false;

bool repeating_timer_callback(struct repeating_timer *t) {
    printf("1 segundo passou\n");
    led_on = !led_on;
    gpio_put(LED_PIN_RED, led_on);
    return true;
}

int main() {
    stdio_init_all();
    
    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);
   
    // Configuração dos pinos GPIO
    const uint red_pin = 13;   // Pino para o LED vermelho
    const uint green_pin = 11; // Pino para o LED verde
    const uint blue_pin = 12;  // Pino para o LED azul

    gpio_init(red_pin);
    gpio_init(green_pin);
    gpio_init(blue_pin);

    gpio_set_dir(red_pin, GPIO_OUT);
    gpio_set_dir(green_pin, GPIO_OUT);
    gpio_set_dir(blue_pin, GPIO_OUT);
   
    struct repeating_timer timer;
    add_repeating_timer_ms(1000, repeating_timer_callback, NULL, &timer);
    
    while (true) {
        sleep_ms(10000);
        printf("Rotina de repetição\n");
    }
    
    return 0;
}
