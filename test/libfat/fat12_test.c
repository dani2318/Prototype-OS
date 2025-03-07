#include "fat12_test.h"
#include "../../src/testing/testing.h"
#include "../../src/libs/libfat/fat.h"
#include <assert.h>

void libfat_fat12_smoke_test(){
  Disk* disk;
  FAT_Initialize(&disk);

  assert(disk)
}
