#include <stdio.h>
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#define GPIO_TOO_HIGH 0
#define GPIO_MOTOR1_EN 13
#define GPIO_MOTOR1_in1 14
#define GPIO_MOTOR1_in2 15
//TODO define all below
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
gpio3 = move up
gpio4 = move down

*/


void too_high_handler(uint callb,uint32_t events){
    //TODO set motor enables to off and flag the too high var
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
}
void too_low_handler(uint callb,uint32_t events){
    //TODO set motor enables to off and flag the too low var
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
}

void init_motors()
{
    // motor1
    gpio_init(GPIO_MOTOR1_EN); 
    gpio_init(GPIO_MOTOR1_in1);
    gpio_init(GPIO_MOTOR1_in2);
    gpio_set_dir(GPIO_MOTOR1_EN,GPIO_OUT);
    gpio_set_dir(GPIO_MOTOR1_in1,GPIO_OUT);
    gpio_set_dir(GPIO_MOTOR1_in2,GPIO_OUT);
    //motor2 TODO
}
void init_encoders()
{
    //encoder1 TODO
    //encoder2 TODO

}
void init_too_high_switches()
{
    //TODO set up interupt

}
void init_too_low_switches()
{
    //TODO set up interupt   
}
void init_move_switches
{
    //these are to move boat up or down set GPIO to in to read if high or low TODO
}

int main()
{
    stdio_init_all();

    if (cyw43_arch_init())
    {
        printf("WiFi init failed");
        return -1;
    }
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    //
    gpio_init(0);
    init_motors();
    
    //TURN MOTOR
    gpio_put(13,1); //MOTOR EN
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
    //multithread to networking TODO
    while(1)
    {
        //check up/down move switches and move accordenly with if statements TODO


        //check networking request buffer and move TODO
        
    }
}