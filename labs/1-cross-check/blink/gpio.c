// these are in start.s
void put32(volatile void *addr, unsigned v);
unsigned get32(const volatile void *addr);
void dummy(int i);

// see broadcomm documents for magic addresses.
#define GPIO_BASE 0x20200000
volatile unsigned *gpio_fsel0 = (volatile unsigned *)(GPIO_BASE + 0x00);
volatile unsigned *gpio_set0  = (volatile unsigned *)(GPIO_BASE + 0x1C);
volatile unsigned *gpio_clr0  = (volatile unsigned *)(GPIO_BASE + 0x28);
volatile unsigned *gpio_lev0  = (volatile unsigned *)(GPIO_BASE + 0x34);

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
    volatile unsigned *addr = gpio_fsel0 + (pin / 10);
    unsigned offset = ((pin % 10) * 3);
    unsigned gpio = get32(addr);
    gpio &=  ~(0b111 << offset);
    gpio |= (0b000 << offset);
    put32(addr, gpio);
}
// set <pin> to <v> (v \in {0,1})
void gpio_write(unsigned pin, unsigned v) {
    (v == 1) ? gpio_set_on(pin) : gpio_set_off(pin);
}

// read input from <pin>.
unsigned gpio_read(unsigned pin) {
    unsigned gpio = get32(gpio_lev0);
    unsigned v = ((gpio >> pin) & 0b1);
    return v;
}
