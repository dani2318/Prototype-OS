org 0x0
bits 16

%define ENDL 0x0D,0x0A

_start:
  jmp main

main:

  mov si, msg_hello_world
  call puts

  cli
  hlt

.halt:
  jmp .halt

%include 'print.asm'

msg_hello_world: db 'oskrnl.bin loaded', ENDL,0

