#include "lwip/apps/httpd.h"
#include "pico/cwy43_arch.h"

// CGI Handler
const char * cgi_led_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    if(strcomp(pcParam[0], "led") == 0)
    {
        // Toggle the LED
        if(strcomp(pcValue[0], "0") == 0)
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN,1);
        else if(strcomp(pcValue[0], "1") == 0)
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN,0);

    }

    return "/index.shtml";
}

static const tCGI cgi_handlers[] = {
    {"/led.cgi", cgi_led_handler},
    // {NULL, NULL} Add more handlers this way
};

void cgi_init(void){
    http_set_cgi_handlers(cgi_handlers, 1);
}