#include <stdio.h>
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
void too_high_handler(uint callb,uint32_t events){
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
}
void too_low_handler(uint callb,uint32_t events){
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
}
/*
gpio0 = too high
gpio1 = too low
gpio13 = motor1 enable
gpio14 = motor1 in 1
gpio15 = motor1 in 2
gpio18 = motor1 enable
gpio17 = motor1 in 1
gpio16 = motor1 in 2
gpio6 = encoder1 1
gpio7 = encoder1 2
gpio8 = encoder2 1
gpio9 = encoder2 2
gpio20 = sda i2c to ethernet
gpio21 = scl i2c to ethernet

*/
int main()
{
    stdio_init_all();

    if (cyw43_arch_init())
    {
        printf("WiFi init failed");
        return -1;
    }
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    //run motor
    gpio_init(0);
    gpio_init(13);
    gpio_init(14);
    gpio_init(15);
    
    gpio_set_dir(13,GPIO_OUT);
    gpio_set_dir(14,GPIO_OUT);
    gpio_set_dir(15,GPIO_OUT);
    gpio_put(13,1);
    gpio_put(14,1);
    gpio_put(15,0);

    gpio_init(6);
    gpio_init(7);
    gpio_set_dir(6,GPIO_IN);
    gpio_set_dir(7,GPIO_IN);
    //gpio_pull_down(0);
    //gpio_set_function(0,GPIO_FUNC_XIP);
    //gpio_set_pulls(0,1,0);//pull up
    //gpio_set_irqover
    //gpio_set_irqenable
    gpio_set_irq_enabled_with_callback(0, GPIO_IRQ_EDGE_RISE, true, &too_high_handler);
    while(1)
    {
        sleep_ms(500);
        
    }
}