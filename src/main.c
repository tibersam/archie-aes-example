

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <aes.h>


void main(void)
{
	
	//Enable gpio clock for led PC9 and PC8
	rcc_periph_clock_enable(RCC_GPIOC);

	//Set mode for PC9 and PC8
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO8 | GPIO9);

	while(true)
	{
		gpio_toggle(GPIOC, GPIO8);
	}
}
