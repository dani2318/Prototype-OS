#include "irq.h"
#include "i8259.h"
#include "io.h"
#include <stddef.h>
#include <stdio.h>

#define PIC_REMAP_OFFSET 0x20

IRQHandler g_IRQHandlers[16];


void i686_IRQ_Handler(Registers* regs){
    int irq = regs->interrupt - PIC_REMAP_OFFSET;

    uint8_t pic_isr = i8259_ReadInServiceRegister();
    uint8_t pic_irr = i8259_GetIRQRequestRegister();

    if(g_IRQHandlers[irq] != NULL){
        g_IRQHandlers[irq](regs);        
    }else{
        printf("Unhandled IRQ %d  ISR=%x  IRR=%x...\n", irq, pic_isr, pic_irr);    }
    
    i8259_SendEndOfInterrupt(irq);
}

void i686_IRQ_Initialize(){
    i8259_Configure(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8, true);
    for(int i = 0; i < 16; i++)
        i686_ISR_RegisterHandler(PIC_REMAP_OFFSET + i , i686_IRQ_Handler);
    i686_EnableInterrupts();
}

void i686_IRQ_RegisterHandler(int irq, IRQHandler handler){
    g_IRQHandlers[irq] = handler;
}
