#include "adxl345.h"

extern I2C_HandleTypeDef hi2c1;

// ADXL345 Device Address (0x53 << 1)
#define ADXL_ADDR 0xA6

void ADXL_Init (void)
{
	uint8_t data;

	// 1. Check ID
	HAL_I2C_Mem_Read(&hi2c1, ADXL_ADDR, 0x00, 1, &data, 1, 100);
	if(data == 0xE5)
	{
		// 2. Go to Standby
		data = 0;
		HAL_I2C_Mem_Write(&hi2c1, ADXL_ADDR, 0x2D, 1, &data, 1, 100);

		// 3. Configure Data Format (+/- 4g)
		data = 0x01;
		HAL_I2C_Mem_Write(&hi2c1, ADXL_ADDR, 0x31, 1, &data, 1, 100);

		// 4. Go to Measurement Mode
		data = 0x08;
		HAL_I2C_Mem_Write(&hi2c1, ADXL_ADDR, 0x2D, 1, &data, 1, 100);
	}
}

void ADXL_Read (int16_t *x, int16_t *y, int16_t *z)
{
	uint8_t data_rec[6];
	HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(&hi2c1, ADXL_ADDR, 0x32, 1, data_rec, 6, 100);

	if(ret == HAL_OK) {
		*x = ((data_rec[1] << 8) | data_rec[0]);
		*y = ((data_rec[3] << 8) | data_rec[2]);
		*z = ((data_rec[5] << 8) | data_rec[4]);
	} else {
		*x = 0; *y = 0; *z = 0;
	}
}
