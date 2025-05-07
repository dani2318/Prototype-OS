[bits 32]

global OutB

OutB:
  mov dx, [esp + 4]
  mov al, [esp + 8]
  out dx, al
  ret

global InB

InB:
  mov dx, [esp + 4]
  xor eax, eax
  in al, dx
  ret


global Panic
Panic:
  cli
  hlt
