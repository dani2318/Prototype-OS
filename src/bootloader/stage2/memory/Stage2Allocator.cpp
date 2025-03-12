#include "Stage2Allocator.hpp"

Stage2Allocator::Stage2Allocator(void *base, uint32_t limit)
  : m_base(reinterpret_cast<uint8_t*>(base)), m_limit(limit)
{
}

void *Stage2Allocator::Allocate(size_t size)
{

  if (m_allocated + size >= m_limit)
    return nullptr;

  void* ret = m_base + m_allocated;
  m_allocated += size;
  return ret;
}

void Stage2Allocator::Free(void *addr)
{
  // NO-OP
}
