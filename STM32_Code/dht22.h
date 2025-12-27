#ifndef INC_DHT22_H_
#define INC_DHT22_H_

#include "stm32f4xx_hal.h"

void DHT22_Init(void);
void DHT22_Start(void);
uint8_t DHT22_Check_Response(void);
uint8_t DHT22_Read_Byte(void);
void delay_us (uint16_t us);

#endif /* INC_DHT22_H_ */
