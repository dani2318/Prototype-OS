global i686_outb

i686_outb:
  [bits 32]
  mov dx, [esp + 4]
  mov al, [esp + 8]
  out dx, al
  ret

global i686_inb

i686_inb:
  [bits 32]
  mov dx, [esp + 4]
  xor eax, eax
  in al, dx
  ret

global i686_Panic
i686_Panic:
  cli
  hlt


;uint8_t __attribute__((cdecl)) i686_EnableInterrupts();
;uint8_t __attribute__((cdecl)) i686_DisableInterrupts();

global i686_EnableInterrupts
i686_EnableInterrupts:
  sti
  ret


global i686_DisableInterrupts
i686_DisableInterrupts:
  cli
  ret