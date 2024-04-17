// orange buck volt = 3.3v
// #include "lwip/apps/httpd.h"
#include <stdio.h>
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwipopts.h"
// #include "ssi.h"
// #include "cgi.h"
#include "lwip/netif.h"
#include "lwip/ip4_addr.h"
#include "lwip/apps/lwiperf.h"
#include "btstack.h"

// const char WIFI_SSID = "WIFISSIDHERE";
// const char WIFI_PASS = "WIFIPASSHERE";

enum
{
    MOTOR_DOWN,
    MOTOR_UP,
    MOTOR_STOP
};
#define GPIO_TOO_HIGH 0
#define GPIO_TOO_LOW 1

#define LED_RED 11
#define LED_YLW 10

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

#define MAX_ENCODER_C 15000
#define MIN_ENCODER_C 0

int16_t encoder1_c = 0; // how many phases from too high limit switch
int16_t encoder2_c = 0;
static bool too_high = false;
static bool too_low = false;
void too_high_handler()
{
    // TODO set motor enables to off and flag the too high var
    gpio_put(GPIO_MOTOR1_EN, 0);
    gpio_put(GPIO_MOTOR2_EN, 0);
    too_high = true;
}
void too_low_handler()
{
    // TODO set motor enables to off and flag the too low var
    gpio_put(GPIO_MOTOR1_EN, 0);
    gpio_put(GPIO_MOTOR2_EN, 0);
    too_low = true;
}

void encoder1_handler()
{
    bool read = gpio_get(GPIO_ENCODER1_IN2);
    if (read) // platform moving down
    {
        encoder1_c++;
    }
    else
    {
        encoder1_c--;
    }
    if (encoder1_c < 600)
    {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    }
}

void encoder2_handler()
{

}

void gpio_handler(uint gpio, uint32_t events)
{
    if (gpio == GPIO_TOO_HIGH)
    {
        too_high_handler();
    }
    if (gpio == GPIO_TOO_LOW)
    {
        too_low_handler();
    }
    if (gpio == GPIO_ENCODER1_IN1)
    {
        encoder1_handler();
    }
    if (gpio == GPIO_ENCODER2_IN1)
    {
        encoder2_handler();
    }
}

void init_motors()
{
    // motor1
    gpio_init(GPIO_MOTOR1_EN);
    gpio_init(GPIO_MOTOR1_IN1);
    gpio_init(GPIO_MOTOR1_IN2);
    gpio_set_dir(GPIO_MOTOR1_EN, GPIO_OUT);
    gpio_set_dir(GPIO_MOTOR1_IN1, GPIO_OUT);
    gpio_set_dir(GPIO_MOTOR1_IN2, GPIO_OUT);
    // motor2
    gpio_init(GPIO_MOTOR2_EN);
    gpio_init(GPIO_MOTOR2_IN1);
    gpio_init(GPIO_MOTOR2_IN2);
    gpio_set_dir(GPIO_MOTOR2_EN, GPIO_OUT);
    gpio_set_dir(GPIO_MOTOR2_IN1, GPIO_OUT);
    gpio_set_dir(GPIO_MOTOR2_IN2, GPIO_OUT);
}

void init_encoders()
{

    //gpio_set_irq_enabled_with_callback(GPIO_ENCODER1_IN1, GPIO_IRQ_EDGE_FALL, true, &encoder1_handler); // this sets up interups
    //gpio_set_irq_enabled_with_callback(GPIO_ENCODER2_IN1, GPIO_IRQ_EDGE_FALL, true, &encoder2_handler);
    // encoder1 TODO
    // encoder2 TODO
    // gpio_init(LED_RED);
    // gpio_init(LED_YLW);
    // gpio_set_dir(LED_RED, GPIO_OUT);
    // gpio_set_dir(LED_YLW, GPIO_OUT);
}

void init_limit_switches()
{
    gpio_init(GPIO_TOO_LOW);
    gpio_init(GPIO_TOO_HIGH);
    gpio_pull_down(GPIO_TOO_HIGH);
    gpio_pull_down(GPIO_TOO_LOW);
    gpio_init(GPIO_ENCODER1_IN1);
    gpio_init(GPIO_ENCODER1_IN2);
    gpio_pull_up(GPIO_ENCODER1_IN1);
    gpio_pull_up(GPIO_ENCODER1_IN2);
    gpio_set_irq_enabled_with_callback(GPIO_TOO_LOW, GPIO_IRQ_EDGE_FALL, true, &too_low_handler);   // this sets up interups
    gpio_set_irq_enabled_with_callback(GPIO_TOO_HIGH, GPIO_IRQ_EDGE_FALL, true, &too_high_handler); // this sets up interups
}

void init_inputs()
{
    gpio_init(GPIO_TOO_LOW);
    gpio_init(GPIO_TOO_HIGH);
    gpio_init(GPIO_ENCODER1_IN1);
    gpio_init(GPIO_ENCODER1_IN2);

    gpio_set_dir(GPIO_TOO_HIGH, GPIO_IN);
    gpio_set_dir(GPIO_TOO_LOW, GPIO_IN);
    gpio_pull_down(GPIO_TOO_HIGH);
    gpio_pull_down(GPIO_TOO_LOW);

    gpio_set_dir(GPIO_ENCODER1_IN1, GPIO_IN);
    gpio_set_dir(GPIO_ENCODER1_IN2, GPIO_IN);
    gpio_pull_up(GPIO_ENCODER1_IN1);
    gpio_pull_up(GPIO_ENCODER1_IN2);

    gpio_set_irq_enabled_with_callback(GPIO_TOO_HIGH, GPIO_IRQ_EDGE_RISE, true, &gpio_handler);
    gpio_set_irq_enabled(GPIO_TOO_LOW, GPIO_IRQ_EDGE_RISE, true);
    gpio_set_irq_enabled(GPIO_ENCODER1_IN1, GPIO_IRQ_EDGE_FALL, true);

    // gpio_set_irq_enabled_with_callback(GPIO_TOO_LOW, GPIO_IRQ_EDGE_RISE, true, &too_low_handler);       // this sets up interups
    // gpio_set_irq_enabled_with_callback(GPIO_TOO_HIGH, GPIO_IRQ_EDGE_RISE, true, &too_high_handler);     // this sets up interups
    // gpio_set_irq_enabled_with_callback(GPIO_ENCODER1_IN1, GPIO_IRQ_EDGE_FALL, true, &encoder1_handler); // this sets up interups
}

