#include "dht22.h"
#include "main.h"

// --- Microsecond Delay ---
// Tuned for STM32F446RE @ 84MHz
void delay_us(uint16_t us)
{
	uint32_t count = us * 7; // Faster timing
    while(count--) { __NOP(); }
}

void DHT22_Init(void) {
    // No init needed
}

void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP; // Keep internal pull-up enabled
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void DHT22_Start (void) {
	Set_Pin_Output(DHT22_PIN_GPIO_Port, DHT22_PIN_Pin);

	// 1. Pull Low for 1.2ms (Standard DHT22 spec)
	HAL_GPIO_WritePin(DHT22_PIN_GPIO_Port, DHT22_PIN_Pin, 0);
	delay_us(1200);

	// 2. Pull High for 20us then release
	HAL_GPIO_WritePin(DHT22_PIN_GPIO_Port, DHT22_PIN_Pin, 1);
	delay_us(20);

	// 3. Switch to Input mode to listen
	Set_Pin_Input(DHT22_PIN_GPIO_Port, DHT22_PIN_Pin);
}

uint8_t DHT22_Check_Response (void) {
	uint8_t Response = 0;

	// We wait max 40us for the pin to go LOW (Sensor ACK)
	delay_us(40);

	if (!(HAL_GPIO_ReadPin(DHT22_PIN_GPIO_Port, DHT22_PIN_Pin))) {
		// Pin is LOW, Sensor is responding!
		delay_us(80); // Wait for the Low to finish and High to start

		if ((HAL_GPIO_ReadPin(DHT22_PIN_GPIO_Port, DHT22_PIN_Pin))) Response = 1;
		else Response = -1;
	}

	// Wait for the pin to go Low again (End of Response)
	uint32_t timeout = 0;
	while ((HAL_GPIO_ReadPin(DHT22_PIN_GPIO_Port, DHT22_PIN_Pin))) {
		timeout++;
		if(timeout > 10000) return 0; // Safety break
	}

	return Response;
}

uint8_t DHT22_Read_Byte (void) {
	uint8_t i, j;
	for (j=0;j<8;j++) {

		// 1. Wait for the pin to go HIGH (Start of bit)
		uint32_t timeout = 0;
		while (!(HAL_GPIO_ReadPin(DHT22_PIN_GPIO_Port, DHT22_PIN_Pin))) {
			timeout++;
			if(timeout > 10000) return 0;
		}

		// 2. Measure how long it stays HIGH
		// 26-28us = '0', 70us = '1'
		delay_us(40);

		if (!(HAL_GPIO_ReadPin(DHT22_PIN_GPIO_Port, DHT22_PIN_Pin))) {
			// Pin went low already -> It was a short pulse -> '0'
			i&= ~(1<<(7-j));
		}
		else {
			// Pin is still high -> It was a long pulse -> '1'
			i|= (1<<(7-j));
		}

		// 3. Wait for pin to go LOW (End of bit)
		timeout = 0;
		while ((HAL_GPIO_ReadPin(DHT22_PIN_GPIO_Port, DHT22_PIN_Pin))) {
			timeout++;
			if(timeout > 10000) return 0;
		}
	}
	return i;
}
