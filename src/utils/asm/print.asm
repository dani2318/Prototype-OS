
puts:
  push si
  push ax
.loop:
  lodsb
  or al,al
  jz .done

  mov ah,0x0e
  mov bh,0
  int 0x10

  jmp .loop

.done:
  pop ax
  pop si
  ret
