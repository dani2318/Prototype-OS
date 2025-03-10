[bits 32]


; void __attribute__((cdecl)) i686_GDT_Load(GDTDescriptor* descriptor, uint16_t codeSegment, uint16_t dataSegment);
global i686_GDT_Load
i686_GDT_Load:
  push ebp
  mov ebp,esp

  ; load gdt
  mov eax, [ebp + 8]
  lgdt [eax]

  ; reload code segment
  mov eax, [ebp + 12]
  push eax
  push .reload_cs
  retf
.reload_cs:

  ; reload data segments
  mov ax, [ebp + 16]
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax



  mov esp,ebp
  pop ebp
  ret
