org 0x7C00
bits 16

%define ENDL 0x0D, 0x0A

;
; FAT12 header
;
jmp short start
nop

%include "header.asm"

start:
    ; setup data segments
    mov ax, 0           ; can't set ds/es directly
    mov ds, ax
    mov es, ax

    ; setup stack
    mov ss, ax
    mov sp, 0x7C00              ; stack grows downwards from where we are loaded in memory

    ; some BIOSes might start us at 07C0:0000 instead of 0000:7C00, make sure we are in the
    ; expected location
    push es
    push word .after
    retf

.after:

    ; read something from floppy disk
    ; BIOS should set DL to drive number
    mov [ebr_drive_number], dl

    ; show loading message
    mov si, msg_loading
    call puts

    ;check exts
    mov ah, 0x41
    mov bx, 0x55AA
    stc
    int 13h
    
    jc .no_disk_extensions
    cmp bx, 0xAA55
    jne .no_disk_extensions

    ; ext present
    mov byte [have_exts], 1
    jmp .after_disk_extensions

.no_disk_extensions:
    mov byte [have_exts], 0

.after_disk_extensions:
    ; load stage2
    mov si, stage2_location

    mov ax, STAGE2_LOAD_SEGMENT         ; set segment registers
    mov es, ax

    mov bx, STAGE2_LOAD_OFFSET

.loop:
    mov eax, [si]
    add si, 4
    mov cl, [si]
    inc si

    cmp eax, 0
    je .read_finish

    call disk_read

    xor ch, ch
    shl cx, 5

    mov di, es
    add di, cx
    mov es, di

    jmp .loop


.read_finish:

    ; jump to our kernel
    mov dl, [ebr_drive_number]          ; boot device in dl

    mov ax, STAGE2_LOAD_SEGMENT         ; set segment registers
    mov ds, ax
    mov es, ax

    jmp STAGE2_LOAD_SEGMENT:STAGE2_LOAD_OFFSET

    jmp wait_key_and_reboot             ; should never happen

    cli                                 ; disable interrupts, this way CPU can't get out of "halt" state
    hlt


;
; Error handlers
;

%include "handlers.asm"

%include "print.asm"

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


msg_loading:            db 'Loading...', ENDL, 0
msg_stage2_not_found:   db 'STAGE2.BIN file not found!', ENDL, 0
file_stage2_bin:        db 'STAGE2  BIN'

have_exts:              db 0
extensions_dap:
    .size:              db 10h
                        db 0
    .count:             dw 0
    .segment:           dw 0
    .offset:            dw 0
    .lba:               dq 0

STAGE2_LOAD_SEGMENT     equ 0x0
STAGE2_LOAD_OFFSET      equ 0x500

times 510-30-($-$$) db 0

stage2_location:        times 30 db 0

dw 0AA55h

buffer:
