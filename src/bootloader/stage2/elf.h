#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "fat.h"
#include "memdefs.h"
#include "stdio.h"
#include "memory.h"
#include "minmax.h"

#define ELF_MAGIC ("\x7F""ELF")

typedef struct {

  uint8_t Magic[4];                      // ELF_MAGIC in ASCII; "7FELF" constitute the magic number.
  uint8_t Bitness;                       // This byte is set to either 1 or 2 to signify 32- or 64-bit format, respectively.
  uint8_t Endianness;                    // This byte is set to either 1 or 2 to signify little or big endianness, respectively.
  uint8_t ELFHeaderVersion;              // Set to 1 for the original and current version of ELF.
  uint8_t ABI;                           // Identifies the target operating system ABI.
  uint8_t _Padding[8];                   // Reserved padding bytes.
  uint16_t Type;                         // Identifies object file type.
  uint16_t InstructionSet;               // Specifies target instruction set architecture.
  uint32_t ELFVersion;                   // Set to 1 for the original version of ELF.
  uint32_t ProgramEntryPosition;         // This is the memory address of the entry point from where the process starts executing. This field is either 32 or 64 bits long
  uint32_t ProgramHeaderTablePosition;   // Points to the start of the program header table.
  uint32_t SectionHeaderTablePosition;   // Points to the start of the section header table.
  uint16_t Flags;                        // Interpretation of this field depends on the target architecture
  uint16_t HeaderSize;                   // Contains the size of this header, normally 64 Bytes for 64-bit and 52 Bytes for 32-bit format.
  uint16_t ProgramHeaderTableEntrySize;  // Contains the size of a program header table entry.
  uint16_t ProgramHeaderTableEntryCount; // Contains the number of entries in the program header table.
  uint16_t SectionHeaderTableEntrySize;  // Contains the size of a section header table entry.
  uint16_t SectionHeaderTableEntryCount; // Contains the number of entries in the section header table.
  uint16_t SectionNamesIndex;            // Contains index of the section header table entry that contains the section names.

} __attribute__((packed)) ELFHeader;


enum ELF_Bitness { ELF_BITNESS_32BIT = 1, ELF_BITNESS_64BIT = 2 };
enum ELF_Endianness { ELF_ENDIANESS_LE = 1, ELF_ENDIANESS_BE = 2 };
enum ELF_Type {RELOCATABLE = 1, EXECUTABLE = 2, SHARED = 3, CORE = 4};

enum ELF_Instruction_Set{
  ELF_INSTRUCTION_SET_No_Specific 	= 0x00,
  ELF_INSTRUCTION_SET_X86 	        = 0x03,
  ELF_INSTRUCTION_SET_ARM 	        = 0x28,
  ELF_INSTRUCTION_SET_IA_64 	      = 0x32,
  ELF_INSTRUCTION_SET_x86_64        = 0x3E,
  ELF_INSTRUCTION_SET_ARM64         =	0xB7,
  ELF_INSTRUCTION_SET_RISC_V        = 0xF3
};


typedef struct {

  uint32_t p_type;                       // Identifies the type of the segment.
  uint32_t p_offset;                     // Offset of the segment in the file image.
  uint32_t p_vaddr;                      // Virtual address of the segment in memory.
  uint32_t p_paddr;                      // On systems where physical address is relevant, reserved for segment's physical address
  uint32_t p_filesz;                     // Size in bytes of the segment in the file image. May be 0.
  uint32_t p_memsz;                      // Size in bytes of the segment in memory. May be 0.
  uint32_t p_flags;                      // Segment-dependent flags (position for 32-bit structure).
  uint32_t p_align;                      // 0 and 1 specify no alignment. Otherwise should be a positive, integral power of 2, with p_vaddr equating p_offset modulus p_align

} __attribute__((packed)) ELFProgramHeader;

// Identifies the type of the segment.
enum ELF_Program_Type{
  PT_NULL    = 0,                        //	Program header table entry unused.
  PT_LOAD    = 1,                        //	Loadable segment.
  PT_DYNAMIC = 2,                        //  Dynamic linking information.
  PT_INTERP  = 3,                        //  Interpreter information.
  PT_NOTE    = 4,                        //	Auxiliary information.
  PT_SHLIB   = 5,                        //  Reserved.
  PT_PHDR    = 6,                        //  Segment containing program header table itself.
  PT_TLS     = 7,                        //  Thread-Local Storage template.

  // Reserved inclusive range. Operating system specific.
  PT_LOOS    = 0x60000000,
  PT_HIOS    = 0x6FFFFFFF,

  // Reserved inclusive range. Processor specific.
  PT_LOPROC  = 0x70000000,
  PT_HIPROC  = 0x7FFFFFFF,
};

bool ELF_Read(Partition* part, const char* path,void** entrypoint);
