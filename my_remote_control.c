#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"

#define duration 362
#define T 500
#define LENGTH 85

// light_switch: turn off/on, nightlight
const uint8_t light_switch[LENGTH] = {
    4, 2, 11, 2, 3, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 3, 1, 3, 1, 1, 1, 3, 1, 3, 1, 1
};
/*
const uint8_t light_brighten[LENGTH] = {
    4, 2, 11, 2, 3, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 3, 1, 1, 1, 1, 1, 3, 1, 1
};
const uint8_t light_darken[LENGTH] = {
    4, 2, 11, 2, 3, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 3, 1, 1, 1, 3, 1, 1, 1, 1
};
*/

volatile bool pushed = false;

void IR_ON(uint8_t t) {
    pwm_set_gpio_level(0, 219);
    sleep_us(T * t);
}

void IR_OFF(uint8_t t) {
    pwm_set_gpio_level(0, 0);
    sleep_us(T * t);
}

void transmit(const uint8_t* d) {
    for (uint8_t i = 0; i < LENGTH; i++) {
        if (i % 2 == 0) {
            IR_ON(d[i]);
        } else {
            IR_OFF(d[i]);
        }
    }

    pwm_set_gpio_level(0, 0);
}

void irq_callback(uint gpio, uint32_t events) {
    pushed = true;
}

int main(void) {
    gpio_init(3);
    gpio_set_dir(3, GPIO_OUT);

    gpio_set_function(0, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(0);

    pwm_set_clkdiv(slice_num, 5);
    pwm_set_wrap(slice_num, 658);
    pwm_set_enabled(slice_num, true);
    pwm_set_gpio_level(0, 0);

    gpio_set_irq_enabled_with_callback(2, GPIO_IRQ_EDGE_FALL, true, &irq_callback);

    while (1) {
        if (pushed) {
            transmit(light_switch);
            sleep_ms(duration);
            transmit(light_switch);

            pushed = false;
        }
    };

    return 0;
}