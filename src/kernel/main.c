#include <stdint.h>
#include "stdio.h"
#include "memory.h"
#include <hal/hal.h>

extern uint8_t __bss_start;
extern uint8_t __end;

// https://www.youtube.com/watch?v=FD48Lx7Rnx0

void __attribute__((section(".entry"))) start(uint16_t bootDrive){

  memset(&__bss_start,0,(&__end) - (&__bss_start));
  clscr();

  printf("============       oskrnl loading!       ============\n");
  HAL_Initialize();
  printf("HAL initialized successfully!\n");

  printf("============ oskrnl loaded successfully! ============\n");

  __asm("int $0x2");
  __asm("int $0x3");
  __asm("int $0x4");
end:
  for(;;);
}
