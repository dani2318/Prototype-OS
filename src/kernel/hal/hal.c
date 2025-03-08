#include "hal.h"
#include <stdio.h>
#include <arch/i686/irq.h>

void HAL_Initialize(){
  i686_GDT_Initialize();
  i686_IDT_Initialize();
  i686_ISR_Initialize();
  i686_IRQ_Initialize();

}
