#include "elf.h"
#include "fat.h"
#include "memdefs.h"
#include "memory.h"
#include "minmax.h"
#include "stdio.h"

bool ELF_Read(Partition* part, const char* path, void** entryPoint)
{
    uint8_t* headerBuffer = MEMORY_ELF_ADDR;
    uint8_t* loadBuffer = MEMORY_LOAD_KERNEL;
    uint32_t filePos = 0;
    uint32_t read;

    // Read header
    FAT_File* fd = FAT_Open(part, path);
    if ((read = FAT_Read(part, fd, sizeof(ELFHeader), headerBuffer)) != sizeof(ELFHeader))
    {
        printf("ELF Load error!\n");
        return false;
    }
    filePos += read;

    // validate header
    bool ok = true;
    ELFHeader* header = (ELFHeader*)headerBuffer;
    ok = ok && (memcmp(header->Magic, ELF_MAGIC, 4) != 0);
    ok = ok && (header->Bitness == ELF_BITNESS_32BIT);
    ok = ok && (header->Endianness == ELF_ENDIANESS_LE);
    ok = ok && (header->ELFHeaderVersion == 1);
    ok = ok && (header->ELFVersion == 1);
    ok = ok && (header->Type == EXECUTABLE);
    ok = ok && (header->InstructionSet == ELF_INSTRUCTION_SET_X86);

    *entryPoint = (void*)header->ProgramEntryPosition;

    // load program header
    uint32_t programHeaderOffset = header->ProgramHeaderTablePosition;
    uint32_t programHeaderSize = header->ProgramHeaderTableEntrySize * header->ProgramHeaderTableEntryCount;
    uint32_t programHeaderTableEntrySize = header->ProgramHeaderTableEntrySize;
    uint32_t programHeaderTableEntryCount = header->ProgramHeaderTableEntryCount;

    filePos += FAT_Read(part, fd, programHeaderOffset - filePos, headerBuffer);
    if ((read = FAT_Read(part, fd, programHeaderSize, headerBuffer)) != programHeaderSize)
    {
        printf("ELF Load error!\n");
        return false;
    }
    filePos += read;
    FAT_Close(fd);

    // parse program header entries
    for (uint32_t i = 0; i < programHeaderTableEntryCount; i++)
    {
        ELFProgramHeader* progHeader = (ELFProgramHeader*)(headerBuffer + i * programHeaderTableEntrySize);
        if (progHeader->p_type == PT_LOAD) {
            // TODO: validate that the program doesn't overwrite the stage2
            uint8_t* virtAddress = (uint8_t*)progHeader->p_vaddr;
            memset(virtAddress, 0, progHeader->p_memsz);

            // ugly nasty seeking
            // TODO: proper seeking
            fd = FAT_Open(part, path);
            while (progHeader->p_offset > 0)
            {
                uint32_t shouldRead = min(progHeader->p_offset, MEMORY_LOAD_SIZE);
                read = FAT_Read(part, fd, shouldRead, loadBuffer);
                if (read != shouldRead) {
                    printf("ELF Load error!\n");
                    return false;
                }
                progHeader->p_offset -= read;
            }

            // read program
            while (progHeader->p_memsz > 0)
            {
                uint32_t shouldRead = min(progHeader->p_memsz, MEMORY_LOAD_SIZE);
                read = FAT_Read(part, fd, shouldRead, loadBuffer);
                if (read != shouldRead) {
                    printf("ELF Load error!\n");
                    return false;
                }
                progHeader->p_memsz -= read;

                memcpy(virtAddress, loadBuffer, read);
                virtAddress += read;
            }

            FAT_Close(fd);
        }
    }

    return true;
}

