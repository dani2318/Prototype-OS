#include "hal.h"
#include <arch/i686/gdt.h>
#include <arch/i686/idt.h>
#include <stdio.h>

void HAL_Initialize(){
  printf("GDT ");
  i686_GDT_Initialize();
  printf("[OK]\r\n");
  printf("IDT ");
  i686_IDT_Initialize();
  printf("[OK]\r\n");
}
