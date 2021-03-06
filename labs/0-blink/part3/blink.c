/*
 * write code to allow blinking using arbitrary pins.
 * Implement:
 *	- gpio_set_output(pin) --- set GPIO <pin> as an output (vs input) pin.
 *	- gpio_set_on(pin) --- set the GPIO <pin> on.
 * 	- gpio_set_off(pin) --- set the GPIO <pin> off.
 * Use the minimal number of loads and stores to GPIO memory.
 *
 * start.s defines a of helper functions (feel free to look at the assembly!  it's
 *  not tricky):
 *      uint32_t get32(volatile uint32_t *addr)
 *              --- return the 32-bit value held at <addr>.
 *
 *      void put32(volatile uint32_t *addr, uint32_t v)
 *              -- write the 32-bit quantity <v> to <addr>
 *
 * Check-off:
 *  1. get a single LED to blink.
 *  2. attach an LED to pin 19 and another to pin 20 and blink in opposite order (i.e.,
 *     one should be on, while the other is off).   Note, if they behave weirdly, look
 *     carefully at the wording for GPIO set.
 */

// these are in start.s
void put32(volatile void *addr, unsigned v);
unsigned get32(const volatile void *addr);
void dummy(int i);

// see broadcomm documents for magic addresses.
#define GPIO_BASE 0x20200000
volatile unsigned *gpio_fsel0 = (volatile unsigned *)(GPIO_BASE + 0x00);
volatile unsigned *gpio_set0  = (volatile unsigned *)(GPIO_BASE + 0x1C);
volatile unsigned *gpio_clr0  = (volatile unsigned *)(GPIO_BASE + 0x28);

// set <pin> to output.  note: fsel0, fsel1, fsel2 are contiguous in memory,
// so you can use array calculations!
void gpio_set_output(unsigned pin) {
    // use gpio_fsel0
    volatile unsigned *addr = gpio_fsel0 + (pin / 10);
    unsigned offset = ((pin % 10) * 3);
    unsigned gpio = get32(addr);
    gpio &=  ~(0b111 << offset);
    gpio |= (0b001 << offset);
    put32(addr, gpio);
}


// set <pin> on.
void gpio_set_on(unsigned pin) {
    // use gpio_set0
    put32(gpio_set0, (0b1 << pin));
}

// set <pin> off
void gpio_set_off(unsigned pin) {
    // use gpio_clr0
    put32(gpio_clr0, (0b1 << pin));
}

// For later labs, write these routines as well.

// set <pin> to input.
void gpio_set_input(unsigned pin) {
    // use gpio_fsel0
}
// set <pin> to <v> (v \in {0,1})
void gpio_write(unsigned pin, unsigned v) {
    //
}

// countdown 'ticks' cycles; the asm probably isn't necessary.
void delay(unsigned ticks) {
    while(ticks-- > 0)
        asm("add r1, r1, #0");
}

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
    int led = 20;
    int input = 21;
    gpio_set_output(led);
    gpio_set_input(input);
    for(int i = 0; i < 10; i++) {
        gpio_set_on(led);
        delay(1000000);
        gpio_set_off(led);
        delay(1000000);
    }
    reboot();
}
