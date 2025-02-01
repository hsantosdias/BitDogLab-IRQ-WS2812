#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

struct ButtonPins {
    uint button_a;
    uint button_b;
};

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

struct ButtonPins button_pins = {5, 6};
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
    
    gpio_init(led_pins.red);
    gpio_set_dir(led_pins.red, GPIO_OUT);
   
    // Configuração dos pinos GPIO
    gpio_init(led_pins.green);
    gpio_init(led_pins.blue);

    gpio_set_dir(led_pins.green, GPIO_OUT);
    gpio_set_dir(led_pins.blue, GPIO_OUT);
   
    struct repeating_timer timer;
    add_repeating_timer_ms(1000, repeating_timer_callback, NULL, &timer);
    
    while (true) {
        sleep_ms(10000);
        printf("Rotina de repetição\n");
    }
    
    return 0;
}
