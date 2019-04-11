// set <pin> to output.
void gpio_set_output(unsigned pin);

// set <pin> on.
void gpio_set_on(unsigned pin);

// set <pin> off
void gpio_set_off(unsigned pin);

// set <pin> to input.
void gpio_set_input(unsigned pin);

// set <pin> to <v> (v \in {0,1})
void gpio_write(unsigned pin, unsigned v);

// read input from <pin>.
unsigned gpio_read(unsigned pin);
