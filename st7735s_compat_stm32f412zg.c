#include "st7735s_compat.h"
// #include "spi.h"
// #include "tim.h"

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_ll_gpio.h"

#define ST_RESET_Pin GPIO_PIN_14
#define ST_RESET_GPIO_Port GPIOD
#define ST_DC_Pin GPIO_PIN_15
#define ST_DC_GPIO_Port GPIOD
#define ST_CS_Pin GPIO_PIN_12
#define ST_CS_GPIO_Port GPIOF

uint32_t tim_period = 32768;
uint32_t tim_pulse;

extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim3;

#ifndef _Pin_Init
#define _Pin_Init(name)                                                        \
  LL_GPIO_SetPinMode(name##_GPIO_Port, name##_Pin, LL_GPIO_MODE_OUTPUT)
#define _Pin_Toggle(name) LL_GPIO_TogglePin(name##_GPIO_Port, name##_Pin)
#define _Pin_High(name) LL_GPIO_SetOutputPin(name##_GPIO_Port, name##_Pin);
#define _Pin_Low(name) LL_GPIO_ResetOutputPin(name##_GPIO_Port, name##_Pin);
#endif

void
SPI_Init(void)
{
  // Pin_Low(CS);
}

void
Pin_CS_Low(void)
{
  _Pin_Low(ST_CS);
}

void
Pin_CS_High(void)
{
  _Pin_High(ST_CS);
}

void
Pin_RES_High(void)
{
  _Pin_High(ST_RESET);
}

void
Pin_RES_Low(void)
{
  _Pin_Low(ST_RESET);
}

void
Pin_DC_High(void)
{
  _Pin_High(ST_DC);
}

void
Pin_DC_Low(void)
{
  _Pin_Low(ST_DC);
}

extern uint8_t backlight_pct;
void
Pin_BLK_Pct(uint8_t pct)
{
  backlight_pct = pct;
  tim_pulse = pct * tim_period / 100;
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, tim_pulse);
}

void
SPI_send(uint16_t len, uint8_t* data)
{
  Pin_CS_Low();
#if 0
	while (len--)
		HAL_SPI_Transmit(&hspi1, data++, 1, 0xF000);
#else
  HAL_SPI_Transmit(&hspi1, data, len, 0xF000);
  Pin_CS_High();
#endif
}

void
SPI_TransmitCmd(uint16_t len, uint8_t* data)
{
  Pin_DC_Low();
  SPI_send(len, data);
}

void
SPI_TransmitData(uint16_t len, uint8_t* data)
{
  Pin_DC_High();
  SPI_send(len, data);
}

void
SPI_Transmit(uint16_t len, uint8_t* data)
{
  SPI_TransmitCmd(1, data++);
  if (--len)
    SPI_TransmitData(len, data);
}

void
_Delay(uint32_t d)
{
  HAL_Delay(d);
}
