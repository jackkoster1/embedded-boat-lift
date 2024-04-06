#include <stdio.h>
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

enum{
    MOTOR_DOWN,
    MOTOR_UP,
    MOTOR_STOP
};
#define GPIO_TOO_HIGH 0
#define GPIO_TOO_LOW 1

#define GPIO_MOTOR1_EN 13
#define GPIO_MOTOR1_IN1 14
#define GPIO_MOTOR1_IN2 15
#define GPIO_MOTOR2_EN 18
#define GPIO_MOTOR2_IN1 17
#define GPIO_MOTOR2_IN2 16

#define GPIO_ENCODER1_IN1 6
#define GPIO_ENCODER1_IN2 7
#define GPIO_ENCODER2_IN1 8
#define GPIO_ENCODER2_IN2 9

#define GPIO_ETHERNET_SDA 2
#define GPIO_ETHERNET_SCL 3

#define GPIO_INTERNET_SDA 4
#define GPIO_INTERNET_SCL 5

#define GPIO_MOVE_UP 21
#define GPIO_MOVE_DOWN 22


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
    gpio_init(GPIO_MOTOR1_IN1);
    gpio_init(GPIO_MOTOR1_IN2);
    gpio_set_dir(GPIO_MOTOR1_EN,GPIO_OUT);
    gpio_set_dir(GPIO_MOTOR1_IN1,GPIO_OUT);
    gpio_set_dir(GPIO_MOTOR1_IN2,GPIO_OUT);
    //motor2 
    gpio_init(GPIO_MOTOR2_EN); 
    gpio_init(GPIO_MOTOR2_IN1);
    gpio_init(GPIO_MOTOR2_IN2);
    gpio_set_dir(GPIO_MOTOR2_EN,GPIO_OUT);
    gpio_set_dir(GPIO_MOTOR2_IN1,GPIO_OUT);
    gpio_set_dir(GPIO_MOTOR2_IN2,GPIO_OUT);
}
void init_encoders()
{
    //encoder1 TODO
    //encoder2 TODO

}
void init_limit_switches()
{
    //TODO set up interupt

}

void init_move_switches()
{
    //these are to move boat up or down set GPIO to in to read if high or low TODO
}
void read_encoder()
{
    //TODO
}

int move_motors_buttons()
{
    //TODO
    //move based on button
    return MOTOR_STOP;
}
void motor_up()
{
    //check limit switches
    //check phase of encoders 
    //if one is higher than the other stop higher motor to allow to catch up
    //

}
void motor_down()
{
    //check limit switches
    //check phase of encoders 
    //if one is higher than the other stop lower motor to allow to catch up
    //
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
    
    init_motors();
    init_limit_switches();
    init_encoders();
    init_move_switches();

    //gpio_init(6);
    //gpio_init(7);
    //gpio_set_dir(6,GPIO_IN);
    //gpio_set_dir(7,GPIO_IN);
    //gpio_pull_down(0);
    //gpio_set_function(0,GPIO_FUNC_XIP);
    //gpio_set_pulls(0,1,0);//pull up
    //gpio_set_irqover
    //gpio_set_irqenable
    //gpio_set_irq_enabled_with_callback(0, GPIO_IRQ_EDGE_RISE, true, &too_high_handler); //this sets up interups 
    //multithread to networking TODO
    while(1)
    {
        switch(move_motors_buttons())
        {
            case MOTOR_DOWN:
            move_down();
            break;
            case MOTOR_UP:
            move_up();
            break;
            case MOTOR_STOP:
            gpio_put(GPIO_MOTOR1_EN,0);
            gpio_put(GPIO_MOTOR2_EN,0);
            break;
        }
        //check up/down move switches and move accordenly with if statements TODO


        //check networking request buffer and move TODO
        
    }
}