void init_motor_buttons()
{
    // these are to move boat up or down set GPIO to in to read if high or low TODO
    gpio_init(GPIO_MOVE_DOWN);
    gpio_init(GPIO_MOVE_UP);
    gpio_set_dir(GPIO_MOVE_DOWN, GPIO_IN);
    gpio_set_dir(GPIO_MOVE_UP, GPIO_IN);
}
void read_encoder()
{
    // TODO
}

int move_motors_buttons()
{
    bool down = gpio_get(GPIO_MOVE_DOWN);
    bool up = gpio_get(GPIO_MOVE_UP);
    if (down && !up)
        return MOTOR_DOWN;
    else if (up && !down)
        return MOTOR_UP;
    else
        return MOTOR_STOP;
}
void motor_up()
{
    // check limit switches
    // check phase of encoders
    // if one is higher than the other stop higher motor to allow to catch up
    if (!too_high)
    {
        if (too_low && gpio_get(GPIO_TOO_LOW))
        {
            too_low = false;
        }
        bool on = 1; // may need to be reversed********************************************************************
        gpio_put(GPIO_MOTOR1_IN1, on);
        gpio_put(GPIO_MOTOR1_IN2, !on);
        gpio_put(GPIO_MOTOR2_IN1, on);
        gpio_put(GPIO_MOTOR2_IN2, !on);
        gpio_put(GPIO_MOTOR1_EN, 1);
        gpio_put(GPIO_MOTOR2_EN, 1);
        // if (encoder1_c >= encoder2_c)
        // {
        //     gpio_put(GPIO_MOTOR1_EN, 1);
        // }
        // else
        //     gpio_put(GPIO_MOTOR1_EN, 0);
        // if (encoder1_c <= encoder2_c)
        // {
        //     gpio_put(GPIO_MOTOR2_EN, 1);
        // }
        // else
        //     gpio_put(GPIO_MOTOR2_EN, 0);
    }
}
void motor_down()
{
    // check limit switches
    // check phase of encoders
    // if one is higher than the other stop lower motor to allow to catch up
    if (!too_low)
    {
        if (too_high && gpio_get(GPIO_TOO_HIGH))
        {
            too_high = false;
        }
        bool on = 1; // may need to be reversed********************************************************************
        gpio_put(GPIO_MOTOR1_IN1, !on);
        gpio_put(GPIO_MOTOR1_IN2, on);
        gpio_put(GPIO_MOTOR2_IN1, !on);
        gpio_put(GPIO_MOTOR2_IN2, on);
        gpio_put(GPIO_MOTOR1_EN, 1);
        gpio_put(GPIO_MOTOR2_EN, 1);
        // enable
        // if (encoder1_c >= encoder2_c)
        // {
        //     gpio_put(GPIO_MOTOR1_EN, 1);
        // }
        // else
        //     gpio_put(GPIO_MOTOR1_EN, 0);
        // if (encoder1_c <= encoder2_c)
        // {
        //     gpio_put(GPIO_MOTOR2_EN, 1);
        // }
        // else
        //     gpio_put(GPIO_MOTOR2_EN, 0);
    }
}

int main()
{
    stdio_init_all();
    int arch_init_status = cyw43_arch_init();
    if (arch_init_status != 0)
    {
        printf("WiFi init failed");
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        return -1;
    }
    // cyw43_arch_enable_sta_mode();
    // while(cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 30000))
    //{
    //     printf("WiFi connect failed");
    // }
    // printf("WiFi connected");

    // httpd_init();
    // printf("HTTP server started\n")

    // ssi_init();
    // printf("SSI Handler initialized\n");

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);

    init_motors();
    // init_limit_switches();
    // init_encoders();
    init_inputs();
    init_motor_buttons();

    // gpio_init(6);
    // gpio_init(7);
    // gpio_set_dir(6,GPIO_IN);
    // gpio_set_dir(7,GPIO_IN);
    // gpio_pull_down(0);
    // gpio_set_function(0,GPIO_FUNC_XIP);
    // gpio_set_pulls(0,1,0);//pull up
    // gpio_set_irqover
    // gpio_set_irqenable
    // gpio_set_irq_enabled_with_callback(0, GPIO_IRQ_EDGE_RISE, true, &too_high_handler); //this sets up interups
    // multithread to networking TODO
    while (1)
    {
        switch (move_motors_buttons())
        {
        case MOTOR_DOWN:
            motor_down();
            break;
        case MOTOR_UP:
            motor_up();
            break;
        case MOTOR_STOP:
            gpio_put(GPIO_MOTOR1_EN, 0);
            gpio_put(GPIO_MOTOR2_EN, 0);
            break;
        }
        // check up/down move switches and move accordenly with if statements TODO

        // check networking request buffer and move TODO
    }
}