#include <stdint.h>
#include <stdio.h>
#include "memory.h"
#include <hal/hal.h>
#include <arch/i686/irq.h>


extern void _init();

void timer(Registers* regs){
  printf(".");
}

void start(uint16_t bootDrive){

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
