#pragma once
#include <stddef.h>
#include <stdint.h>

#include <core/memory/Allocator.hpp>

class Stage2Allocator : public Allocator{
  public:
    Stage2Allocator(void* base, uint32_t limit);
    virtual void* Allocate(size_t size)override;
    virtual void Free(void* addr)override;
  private:
    uint8_t* m_base;
    uint32_t m_allocated;
    uint32_t m_limit;
};
