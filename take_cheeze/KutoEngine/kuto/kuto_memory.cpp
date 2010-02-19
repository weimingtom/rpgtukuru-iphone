/**
 * @file
 * @brief Memory Manager
 * @author project.kuto
 */

#include <cstdio>
#include <cstdlib>
#include <string.h>
#include "kuto_memory.h"
#include "kuto_error.h"


namespace kuto {

static Memory sMemory;

Memory* Memory::instance()
{
	return &sMemory;
}

Memory::Memory()
: totalSize_(0)
{
	memset(allocSize_, 0, sizeof(allocSize_));
	memset(allocCount_, 0, sizeof(allocCount_));
}

void* Memory::allocImpl(AllocType type, int size)
{
	totalSize_ += size;
	allocSize_[type] += size;
	allocCount_[type]++;
	char* mem = (char*)malloc(size + 8);
	*(int*)(mem) = (int)type;
	*(int*)(mem + 4) = size;
	return mem + 8;
}

void Memory::deallocImpl(AllocType type, void* mem)
{
	char* realMem = (char*)mem - 8;
	kuto_assert((int)type == *(int*)(realMem));
	int size = *(int*)(realMem + 4);
	totalSize_ -= size;
	allocSize_[type] -= size;
	allocCount_[type]--;
	free(realMem);
}

void Memory::print()
{
	kuto_printf("total   : %8d bytes¥n", totalSize_);
	kuto_printf("  alloc : %8d bytes / %6d counts¥n", allocSize_[kAllocTypeAlloc], allocCount_[kAllocTypeAlloc]);
	kuto_printf("  new   : %8d bytes / %6d counts¥n", allocSize_[kAllocTypeNew], allocCount_[kAllocTypeNew]);
	kuto_printf("  new[] : %8d bytes / %6d counts¥n", allocSize_[kAllocTypeNewArray], allocCount_[kAllocTypeNewArray]);	
}

}	// namespace kuto



void* operator new(size_t size)
{
    return kuto::Memory::instance()->allocImpl(kuto::Memory::kAllocTypeNew, size);
}

void operator delete(void* mem)
{
    kuto::Memory::instance()->deallocImpl(kuto::Memory::kAllocTypeNew, mem);
}

void* operator new[](size_t size)
{
    return kuto::Memory::instance()->allocImpl(kuto::Memory::kAllocTypeNewArray, size);
}

void operator delete[](void* mem)
{
    kuto::Memory::instance()->deallocImpl(kuto::Memory::kAllocTypeNewArray, mem);
}
