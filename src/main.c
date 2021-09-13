#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <aes.h>
#include <stdint.h>

#include "aesTest.h"
#include "usart1.h"

void aesexperiment(void);
void send_aes_data(void);


uint8_t aesKey[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
uint8_t aesData[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};

void send_aes_data(void)
{
	for(int i = 0; i < 16; i++)
	{
		send_hex_byte(aesData[i]);
	}
}

void aesexperiment(void)
{
	struct AES_ctx ctx = {{0},{0}};
	AES_init_ctx(&ctx, aesKey);
	gpio_set(GPIOC, GPIO9);
	AES_ECB_encrypt(&ctx, aesData);
	gpio_clear(GPIOC, GPIO9);
}

int main(void)
{
	
	//Enable gpio clock for led PC9 and PC8
	rcc_periph_clock_enable(RCC_GPIOC);

	//Set mode for PC9 and PC8
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO8 | GPIO9);
	gpio_clear(GPIOC, GPIO8 | GPIO9);
	
	usart1_init();

	usart1_send_string("Initialise\r");
	usart1_send_string("Test AES.....");
	int failed = 0;
	failed = aesTest();
	if(failed != 0)
	{
		usart1_send_string("failed\r");
		while(1)
		{
			__asm__("nop");
		}
	}
	usart1_send_string("success\r");
	gpio_set(GPIOC, GPIO8);
	usart1_send_string("Start experiment\r");
	aesexperiment();
	usart1_send_string("Experiment done\r");
	while(1)
	{
		gpio_toggle(GPIOC, GPIO8);
	}
}
