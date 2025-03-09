#include <core/dev/BlockDevice.hpp>
#include <core/Defs.hpp>
#include <stdint.h>

static inline const constexpr int SectorSize = 512;

class BIOSDisk : public BlockDevice{
public:
  BIOSDisk(uint8_t id);
  bool Initialize();
  virtual size_t Read(uint8_t* data, size_t size) override;
  virtual size_t Write(const uint8_t* data, size_t size) override;
  virtual void Seek(SeekPos pos, int rel) override;
  virtual size_t Position();
  virtual size_t Size() override;
private:

  virtual bool ReadNextSector();
  virtual void LBA2CHS(uint32_t lba,uint16_t* cylindersOut, uint16_t*sectorsOut, uint16_t* headsOut);
  uint8_t  m_id;
  bool     m_haveExtensions;
  uint16_t m_cylinders;
  uint16_t m_sectors;
  uint16_t m_heads;
  uint8_t  m_driveType;
  uint8_t  m_Buffer[SectorSize];
  uint64_t m_Pos;
  uint64_t m_Size;
};
