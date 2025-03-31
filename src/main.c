#include "stm32f7xx_hal.h"
#include <FreeRTOS.h>
#include <task.h>
#include <stm32f7xx_hal_conf.h>

#define LED_PIN                                GPIO_PIN_5
#define LED_GPIO_PORT                          GPIOA
#define LED_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOA_CLK_ENABLE()

TaskHandle_t blinkyTaskHandle;

void blinky_task(void* taskArg) {
    while(1) {
        HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

int main(void)
{
  HAL_Init();
  
  LED_GPIO_CLK_ENABLE();
  
  GPIO_InitTypeDef GPIO_InitStruct;
  
  GPIO_InitStruct.Pin = LED_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct); 

  if (xTaskCreate(&blinky_task, "Blinky Task", 1024U / sizeof(int), NULL, 5 /* prio */, &blinkyTaskHandle) != pdPASS) {
    /* error creating task, e.g., insufficient memory */
  }

  vTaskStartScheduler(); // start scheduling
  while (1); // should never reach here
}