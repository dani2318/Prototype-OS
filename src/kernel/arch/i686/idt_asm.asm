[bits 32]


;void __attribute__((cdecl)) i686_IDT_Load(IDTDescriptor* descriptor);

global i686_IDT_Load
i686_IDT_Load:

  push ebp
  mov ebp,esp

  ; load gdt
  mov eax, [ebp + 8]
  lidt [eax]

  mov esp,ebp
  pop ebp
  ret
