#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>

#define DEV_OUT DT_NODELABEL(gpioa)
#define PIN_OUT 0

void kernel_main(void)
{
    const struct device *dev_out;
    dev_out = device_get_binding(DT_LABEL(DEV_OUT));
    gpio_pin_configure(dev_out, PIN_OUT, GPIO_OUTPUT_ACTIVE);

	struct k_timer t;
	k_timer_init(&t, NULL, NULL);
    while (1) {
        gpio_pin_toggle(dev_out, PIN_OUT);
		k_timer_start(&t, K_MSEC(1), K_NO_WAIT);
        k_busy_wait(800);


		k_timer_status_sync(&t);
    }
}
// expected period = 2ms
// meas. period    = 1.99732ms
// 360000 ms/hr = 180k T/hr

// off per hour 180k * -0.00268m = -0.4824s