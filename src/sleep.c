#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>

#define DEV_OUT DT_NODELABEL(gpioa)
#define PIN_OUT 0

void sleep_main(void)
{
    const struct device *dev_out;
    dev_out = device_get_binding(DT_LABEL(DEV_OUT));
    gpio_pin_configure(dev_out, PIN_OUT, GPIO_OUTPUT_ACTIVE);

    while (1) {
        gpio_pin_toggle(dev_out, PIN_OUT);
        // k_busy_wait(1000);
        k_sleep(K_MSEC(1));
    }
}
// expected period = 2ms
// meas. period    = 2.197ms
// 360000 ms/hr = 180k T/hr

// off per hour 180k * .197052m = 35.46936s