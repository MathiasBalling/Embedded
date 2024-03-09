//*****************************************************************************
// startup_gcc.c - Startup code for use with GNU tools.
//*****************************************************************************

#include <stdint.h>

//*****************************************************************************
//
// Forward declaration of the default fault handlers.
//
//*****************************************************************************
void ResetISR(void);
static void NmiSR(void);
static void FaultISR(void);
static void IntDefaultHandler(void);

//*****************************************************************************
//
// The entry point for the application.
//
//*****************************************************************************
extern int main(void);

//*****************************************************************************
//
// Add handlers for the various interrupt sources.
//
//*****************************************************************************
extern void systick_handler(void);
extern void gpioa_handler();
extern void gpiob_handler();
extern void gpioc_handler();
extern void gpiod_handler();
extern void gpioe_handler();
extern void uart0_handler();
extern void uart1_handler();
extern void ssi0_handler();
extern void i2c0_handler();
extern void pwm0_fault_handler();
extern void pwm0_0_handler();
extern void pwm0_1_handler();
extern void pwm0_2_handler();
extern void qei0_handler();
extern void adc0ss0_handler();
extern void adc0ss1_handler();
extern void adc0ss2_handler();
extern void adc0ss3_handler();
extern void watchdog_handler();
extern void timer0a_handler();
extern void timer0b_handler();
extern void timer1a_handler();
extern void timer1b_handler();
extern void timer2a_handler();
extern void timer2b_handler();
extern void comp0_handler();
extern void comp1_handler();
extern void sysctl_handler();
extern void flash_handler();
extern void gpiof_handler();
extern void uart2_handler();
extern void ssi1_handler();
extern void timer3a_handler();
extern void timer3b_handler();
extern void i2c1_handler();
extern void qei1_handler();
extern void can0_handler();
extern void can1_handler();
extern void hibernate_handler();
extern void usb0_handler();
extern void pwm0_3_handler();
extern void udma_handler();
extern void udmaerr_handler();
extern void adc1ss0_handler();
extern void adc1ss1_handler();
extern void adc1ss2_handler();
extern void adc1ss3_handler();
extern void ssi2_handler();
extern void ssi3_handler();
extern void uart3_handler();
extern void uart4_handler();
extern void uart5_handler();
extern void uart6_handler();
extern void uart7_handler();
extern void i2c2_handler();
extern void i2c3_handler();
extern void timer4a_handler();
extern void timer4b_handler();
extern void timer5a_handler();
extern void timer5b_handler();
extern void wtimer0a_handler();
extern void wtimer0b_handler();
extern void wtimer1a_handler();
extern void wtimer1b_handler();
extern void wtimer2a_handler();
extern void wtimer2b_handler();
extern void wtimer3a_handler();
extern void wtimer3b_handler();
extern void wtimer4a_handler();
extern void wtimer4b_handler();
extern void wtimer5a_handler();
extern void wtimer5b_handler();
extern void sysexc_handler();
extern void pwm1_0_handler();
extern void pwm1_1_handler();
extern void pwm1_2_handler();
extern void pwm1_3_handler();
extern void pwm1_fault();

//*****************************************************************************
//
// Reserve space for the system stack.
//
//*****************************************************************************
static unsigned long pulStack[64];

