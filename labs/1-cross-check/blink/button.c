#include "gpio.h"

// these are in start.s
void put32(volatile void *addr, unsigned v);
unsigned get32(const volatile void *addr);
void dummy(int i);

// countdown 'ticks' cycles; the asm probably isn't necessary.
void delay(unsigned ticks) {
    while(ticks-- > 0)
        asm("add r1, r1, #0");
}

// resets the pi to the original state
void reboot(void) {
     const int PM_RSTC = 0x2010001c;
     const int PM_WDOG = 0x20100024;
     const int PM_PASSWORD = 0x5a000000;
     const int PM_RSTC_WRCFG_FULL_RESET = 0x00000020;
     int i;
     for(i = 0; i < 100000; i++)
          dummy(i);
     put32((void *) PM_WDOG, PM_PASSWORD | 1);
     put32((void *) PM_RSTC, PM_PASSWORD | PM_RSTC_WRCFG_FULL_RESET);
     while(1);
}

// when you run should blink 10 times. will have to restart the pi by pulling the
// usb connection out.
void notmain(void) {
    int output_port = 20;
    int input_port = 21;
    gpio_set_input(input_port);
    gpio_set_output(output_port);
    while(1) {
        gpio_write(output_port, gpio_read(input_port));
    }
    reboot();
}
