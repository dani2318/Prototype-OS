#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef void* (SendEndOfInterrupt)(int irq);

typedef struct
{
    void *SendEndOfInterrupt(int irq);
    bool *Probe();
} PICDriver;


void i8259_Configure(uint8_t offsetPic1, uint8_t offsetPic2, bool autoEoi);
void i8259_SendEndOfInterrupt(int irq);
void i8259_Disable();
void i8259_Mask(int irq);
void i8259_Unmask(int irq);
void i8259_SetMask(uint16_t newMask);
uint16_t i8259_GetIRQRequestRegister();
uint16_t i8259_ReadInServiceRegister();