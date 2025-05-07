#pragma once
#include <core/memory/Allocator.hpp>
#include <stddef.h>

Allocator* g_cppAllocator;

void SetCppAllocator(Allocator* cppAllocator){
    g_cppAllocator = cppAllocator;
}


void *operator new(size_t size)
{
    if(g_cppAllocator == nullptr) return nullptr;
    return g_cppAllocator->Allocate(size);
}

void *operator new[](size_t size)
{
    if(g_cppAllocator == nullptr) return nullptr;
    return g_cppAllocator->Allocate(size);
}

void operator delete(void *p)
{
    if(g_cppAllocator == nullptr) return;
    g_cppAllocator->Free(p);
}

void operator delete[](void *p)
{
    if(g_cppAllocator == nullptr) return;
    g_cppAllocator->Free(p);
}
