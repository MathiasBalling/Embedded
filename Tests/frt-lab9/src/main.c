#include "FreeRTOS.h"
#include "portmacro.h"
#include "queue.h"
#include "task.h"

#include "emp_type.h"
#include "tm4c123gh6pm.h"

#include "uart0.h"
#include "adc.h"
#include "lcd.h"

xQueueHandle q_uartTx;
xQueueHandle q_lcd;

#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define IDLE_PRIO 0
#define LOW_PRIO 1
#define MED_PRIO 2
#define HIGH_PRIO 3

void status_led_task(void *pvParameters) {
  // Setup status led
  SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R3;
  while (!(SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R3)) {
  }
  GPIO_PORTD_DIR_R = 0x40;
  GPIO_PORTD_DEN_R = 0x40;

  while (1) {
    // Toggle status led
    GPIO_PORTD_DATA_R ^= 0x40;
    vTaskDelay(500 / portTICK_RATE_MS); // wait 500 ms.
  }
}
int main() {
  q_uartTx = xQueueCreate(100, sizeof(INT8U));
  q_lcd = xQueueCreate(255, sizeof(INT8U));

  xTaskCreate(status_led_task, "Status_led", USERTASK_STACK_SIZE, NULL,
              LOW_PRIO, NULL);
  xTaskCreate(uart0_task, "Uart0 RX/TX", USERTASK_STACK_SIZE, NULL, MED_PRIO,
              NULL);
  xTaskCreate(adc_task, "Adc read", USERTASK_STACK_SIZE, NULL, MED_PRIO, NULL);
  xTaskCreate(lcd_task, "LCD display", USERTASK_STACK_SIZE, NULL, MED_PRIO, NULL);

  vTaskStartScheduler();
  return 0;
}
