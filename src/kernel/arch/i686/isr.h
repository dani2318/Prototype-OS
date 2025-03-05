#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

typedef struct {
  uint32_t ds;                                          // data segment pushed by us
  uint32_t edi, esi, ebp, kern_esp, ebx, edx, ecx, eax; // pusha
  uint32_t interrupt, error;                            // interrupt and error code pushed before
  uint32_t eip, cs, eflags, esp, ss;                    // pushed automatically by CPU
} __attribute__((packed)) Registers;

typedef void (*ISRHandler)(Registers* regs);

void i686_ISR_Initialize();
void i686_ISR_RegisterHandler(int interrupt, ISRHandler handler);
