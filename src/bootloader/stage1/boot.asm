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
%include "disk.asm"
%include "data.asm"

times 510-30-($-$$) db 0

stage2_location:        times 30 db 0

dw 0AA55h

buffer:
