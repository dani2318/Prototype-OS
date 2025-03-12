#include "FATFileSystem.hpp"



FATFileSystem::FATFileSystem(void* fsMemoryRegion) : m_device(nullptr){

}


bool FATFileSystem::Initialize(BlockDevice* device) {

  m_device = device;
  return true;
}

FileEntry* FATFileSystem::GetNextFileEntry(FileEntry* parent, FileEntry* previous) {

}

File* FATFileSystem::Open(FileEntry* parent,FileOpenMode mode) {

}
