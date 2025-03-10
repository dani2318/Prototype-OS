#include "RangeBlockDevice.hpp"
#include <core/cpp/Algorithm.hpp>


RangeBlockDevice::RangeBlockDevice(BlockDevice* device, size_t rangeBegin, size_t rangeSize)
  : m_device(nullptr),
    m_rangeBegin(0),
    m_rangeSize(0)
{
  m_device->Seek(SeekPos::Set,rangeBegin);
}

void RangeBlockDevice::Initialize(BlockDevice* device, size_t rangeBegin, size_t rangeSize){
  m_device = device;
  m_rangeBegin = rangeBegin;
  m_rangeSize = rangeSize;
}


void RangeBlockDevice::Seek(SeekPos pos, int rel) {
  if(m_device == nullptr) return;
  switch (pos)
  {
  case SeekPos::Set:
    m_device->Seek(SeekPos::Set,m_rangeBegin+rel);
    break;
  case SeekPos::Current:
    m_device->Seek(SeekPos::Current,rel);
    break;
  case SeekPos::End:
    m_device->Seek(SeekPos::End,m_rangeBegin+m_rangeSize);
    break;
  }
}

size_t RangeBlockDevice::Size() {
  return m_rangeSize;
}

size_t RangeBlockDevice::Position() {
  return m_device->Position() - m_rangeBegin;
}

size_t RangeBlockDevice::Read(uint8_t* data, size_t size) {
  if(m_device == nullptr) return 0;
  size = min(size,Size() - Position());
  return m_device->Read(data,size);
}

size_t RangeBlockDevice::Write(const uint8_t* data, size_t size) {
  if(m_device == nullptr) return 0;
  size = min(size,Size() - Position());
  return m_device->Write(data,size);
}

