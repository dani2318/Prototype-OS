#include <stdint.h>
#include "stdio.h"
// #include "disk.h"
// #include "fat.h"

// https://www.youtube.com/watch?v=db_5skZaneg&t=3412s [1:52:38]

void __attribute__((cdecl)) start(uint16_t bootDrive){

  printf("Hello from stage2!");

  /*DISK disk;
  if(!DISK_Initialize(&disk,bootDrive)){
    printf("Disk init error \r\n");
    goto end;
  }

  if(!FAT_Initialize(&disk)){
    printf("FAT init error \r\n");
    goto end;
  }

  FAT_File far* fd = FAT_Open(&disk, "/");
  FAT_DirectoryEntry entry;
  int i = 0;
  while (FAT_ReadEntry(&disk, fd, &entry) && i++ < 5)
  {
      printf("  ");
      for (int i = 0; i < 11; i++)
          putc(entry.Name[i]);
      printf("\r\n");
  }
  FAT_Close(fd);

  // read test.txt
  char buffer[100];
  uint32_t read;
  fd = FAT_Open(&disk, "mydir/test.txt");
  while ((read = FAT_Read(&disk, fd, sizeof(buffer), buffer)))
  {
      for (uint32_t i = 0; i < read; i++)
      {
          if (buffer[i] == '\n')
              putc('\r');
          putc(buffer[i]);
      }
  }
  FAT_Close(fd);

end:*/
  for(;;);
}
