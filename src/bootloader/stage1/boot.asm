org 0x7C00
bits 16

%define ENDL 0x0D,0x0A


;
; FAT12 header
;

jmp short start
nop

%include 'header.asm'

start:
  mov ax,0
  mov ds,ax
  mov es,ax

  mov ss,ax
  mov sp,0x7C00

  push es
  push word .after
  retf

.after:
  ; read something from floppy disk
  ; BIOS should set DL to drive number
  mov [ebr_drive_number], dl


  mov si, msg_hello_world
  call puts

  push es
  mov  ah, 08h
  int  13h
  jc   floppy_error
  pop  es

  and  cl, 0x3F                           ; remove top 2 bits
  xor  ch, ch
  mov  [bdb_sectors_per_track], cx        ; sector count

  inc  dh
  mov  [bdb_heads], dh                    ; head_count

  ; compute LBA of root directory = reserved + fats * sectors_per_fat
  mov  ax, [bdb_sectors_per_fat]
  mov  bl, [bdb_fat_count]
  xor bh, bh
  mul  bx
  add  ax, [bdb_reserved_sectors]
  push ax

  ; compute size of root directory = (32 * number_of_entries) / bytes_per_sector
  mov ax,  [bdb_dir_entries_count]
  shl ax,  5
  xor dx,  dx
  div word [bdb_bytes_per_sector]

  test dx, dx
  jz .root_dir_after
  inc  ax

.root_dir_after:

  mov cl, al
  pop ax
  mov dl, [ebr_drive_number]
  mov bx, buffer
  call disk_read

  xor bx,bx
  mov di, buffer

.search:
  mov si, file_kernel_bin
  mov cx, 11
  push di
  repe cmpsb
  pop di
  je .found_kernel

  add di,32
  inc bx
  cmp bx, [bdb_dir_entries_count]
  jl .search

  jmp kernel_not_found

.found_kernel:

  mov ax,[di + 26]
  mov [kernel_cluster], ax

  mov ax, [bdb_reserved_sectors]
  mov bx, buffer
  mov cl, [bdb_sectors_per_fat]
  mov dl, [ebr_drive_number]
  call disk_read

  mov bx, KERNEL_LOAD_SEGMENT
  mov es, bx
  mov bx, KERNEL_LOAD_OFFSET


.load_kernel_loop:

  mov ax, [kernel_cluster]
  ; WARNING: HARD CODED NUMBER MUST BE CHANGED IF NEADED WITH A MORE EFFICENT SOLUTION!
  add ax, 31
  mov cl, 1
  mov dl, [ebr_drive_number]
  call disk_read

  add bx, [bdb_bytes_per_sector]

  mov ax, [kernel_cluster]
  mov cx, 3
  mul cx
  mov cx, 2
  div cx

  mov si, buffer
  add si, ax
  mov ax, [ds:si]

  or  dx, dx
  jz .even

.odd:
  shr ax, 4
  jmp .next_cluster_after

.even:
  and ax, 0x0FFF

.next_cluster_after:

  cmp ax, 0x0FF8
  jae .read_finish

  mov [kernel_cluster], ax
  jmp .load_kernel_loop


.read_finish:

  mov dl, [ebr_drive_number]

  mov ax, KERNEL_LOAD_SEGMENT
  mov ds, ax
  mov es, ax

  jmp KERNEL_LOAD_SEGMENT:KERNEL_LOAD_OFFSET

  jmp wait_key_and_reboot

  cli
  hlt

kernel_not_found:
  mov si, msg_load_kernel_failed
  call puts
  jmp wait_key_and_reboot

.halt:
  cli
  hlt

%include 'print.asm'
%include 'disk.asm'



msg_hello_world: db 'Loading OSDEV', ENDL,0
msg_load_kernel_failed: db 'Failed to find stage2.bin', ENDL,0
file_kernel_bin: db 'STAGE2  BIN'


kernel_cluster: dw 0


KERNEL_LOAD_SEGMENT    equ 0x2000
KERNEL_LOAD_OFFSET     equ 0

times 510-($-$$) db 0
dw 0AA55h

buffer:
