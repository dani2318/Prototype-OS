;
; THIS FILE CONTAINS ALL THE ASSEMBLY
; DISK ROUTINES
;
%define ENDL 0x0D,0x0A


;
; Converts an LBA addr. to a CHS addr.
;   Params:
;       -  ax: LBA addr.
;   Returns
;       -  cx [bits 0 - 5] :  sector number
;       -  cx [bits 6 - 15]:  cylinder
;       -  dh              :  head
;
lba_to_chs:

  push ax
  push dx

  xor dx, dx                        ; dx = 0
  div word [bdb_sectors_per_track]  ; ax = LBA / Sectors_per_track
                                    ; dx = LBA % Sectors_per_track
  inc dx                            ; dx = (LBA % Sectors_per_track + 1) = sector
  mov cx, dx                        ; cx = sectors

  xor dx,dx                         ; dx = 0
  div word [bdb_heads]              ; ax = (LBA / Sectors_per_track) / Heads = cylinder
                                    ; dx = (LBA / Sectors_per_track) % Heads = head
  mov dh, dl                        ; dh = head
  mov ch, al                        ; ch = cylinder (lower 8 bits)
  shl ah, 6
  or  cl, ah                        ; puts upper 2 bits of cylinder in CL

  pop ax
  mov dl, al                        ; restore dl
  pop ax
  ret



;
; Reads from disk.
;   Params:
;       -  eax: LBA addr.
;       -  cl: numbers of sectors to read (up to 128).
;       -  dl: drive number
;       -  es:bx: memory address where to store read data
;
disk_read:

  push eax
  push bx
  push cx
  push dx
  push si
  push di

  cmp byte[have_exts], 1
  jne .no_exts

  ;with exts
  mov [extensions_dap.lba], eax
  mov [extensions_dap.segment], es
  mov [extensions_dap.offset], bx
  mov [extensions_dap.count], cl

  mov ah, 0x42
  mov si, extensions_dap
  mov di, 3
  jmp .retry

.no_exts:
  push cx                          ; temporarily save CL (number of sectors to read)
  call lba_to_chs                  ; compute CHS
  pop  ax                          ; AL = number of sectors to read

  mov  ah, 02h
  mov di, 3

.retry:
  pusha                            ; save all registers, we don't know what bios modifies
  stc                              ; set carry flag, some BIOS'es don't set it
  int 13h                          ; carry flag cleared = success
  jnc .done                        ; jump if carry not set

  popa
  call disk_reset

  ; disk read failed
  dec di
  test di,di
  jnz .retry

.fail:
  jmp floppy_error

.done:
  popa

  pop di
  pop si
  pop dx
  pop cx
  pop bx
  pop eax
  ret

;
; Reads from disk.
;   Params:
;       -  dl: drive number
;
disk_reset:
  pusha
  mov ah, 0
  stc
  int 13h
  jc floppy_error
  popa
  ret



msg_read_failed:      db 'Disk read failed!', ENDL, 0
