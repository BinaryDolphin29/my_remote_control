#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define T 500
#define LENGTH 85

const int offOn[LENGTH] = { 4, 2, 11, 2, 3, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 3, 1, 3, 1, 1, 1, 3, 1, 3, 1, 1 };
const int brighten[LENGTH] = { 4, 2, 11, 2, 3, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 3, 1, 1, 1, 1, 1, 3, 1, 1 };
const int darken[LENGTH] = { 4, 2, 11, 2, 3, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 3, 1, 1, 1, 3, 1, 1, 1, 1 };

void IR_ON(int t) {
    pwm_set_gpio_level(0, 219);
    sleep_us(T * t);
}

void IR_OFF(int t) {
    pwm_set_gpio_level(0, 0);
    sleep_us(T * t);
}

void transmit(const int* d) {
    for (size_t i = 0; i < LENGTH; i++) {
        if (i % 2 == 0) {
            IR_ON(d[i]);
        } else {
            IR_OFF(d[i]);
        }
    }

    pwm_set_gpio_level(0, 0);
}

int main(void) {
    gpio_set_function(0, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(0);

    pwm_set_clkdiv(slice_num, 5);
    pwm_set_wrap(slice_num, 658);
    pwm_set_gpio_level(0, 219);
    pwm_set_enabled(slice_num, true);

    transmit(offOn);
    sleep_ms(500);
    transmit(offOn);

    return 0;
}