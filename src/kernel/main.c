#include <stdint.h>
#include <stdio.h>
#include "memory.h"
#include <hal/hal.h>
#include <arch/i686/irq.h>

extern uint8_t __bss_start;
extern uint8_t __end;
extern void _init();

void timer(Registers* regs){
  printf(".");
}

void __attribute__((section(".entry"))) start(uint16_t bootDrive){

  memset(&__bss_start,0,(&__end) - (&__bss_start));
  _init(); // call global constr.

  clscr();

  printf("============       oskrnl loading!       ============\n");
  HAL_Initialize();
  printf("HAL initialized successfully!\n");

  printf("============ oskrnl loaded successfully! ============\n");
  i686_IRQ_RegisterHandler(0, timer);

end:
  for(;;);
}
