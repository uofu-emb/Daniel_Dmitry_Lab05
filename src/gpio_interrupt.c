#include <zephyr.h>
#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>

#define DEV_IN DT_NODELABEL(gpioa)
#define DEV_OUT DT_NODELABEL(gpioa)
#define PIN_OUT 0
#define PIN_IN 1

struct gpio_callback callback;
const struct device* dev_in, * dev_out;

#define STACKSIZE 2000

K_MSGQ_DEFINE(msgq, sizeof(int), 32, 4);
int data = 32;

K_THREAD_STACK_DEFINE(t_stack, STACKSIZE);
struct k_thread my_thread;

void pin_interrupt(const struct device* port,
    struct gpio_callback* cb,
    gpio_port_pins_t pins_) {
    // gpio_pin_toggle(dev_out, PIN_OUT);
    k_msgq_put(&msgq, &data, K_FOREVER);
}

void thread_function(void) {
    int received = 0;
    //moving to msgq increased delay from ~30us to ~50us
    while (true) {
        k_msgq_get(&msgq, &received, K_FOREVER);
        gpio_pin_toggle(dev_out, PIN_OUT);

    }
}

void interrupt_main(void) {
    dev_in = device_get_binding(DT_LABEL(DEV_IN));
    dev_out = device_get_binding(DT_LABEL(DEV_OUT));

    gpio_pin_configure(dev_out, PIN_OUT, GPIO_OUTPUT_ACTIVE);
    gpio_pin_configure(dev_in, PIN_IN, GPIO_INPUT);
    gpio_pin_interrupt_configure(dev_in, PIN_IN, GPIO_INT_EDGE_RISING);
    gpio_init_callback(&callback, pin_interrupt, 1 << PIN_IN);
    gpio_add_callback(dev_in, &callback);
    

    k_thread_create(&my_thread,
        t_stack,
        STACKSIZE,
        (k_thread_entry_t)thread_function,
        NULL,
        NULL,
        NULL,
        K_PRIO_COOP(7),
        0,
        K_NO_WAIT);


    k_sleep(K_FOREVER);
}
