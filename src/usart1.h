#ifndef USART1_HF
#define USART1_HF

void usart1_init(void);

void usart1_send_string_len(char *s, unsigned int len);

void usart1_send_string(char *s);

void send_hex_byte(uint8_t c);

void send_hex(uint32_t val);

#endif
