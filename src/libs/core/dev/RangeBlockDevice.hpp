#pragma once
#include "BlockDevice.hpp"

class RangeBlockDevice: public BlockDevice{
public:
  RangeBlockDevice(BlockDevice* device, size_t rangeBegin, size_t rangeSize);
  void Initialize(BlockDevice* device, size_t rangeBegin, size_t rangeSize);
  virtual void Seek(SeekPos pos, int rel) override;
  virtual size_t Size() override;
  virtual size_t Position() override;
  virtual size_t Read(uint8_t* data, size_t size) override;
  virtual size_t Write(const uint8_t* data, size_t size) override;
private:
  BlockDevice* m_device;
  size_t m_rangeBegin;
  size_t m_rangeSize;
};
