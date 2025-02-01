#include <stdio.h>
#include "pico/stdlib.h"


int gpio_pin = 13; // Led vermelho

int main () {
	
	
while (true) {
    
    gpio_init(gpio_pin);
    gpio_set_dir(gpio_pin, GPIO_OUT);
    gpio_put(gpio_pin, 1);
    sleep_ms(500);
    gpio_put(gpio_pin, 0);
    sleep_ms(500);
    
}


return 0;

	
	
	
	
}