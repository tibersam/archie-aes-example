
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#include "usart1.h"

void usart1_gpio_configure(void);

void usart1_enable(void);

void usart1_send_char(char c);

void usart1_gpio_configure(void)
{
	/*Configure gpio*/
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);
	gpio_set_af(GPIOA, GPIO_AF1, GPIO9);
}

void usart1_init(void)
{
	rcc_periph_clock_enable(RCC_USART1);
	rcc_periph_clock_enable(RCC_GPIOA);
	
	usart_set_baudrate(USART1, 115200);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_CR2_STOPBITS_1);
	usart_set_mode(USART1, USART_MODE_TX);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
	
	usart1_enable();
}

void usart1_enable(void)
{
	usart1_gpio_configure();
	usart_enable(USART1);
}

void usart1_send_char(char c)
{
	usart_send_blocking(USART1, c);
}

void usart1_send_string_len(char *s, unsigned int len)
{
	for(unsigned int i = 0; i < len; i++)
	{
		if(*s == 0)
		{
			break;
		}
		usart1_send_char(*s);
		if(*s == '\r')
		{
			usart1_send_char('\n');
		}
		s++;
	}
}
void usart1_send_string(char *s)
{
	unsigned int len = 0;
	char *s_inc = s;
	while(*s_inc != '\0')
	{
		len++;
		s_inc++;
	}
	usart1_send_string_len(s, len);
}

void send_hex_byte(uint8_t c)
{
	char lookup[]="0123456789ABCDEF";
	uint8_t c_high = (c & 0xF0) >> 4;
	c = c & 0xF;
	usart1_send_char(lookup[c_high]);
	usart1_send_char(lookup[c]);
}

void send_hex(uint32_t val)
{
	usart1_send_string("0x");
	for(int i = 0; i < 4; i++)
	{
		send_hex_byte((char)(val >> i*8) & 0xFF );
	}
	usart1_send_string("\r");
}