//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000.
//
//*****************************************************************************
__attribute__((section(".isr_vector"))) void (*const g_pfnVectors[])(void) = {
    (void (*)(void))((unsigned long)pulStack + sizeof(pulStack)),
    // The initial stack pointer
    ResetISR,           // The reset handler
    NmiSR,              // The NMI handler
    FaultISR,           // The hard fault handler
    IntDefaultHandler,  // The MPU fault handler
    IntDefaultHandler,  // The bus fault handler
    IntDefaultHandler,  // The usage fault handler
    0,                  // Reserved
    0,                  // Reserved
    0,                  // Reserved
    0,                  // Reserved
    IntDefaultHandler,  // SVCall handler
    IntDefaultHandler,  // Debug monitor handler
    0,                  // Reserved
    IntDefaultHandler,  // The PendSV handler
    systick_handler,    // The SysTick handler
    gpioa_handler,      // GPIO Port A
    gpiob_handler,      // GPIO Port B
    gpioc_handler,      // GPIO Port C
    gpiod_handler,      // GPIO Port D
    gpioe_handler,      // GPIO Port E
    uart0_handler,      // UART0 Rx and Tx
    uart1_handler,      // UART1 Rx and Tx
    ssi0_handler,       // SSI0 Rx and Tx
    i2c0_handler,       // I2C0 Master and Slave
    pwm0_fault_handler, // PWM Fault
    pwm0_0_handler,     // PWM Generator 0
    pwm0_1_handler,     // PWM Generator 1
    pwm0_2_handler,     // PWM Generator 2
    qei0_handler,       // Quadrature Encoder 0
    adc0ss0_handler,    // ADC Sequence 0
    adc0ss1_handler,    // ADC Sequence 1
    adc0ss2_handler,    // ADC Sequence 2
    adc0ss3_handler,    // ADC Sequence 3
    watchdog_handler,   // Watchdog timer
    timer0a_handler,    // Timer 0 subtimer A
    timer0b_handler,    // Timer 0 subtimer B
    timer1a_handler,    // Timer 1 subtimer A
    timer1b_handler,    // Timer 1 subtimer B
    timer2a_handler,    // Timer 2 subtimer A
    timer2b_handler,    // Timer 2 subtimer B
    comp0_handler,      // Analog Comparator 0
    comp1_handler,      // Analog Comparator 1
    IntDefaultHandler,  // Analog Comparator 2
    sysctl_handler,     // System Control (PLL, OSC, BO)
    flash_handler,      // FLASH Control
    gpiof_handler,      // GPIO Port F
    IntDefaultHandler,  // GPIO Port G
    IntDefaultHandler,  // GPIO Port H
    uart2_handler,      // UART2 Rx and Tx
    ssi1_handler,       // SSI1 Rx and Tx
    timer3a_handler,    // Timer 3 subtimer A
    timer3b_handler,    // Timer 3 subtimer B
    i2c1_handler,       // I2C1 Master and Slave
    qei1_handler,       // Quadrature Encoder 1
    can0_handler,       // CAN0
    can1_handler,       // CAN1
    IntDefaultHandler,  // CAN2
    IntDefaultHandler,  // Ethernet
    IntDefaultHandler,  // Hibernate
    usb0_handler,       // USB0
    pwm0_3_handler,     // PWM Generator 3
    udma_handler,       // uDMA Software Transfer
    udmaerr_handler,    // uDMA Error
    adc1ss0_handler,    // ADC1 Sequence 0
    adc1ss1_handler,    // ADC1 Sequence 1
    adc1ss2_handler,    // ADC1 Sequence 2
    adc1ss3_handler,    // ADC1 Sequence 3
    IntDefaultHandler,  // I2S0
    IntDefaultHandler,  // External Bus Interface 0
    IntDefaultHandler,  // GPIO Port J
    IntDefaultHandler,  // GPIO Port K
    IntDefaultHandler,  // GPIO Port L
    ssi2_handler,       // SSI2 Rx and Tx
    ssi3_handler,       // SSI3 Rx and Tx
    uart3_handler,      // UART3 Rx and Tx
    uart4_handler,      // UART4 Rx and Tx
    uart5_handler,      // UART5 Rx and Tx
    uart6_handler,      // UART6 Rx and Tx
    uart7_handler,      // UART7 Rx and Tx
    0,                  // Reserved
    0,                  // Reserved
    0,                  // Reserved
    0,                  // Reserved
    i2c2_handler,       // I2C2 Master and Slave
    i2c3_handler,       // I2C3 Master and Slave
    timer4a_handler,    // Timer 4 subtimer A
    timer4b_handler,    // Timer 4 subtimer B
    0,                  // Reserved
    0,                  // Reserved
    0,                  // Reserved
    0,                  // Reserved
    0,                  // Reserved
    0,                  // Reserved
    0,                  // Reserved
    0,                  // Reserved
    0,                  // Reserved
    0,                  // Reserved
    0,                  // Reserved
    0,                  // Reserved
    0,                  // Reserved
    0,                  // Reserved
    0,                  // Reserved
    0,                  // Reserved
    0,                  // Reserved
    0,                  // Reserved
    0,                  // Reserved
    0,                  // Reserved
    timer5a_handler,    // Timer 5 subtimer A
    timer5b_handler,    // Timer 5 subtimer B
    wtimer0a_handler,   // Wide Timer 0 subtimer A
    wtimer0b_handler,   // Wide Timer 0 subtimer B
    wtimer1a_handler,   // Wide Timer 1 subtimer A
    wtimer1b_handler,   // Wide Timer 1 subtimer B
    wtimer2a_handler,   // Wide Timer 2 subtimer A
    wtimer2b_handler,   // Wide Timer 2 subtimer B
    wtimer3a_handler,   // Wide Timer 3 subtimer A
    wtimer3b_handler,   // Wide Timer 3 subtimer B
    wtimer4a_handler,   // Wide Timer 4 subtimer A
    wtimer4b_handler,   // Wide Timer 4 subtimer B
    wtimer5a_handler,   // Wide Timer 5 subtimer A
    wtimer5b_handler,   // Wide Timer 5 subtimer B
    IntDefaultHandler,  // FPU
    IntDefaultHandler,  // PECI 0
    IntDefaultHandler,  // LPC 0
    IntDefaultHandler,  // I2C4 Master and Slave
    IntDefaultHandler,  // I2C5 Master and Slave
    IntDefaultHandler,  // GPIO Port M
    IntDefaultHandler,  // GPIO Port N
    IntDefaultHandler,  // Quadrature Encoder 2
    IntDefaultHandler,  // Fan 0
    0,                  // Reserved
    IntDefaultHandler,  // GPIO Port P (Summary or P0)
    IntDefaultHandler,  // GPIO Port P1
    IntDefaultHandler,  // GPIO Port P2
    IntDefaultHandler,  // GPIO Port P3
    IntDefaultHandler,  // GPIO Port P4
    IntDefaultHandler,  // GPIO Port P5
    IntDefaultHandler,  // GPIO Port P6
    IntDefaultHandler,  // GPIO Port P7
    IntDefaultHandler,  // GPIO Port Q (Summary or Q0)
    IntDefaultHandler,  // GPIO Port Q1
    IntDefaultHandler,  // GPIO Port Q2
    IntDefaultHandler,  // GPIO Port Q3
    IntDefaultHandler,  // GPIO Port Q4
    IntDefaultHandler,  // GPIO Port Q5
    IntDefaultHandler,  // GPIO Port Q6
    IntDefaultHandler,  // GPIO Port Q7
    IntDefaultHandler,  // GPIO Port R
    IntDefaultHandler,  // GPIO Port S
    pwm1_0_handler,     // PWM 1 Generator 0
    pwm1_1_handler,     // PWM 1 Generator 1
    pwm1_2_handler,     // PWM 1 Generator 2
    pwm1_3_handler,     // PWM 1 Generator 3
    pwm1_fault,         // PWM 1 Fault
};

