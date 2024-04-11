#include "lwip/apps/httpd.h"
#include "pico/cwy43_arch.h"
#include "hardware/adc.h"

// Set SSI variables
const char * ssi_tags[] = {"motor1", "motor2", "encoder1", "encoder2", "adc", "led"};

u16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen)(
    size_t printed;

    switch(iIndex){
        case 0:
            // Do something with motor1 here
            // printed = snprintf(pcInsert, iInsertLen, "%d", MOTOR_DOWN);
            break;
        case 1:
            // Do something with motor2 here
            // printed = snprintf(pcInsert, iInsertLen, "%d", MOTOR_UP);
            break;
        case 2:
            // Do something with encoder1 here
            // printed = snprintf(pcInsert, iInsertLen, "%d", encoder1);
            break;
        case 3:
            // Do something with encoder2 here
            // printed = snprintf(pcInsert, iInsertLen, "%d", encoder2);
            break;
        case 4:
            // Do something with adc here
            // printed = snprintf(pcInsert, iInsertLen, "%d", adc_read());
            break;
        case 5:
            // Check the state of the LED
            bool led_state = cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN);
            if(led_state)
                printed = snprintf(pcInsert, iInsertLen, "ON");
            else
                printed = snprintf(pcInsert, iInsertLen, "OFF");
            break;
        default:
            printed = 0;
            break;

    }
    return (u16_t)printed;
)

void ssi_init()
{

    http_set_ssi_handler(ssi_handler, ssi_tags, LWIP_ARRAYSIZE(ssi_tags));
}