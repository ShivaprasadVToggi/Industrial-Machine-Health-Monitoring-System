#ifndef INC_ADXL345_H_
#define INC_ADXL345_H_

#include "stm32f4xx_hal.h"

void ADXL_Init (void);
void ADXL_Read (int16_t *x, int16_t *y, int16_t *z);

#endif /* INC_ADXL345_H_ */