//*****************************************************************************
//
// The following are constructs created by the linker, indicating where the
// the "data" and "bss" segments reside in memory.  The initializers for the
// for the "data" segment resides immediately following the "text" segment.
//
//*****************************************************************************
extern unsigned long _etext;
extern unsigned long _data;
extern unsigned long _edata;
extern unsigned long _bss;
extern unsigned long _ebss;

//*****************************************************************************
//
// This is the code that gets called when the processor first starts execution
// following a reset event.  Only the absolutely necessary set is performed,
// after which the application supplied entry() routine is called.  Any fancy
// actions (such as making decisions based on the reset cause register, and
// resetting the bits in that register) are left solely in the hands of the
// application.
//
//*****************************************************************************
void ResetISR(void) {
  unsigned long *pulSrc, *pulDest;

  //
  // Copy the data segment initializers from flash to SRAM.
  //
  pulSrc = &_etext;
  for (pulDest = &_data; pulDest < &_edata;) {
    *pulDest++ = *pulSrc++;
  }

  //
  // Zero fill the bss segment.
  //
  __asm("    ldr     r0, =_bss\n"
        "    ldr     r1, =_ebss\n"
        "    mov     r2, #0\n"
        "    .thumb_func\n"
        "zero_loop:\n"
        "        cmp     r0, r1\n"
        "        it      lt\n"
        "        strlt   r2, [r0], #4\n"
        "        blt     zero_loop");

  //
  // Call the application's entry point.
  //
  main();
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a NMI.  This
// simply enters an infinite loop, preserving the system state for examination
// by a debugger.
//
//*****************************************************************************
static void NmiSR(void) {
  //
  // Enter an infinite loop.
  //
  while (1) {
  }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a fault
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void FaultISR(void) {
  //
  // Enter an infinite loop.
  //
  while (1) {
  }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives an unexpected
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void IntDefaultHandler(void) {
  //
  // Go into an infinite loop.
  //
  while (1) {
  }
}

//*****************************************************************************
//
// Add default handlers for the following interrupts.
// Meant to be overridden by the application.
//
//*****************************************************************************
__attribute__((weak)) void systick_handler() {
  while (1) {
  }
}
__attribute__((weak)) void gpioa_handler() {
  while (1) {
  }
}
__attribute__((weak)) void gpiob_handler() {
  while (1) {
  }
}
__attribute__((weak)) void gpioc_handler() {
  while (1) {
  }
}
__attribute__((weak)) void gpiod_handler() {
  while (1) {
  }
}
__attribute__((weak)) void gpioe_handler() {
  while (1) {
  }
}
__attribute__((weak)) void uart0_handler() {
  while (1) {
  }
}
__attribute__((weak)) void uart1_handler() {
  while (1) {
  }
}
__attribute__((weak)) void ssi0_handler() {
  while (1) {
  }
}
__attribute__((weak)) void i2c0_handler() {
  while (1) {
  }
}
__attribute__((weak)) void pwm0_fault_handler() {
  while (1) {
  }
}
__attribute__((weak)) void pwm0_0_handler() {
  while (1) {
  }
}
__attribute__((weak)) void pwm0_1_handler() {
  while (1) {
  }
}
__attribute__((weak)) void pwm0_2_handler() {
  while (1) {
  }
}
__attribute__((weak)) void qei0_handler() {
  while (1) {
  }
}
__attribute__((weak)) void adc0ss0_handler() {
  while (1) {
  }
}
__attribute__((weak)) void adc0ss1_handler() {
  while (1) {
  }
}
__attribute__((weak)) void adc0ss2_handler() {
  while (1) {
  }
}
__attribute__((weak)) void adc0ss3_handler() {
  while (1) {
  }
}
__attribute__((weak)) void watchdog_handler() {
  while (1) {
  }
}
__attribute__((weak)) void timer0a_handler() {
  while (1) {
  }
}
__attribute__((weak)) void timer0b_handler() {
  while (1) {
  }
}
__attribute__((weak)) void timer1a_handler() {
  while (1) {
  }
}
__attribute__((weak)) void timer1b_handler() {
  while (1) {
  }
}
__attribute__((weak)) void timer2a_handler() {
  while (1) {
  }
}
__attribute__((weak)) void timer2b_handler() {
  while (1) {
  }
}
__attribute__((weak)) void comp0_handler() {
  while (1) {
  }
}
__attribute__((weak)) void comp1_handler() {
  while (1) {
  }
}
__attribute__((weak)) void comp2_handler() {
  while (1) {
  }
}
__attribute__((weak)) void sysctl_handler() {
  while (1) {
  }
}
__attribute__((weak)) void flash_handler() {
  while (1) {
  }
}
__attribute__((weak)) void gpiof_handler() {
  while (1) {
  }
}
__attribute__((weak)) void uart2_handler() {
  while (1) {
  }
}
__attribute__((weak)) void ssi1_handler() {
  while (1) {
  }
}
__attribute__((weak)) void timer3a_handler() {
  while (1) {
  }
}
__attribute__((weak)) void timer3b_handler() {
  while (1) {
  }
}
__attribute__((weak)) void i2c1_handler() {
  while (1) {
  }
}
__attribute__((weak)) void qei1_handler() {
  while (1) {
  }
}
__attribute__((weak)) void can0_handler() {
  while (1) {
  }
}
__attribute__((weak)) void can1_handler() {
  while (1) {
  }
}
__attribute__((weak)) void hibernate_handler() {
  while (1) {
  }
}
__attribute__((weak)) void usb0_handler() {
  while (1) {
  }
}
__attribute__((weak)) void pwm0_3_handler() {
  while (1) {
  }
}
__attribute__((weak)) void udma_handler() {
  while (1) {
  }
}
__attribute__((weak)) void udmaerr_handler() {
  while (1) {
  }
}
__attribute__((weak)) void adc1ss0_handler() {
  while (1) {
  }
}
__attribute__((weak)) void adc1ss1_handler() {
  while (1) {
  }
}
__attribute__((weak)) void adc1ss2_handler() {
  while (1) {
  }
}
__attribute__((weak)) void adc1ss3_handler() {
  while (1) {
  }
}
__attribute__((weak)) void ssi2_handler() {
  while (1) {
  }
}
__attribute__((weak)) void ssi3_handler() {
  while (1) {
  }
}
__attribute__((weak)) void uart3_handler() {
  while (1) {
  }
}
__attribute__((weak)) void uart4_handler() {
  while (1) {
  }
}
__attribute__((weak)) void uart5_handler() {
  while (1) {
  }
}
__attribute__((weak)) void uart6_handler() {
  while (1) {
  }
}
__attribute__((weak)) void uart7_handler() {
  while (1) {
  }
}
__attribute__((weak)) void i2c2_handler() {
  while (1) {
  }
}
__attribute__((weak)) void i2c3_handler() {
  while (1) {
  }
}
__attribute__((weak)) void timer4a_handler() {
  while (1) {
  }
}
__attribute__((weak)) void timer4b_handler() {
  while (1) {
  }
}
__attribute__((weak)) void timer5a_handler() {
  while (1) {
  }
}
__attribute__((weak)) void timer5b_handler() {
  while (1) {
  }
}
__attribute__((weak)) void wtimer0a_handler() {
  while (1) {
  }
}
__attribute__((weak)) void wtimer0b_handler() {
  while (1) {
  }
}
__attribute__((weak)) void wtimer1a_handler() {
  while (1) {
  }
}
__attribute__((weak)) void wtimer1b_handler() {
  while (1) {
  }
}
__attribute__((weak)) void wtimer2a_handler() {
  while (1) {
  }
}
__attribute__((weak)) void wtimer2b_handler() {
  while (1) {
  }
}
__attribute__((weak)) void wtimer3a_handler() {
  while (1) {
  }
}
__attribute__((weak)) void wtimer3b_handler() {
  while (1) {
  }
}
__attribute__((weak)) void wtimer4a_handler() {
  while (1) {
  }
}
__attribute__((weak)) void wtimer4b_handler() {
  while (1) {
  }
}
__attribute__((weak)) void wtimer5a_handler() {
  while (1) {
  }
}
__attribute__((weak)) void wtimer5b_handler() {
  while (1) {
  }
}
__attribute__((weak)) void sysexc_handler() {
  while (1) {
  }
}
__attribute__((weak)) void pwm1_0_handler() {
  while (1) {
  }
}
__attribute__((weak)) void pwm1_1_handler() {
  while (1) {
  }
}
__attribute__((weak)) void pwm1_2_handler() {
  while (1) {
  }
}
__attribute__((weak)) void pwm1_3_handler() {
  while (1) {
  }
}
__attribute__((weak)) void pwm1_fault() {
  while (1) {
  }
}
