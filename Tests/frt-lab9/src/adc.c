// -----------------
// adc.c
// -----------------
#include "adc.h"
#include "FreeRTOS.h"
#include "emp_type.h"
#include "task.h"
#include "tm4c123gh6pm.h"
#include "queue.h"
#include <stdint.h>

extern xQueueHandle q_uartTx;
extern xQueueHandle q_lcd;

INT16U get_adc() { return (ADC0_SSFIFO3_R); }

void adc_init() {
  SYSCTL_RCGC0_R |= SYSCTL_RCGC0_ADC0;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;

  // Set ADC0 Sequencer priorities.
  // SS3(bit12-13): Priority = 0 ; Highest
  // SS2(bit8-9):   Priority = 1
  // SS1(bit4-5):   Priority = 2
  // SS0(bit0-1):   Priority = 3 ; Lowest
  ADC0_SSPRI_R = 0x00000123;

  // Disable all sequencers
  ADC0_ACTSS_R = 0;

  // Trigger for Sequencer 3 (bit 12-15) = 0xF = Always.
  ADC0_EMUX_R = 0x0000F000;

  // sample multiplexer input, sequencer 3 select, ADC 11 (0x0B) enable.
  ADC0_SSMUX3_R = 0x0B;

  // ADC sample sequence control 3 = END0
  ADC0_SSCTL3_R = 0x00000002;

  // enable sequencer 3
  ADC0_ACTSS_R = 0x00000008;

  // Start conversion at sequencer 3
  ADC0_PSSI_R = 0x08;
}
void send_value(INT16U val, QueueHandle_t queue){
  INT8U temp = val / 10000;
  xQueueSend(queue, &temp, 0);
  temp = val % 10000 / 1000 + '0';
  xQueueSend(queue, &temp, 0);
  temp = val % 1000 / 100 + '0';
  xQueueSend(queue, &temp, 0);
  temp = val % 100 / 10 + '0';
  xQueueSend(queue, &temp, 0);
  temp = val % 10 + '0';
  xQueueSend(queue, &temp, 0);
}

void adc_task(void *pvParameters) {
  adc_init();
  INT16U adc;
  while (1) {
    adc = get_adc();
    send_value(adc, q_uartTx);
    xQueueSend(q_uartTx, "\n", 0);
    send_value(adc, q_lcd);
    vTaskDelay(100 / portTICK_RATE_MS);
  }
}
