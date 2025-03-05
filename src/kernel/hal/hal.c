#include "hal.h"
#include <stdio.h>
#include <arch/i686/irq.h>

void HAL_Initialize(){
  printf("GDT ");
  i686_GDT_Initialize();
  printf("[OK]\r\n");
  printf("IDT ");
  i686_IDT_Initialize();
  printf("[OK]\r\n");

  printf("ISR ");
  i686_ISR_Initialize();
  printf("[OK]\r\n");

  printf("IRQ ");
  i686_IRQ_Initialize();
  printf("[OK]\r\n");

}
