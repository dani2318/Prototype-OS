#include "BIOSDisk.hpp"
#include "RealMemory.hpp"
#include <core/Assert.hpp>
#include <core/cpp/Algorithm.hpp>
#include <core/Memory.hpp>

EXPORT bool ASMCALL i686_Disk_GetDriveParams(uint8_t drive,
                                              uint8_t* driveTypeOut,
                                              uint16_t* cylindersOut,
                                              uint16_t* sectorsOut,
                                              uint16_t* headsOut);

EXPORT bool ASMCALL i686_Disk_Reset(uint8_t drive);
EXPORT bool ASMCALL i686_Disk_Read(uint8_t drive,
                                    uint16_t cylinder,
                                    uint16_t sector,
                                    uint16_t head,
                                    uint8_t count,
                                    void * lowerdataOut);

EXPORT bool ASMCALL i686_Disk_HaveExtensionsPresent(uint8_t drive);

struct ExtendedReadParameters{
  uint8_t  ParamsSize;
  uint8_t  Reserved;
  uint16_t Count;
  uint32_t Buffer;
  uint64_t lba;
}__attribute__((packed));

EXPORT bool ASMCALL i686_Disk_ExtendedRead(uint8_t drive,ExtendedReadParameters* params);

struct ExtendedDriveParameters{
  uint16_t ParamSize;
  uint16_t Flags;
  uint32_t Cylinders;
  uint32_t Heads;
  uint32_t Sectors;
  uint64_t SectorPerTrack;
  uint16_t BytesPerSectors;
}__attribute__((packed));

EXPORT bool ASMCALL i686_Disk_GetExtendedDriveParams(uint8_t drive,ExtendedDriveParameters* params);

bool BIOSDisk::Initialize(){
  m_haveExtensions = i686_Disk_HaveExtensionsPresent(m_id);
  if(m_haveExtensions){

    ExtendedDriveParameters params;
    if(!i686_Disk_GetExtendedDriveParams(m_id, &params))
      return false;

    Assert(params.BytesPerSectors == SectorSize);
    m_Size = SectorSize * params.Sectors;


  }else{

     if(!i686_Disk_GetDriveParams(m_id, &m_driveType,&m_cylinders, &m_sectors,&m_heads))
      return false;

  }
  return true;
}
size_t BIOSDisk::Position(){
  return m_Pos;
}


BIOSDisk::BIOSDisk(uint8_t id): m_id(id), m_Pos(-1), m_Size(0){

}

bool BIOSDisk::ReadNextSector(){
  uint64_t lba = m_Pos / SectorSize;
  bool ok = false;

  if(m_haveExtensions){
    ExtendedReadParameters params;
    params.Reserved = 0;
    params.ParamsSize = sizeof(ExtendedDriveParameters);
    params.Count = 1;
    params.lba = lba;
    params.Buffer = ToSegOffset(m_Buffer);

    for(int i = 0; i < 3; i++)
    {
      ok = ok && i686_Disk_ExtendedRead(m_id, &params);
      if(!ok) i686_Disk_Reset(m_id);
    }

  }else{
    uint16_t cylinder,sector, head;
    LBA2CHS(lba,&cylinder,&sector, &head);
    for(int i = 0; i < 3; i++)
    {
      ok = ok && i686_Disk_Read(m_id, cylinder, sector, head, 1, m_Buffer);
      if(!ok) i686_Disk_Reset(m_id);
    }
  }

  return ok;
}

void BIOSDisk::LBA2CHS(uint32_t lba,uint16_t* cylindersOut, uint16_t*sectorsOut, uint16_t* headsOut){
  *sectorsOut = lba % m_sectors + 1;
  *cylindersOut = (lba / m_sectors) / m_heads;
  *headsOut = (lba / m_sectors) % m_heads;
}

size_t BIOSDisk::Read(uint8_t* data, size_t size) {
  size_t initialPos = m_Pos;
  if(m_Pos == -1) {ReadNextSector(); m_Pos = 0;}
  if(m_Pos >= m_Size) {return 0;}

  while(size > 0){
    size_t buffPos = m_Pos % SectorSize;
    size_t CanRead = min(size, SectorSize - buffPos);
    Memory::Copy(data,m_Buffer + buffPos, CanRead);

    size -= CanRead;
    data += CanRead;
    m_Pos += CanRead;

    if(size > 0){
      ReadNextSector();
    }
  }
  return m_Pos - initialPos;
}

size_t BIOSDisk::Write(const uint8_t* data, size_t size) {
  return 0;
}

void BIOSDisk::Seek(SeekPos pos, int rel) {
  switch (pos)
  {
    case SeekPos::Set:
      m_Pos = -1;
      break;

    case SeekPos::Current:
      m_Pos += rel;
      ReadNextSector();
      break;

    case SeekPos::End:
      m_Pos = m_Size;
      break;
  }
}

size_t BIOSDisk::Size() {
  return m_Size;
}
