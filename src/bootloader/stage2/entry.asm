bits 16

section _ENTRY class=CODE

extern _cstart_
global entry

entry:
  cli
  mov ax, ds
  mov ss, ax
  mov sp, 0
  mov bp, sp
  sti
  ; https://www.youtube.com/watch?v=db_5skZaneg&list=PLFjM7v6KGMpiH2G-kT781ByCNC_0pKpPN&index=9
  ; expect boot drive in dl, send it as argument to cstart func.
  xor dh, dh
  push dx
  call _cstart_

  cli
  hlt
