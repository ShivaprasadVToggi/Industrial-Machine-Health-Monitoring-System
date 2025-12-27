/* USER CODE BEGIN Header */
/**
  * ZODIAC FINAL SYSTEM
  * DHT22 + ADXL345 + LCD + IoT (ThingSpeak)
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dht22.h"
#include "adxl345.h"
#include "i2c-lcd.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

// ==========================================

// ==========================================
char *SSID = "SHIVA";       // Your Hotspot Name
char *PASS = "Shiva@879";   // Your Hotspot Password
char *API  = "RSB6N3P4YLOM4CZE"; // Your ThingSpeak Key
// ==========================================

// Data Variables
uint8_t RH1, RH2, TC1, TC2, SUM, CHECK;
float tCelsius = 0;
float RH = 0;
int16_t x, y, z;
float xg, yg, zg;
char lcd_buffer[16];

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
void ESP_Init(void);
void ESP_Send_Multi(float t, float h, float gx, float gy, float gz);

int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();

  /* USER CODE BEGIN 2 */
  // --- 1. STARTUP ---
  DHT22_Init();
  ADXL_Init();
  lcd_init();

  // Show Splash Screen
  lcd_send_cmd(0x80);
  lcd_send_string("ZODIAC TEAM");
  lcd_send_cmd(0xC0);
  lcd_send_string("Connecting WiFi.");

  // Connect to Wi-Fi
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1); // LED ON
  ESP_Init();
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0); // LED OFF

  lcd_send_cmd(0xC0);
  lcd_send_string("WiFi Connected! ");
  HAL_Delay(2000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // --- STEP 1: READ SENSORS ---

	  // Critical Section for DHT22 (Disable Interrupts)
	  __disable_irq();
	  DHT22_Start();
	  CHECK = DHT22_Check_Response();
	  if(CHECK == 1) {
		  RH1 = DHT22_Read_Byte();
		  RH2 = DHT22_Read_Byte();
		  TC1 = DHT22_Read_Byte();
		  TC2 = DHT22_Read_Byte();
		  SUM = DHT22_Read_Byte();
	  }
	  __enable_irq(); // Re-enable Interrupts

	  // Process Temp Data
	  if(CHECK == 1) {
		  tCelsius = (float)((TC1<<8)|TC2) / 10.0;
		  RH = (float)((RH1<<8)|RH2) / 10.0;
	  }

	  // Read Vibration Data
	  ADXL_Read(&x, &y, &z);
	  xg = x * 0.0078;
	  yg = y * 0.0078;
	  zg = z * 0.0078;

	  // --- STEP 2: LCD SLIDESHOW (Hides the 16s delay) ---

	  // SCREEN A: Temp & Humidity (5 Seconds)
	  lcd_clear();
	  lcd_send_cmd(0x80);
	  sprintf(lcd_buffer, "T:%d.%dC H:%d.%d%%", (int)tCelsius, abs((int)(tCelsius*10)%10), (int)RH, abs((int)(RH*10)%10));
	  lcd_send_string(lcd_buffer);
	  lcd_send_cmd(0xC0);
	  lcd_send_string("Status: MONITOR");
	  HAL_Delay(5000);

	  // SCREEN B: Vibration X & Y (5 Seconds)
	  lcd_clear();
	  lcd_send_cmd(0x80);
	  sprintf(lcd_buffer, "X:%d.%02dG Y:%d.%02dG", (int)xg, abs((int)(xg*100)%100), (int)yg, abs((int)(yg*100)%100));
	  lcd_send_string(lcd_buffer);
	  lcd_send_cmd(0xC0);
	  lcd_send_string("Vibration Check");
	  HAL_Delay(5000);

	  // SCREEN C: Vibration Z & Uploading (6 Seconds)
	  lcd_clear();
	  lcd_send_cmd(0x80);
	  sprintf(lcd_buffer, "Z:%d.%02dG", (int)zg, abs((int)(zg*100)%100));
	  lcd_send_string(lcd_buffer);
	  lcd_send_cmd(0xC0);
	  lcd_send_string("Uploading...");

	  // --- STEP 3: UPLOAD TO CLOUD ---
	  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	  ESP_Send_Multi(tCelsius, RH, xg, yg, zg);
	  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

	  lcd_send_cmd(0xC0);
	  lcd_send_string("Upload Done!    ");
	  HAL_Delay(1000);
  }
  /* USER CODE END WHILE */
}

// --- WI-FI FUNCTIONS ---

void ESP_Init(void)
{
	char buffer[100];
	HAL_UART_Transmit(&huart1, (uint8_t*)"AT\r\n", 4, 1000);
	HAL_Delay(500);
	HAL_UART_Transmit(&huart1, (uint8_t*)"AT+CWMODE=1\r\n", 13, 1000);
	HAL_Delay(500);
	sprintf(buffer, "AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, PASS);
	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 5000);
	HAL_Delay(5000);
}

void ESP_Send_Multi(float t, float h, float gx, float gy, float gz)
{
	char cmd_data[200];
	char cmd_len[50];

	HAL_UART_Transmit(&huart1, (uint8_t*)"AT+CIPSTART=\"TCP\",\"184.106.153.149\",80\r\n", 42, 2000);
	HAL_Delay(1000);

	sprintf(cmd_data, "GET /update?api_key=%s&field1=%d.%d&field2=%d.%d&field3=%d.%02d&field4=%d.%02d&field5=%d.%02d\r\n",
			API,
			(int)t, abs((int)(t*10)%10),
			(int)h, abs((int)(h*10)%10),
			(int)gx, abs((int)(gx*100)%100),
			(int)gy, abs((int)(gy*100)%100),
			(int)gz, abs((int)(gz*100)%100));

	sprintf(cmd_len, "AT+CIPSEND=%d\r\n", strlen(cmd_data));
	HAL_UART_Transmit(&huart1, (uint8_t*)cmd_len, strlen(cmd_len), 1000);
	HAL_Delay(500);

	HAL_UART_Transmit(&huart1, (uint8_t*)cmd_data, strlen(cmd_data), 2000);
	HAL_Delay(1000);

	HAL_UART_Transmit(&huart1, (uint8_t*)"AT+CIPCLOSE\r\n", 13, 1000);
}

// --- INIT FUNCTIONS (These are the ones you were missing) ---

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_I2C1_Init(void)
{
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DHT22_PIN_GPIO_Port, DHT22_PIN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DHT22_PIN_Pin */
  GPIO_InitStruct.Pin = DHT22_PIN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(DHT22_PIN_GPIO_Port, &GPIO_InitStruct);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
