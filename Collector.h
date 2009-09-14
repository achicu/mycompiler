/*
 *  Collector.h
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/14/09.
 *
 */

#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <vector>
#include "RefCounted.h"

class RegisterFile;

template<size_t bytesPerWord> struct CellSize;

template<> struct CellSize<sizeof(uint32_t)> { static const size_t m_value = 64; }; // 32-bit
template<> struct CellSize<sizeof(uint64_t)> { static const size_t m_value = 128; }; // 64-bit
const size_t BLOCK_SIZE = 16 * 4096; // 64k

const size_t BLOCK_OFFSET_MASK = BLOCK_SIZE - 1;
const size_t BLOCK_MASK = ~BLOCK_OFFSET_MASK;
const size_t MINIMUM_CELL_SIZE = CellSize<sizeof(void*)>::m_value;
const size_t CELL_ARRAY_LENGTH = (MINIMUM_CELL_SIZE / sizeof(double)) + (MINIMUM_CELL_SIZE % sizeof(double) != 0 ? sizeof(double) : 0);
const size_t CELL_SIZE = CELL_ARRAY_LENGTH * sizeof(double);
const size_t CELL_MASK = CELL_SIZE - 1;
const size_t CELL_ALIGN_MASK = ~CELL_MASK;
const size_t CELLS_PER_BLOCK = (BLOCK_SIZE * 8 - sizeof(uint32_t) * 8 - sizeof(void *) * 8 - 2 * (7 + 3 * 8)) / (CELL_SIZE * 8 + 2);
const size_t BITMAP_SIZE = (CELLS_PER_BLOCK + 7) / 8;
const size_t BITMAP_WORDS = (BITMAP_SIZE + 3) / sizeof(uint32_t);

struct CollectorBitmap {
    uint32_t bits[BITMAP_WORDS];
    bool Get(size_t n) const { return !!(bits[n >> 5] & (1 << (n & 0x1F))); } 
    void Set(size_t n) { bits[n >> 5] |= (1 << (n & 0x1F)); } 
    void Clear(size_t n) { bits[n >> 5] &= ~(1 << (n & 0x1F)); } 
    void ClearAll() { memset(bits, 0, sizeof(bits)); }
};

struct CollectorCell {
    union {
        double memory[CELL_ARRAY_LENGTH];
        struct {
            void* zeroIfFree; // this will actually contain the virtual table
            ptrdiff_t next;
        } freeCell;
    } u;
};

class Heap;

class CollectorBlock {
public:
    CollectorCell cells[CELLS_PER_BLOCK];
    uint32_t usedCells;
    CollectorCell* freeList;
    CollectorBitmap marked;
    Heap* heap;
};

class CollectorRef
{
public:
    virtual ~CollectorRef();
    virtual void Mark();
    bool IsMarked() const;
    
    void* operator new(size_t size);
};

class Heap: public RefCounted
{
public:
    Heap(RegisterFile* registerFile)
        : m_registerFile(registerFile)
    {
        s_currentHeap = this;
    }
    virtual ~Heap();
    
    void* Allocate(size_t size);
    bool Collect();

    static CollectorBlock* CellBlock(const CollectorRef* cell);
    static size_t CellOffset(const CollectorRef* cell);
    static bool IsCellMarked(const CollectorRef*);
    static void MarkCell(CollectorRef*);
    
    static Heap* CurrentHeap() { return s_currentHeap; }

private:
    void MarkRegisterFile();
    bool CreateNewBlock();
    
    RegisterFile* m_registerFile;
    
    std::vector<CollectorBlock*> m_blocks;
    static Heap* s_currentHeap;
};

inline CollectorBlock* Heap::CellBlock(const CollectorRef* cell)
{
    return reinterpret_cast<CollectorBlock*>(reinterpret_cast<uintptr_t>(cell) & BLOCK_MASK);
}

inline size_t Heap::CellOffset(const CollectorRef* cell)
{
    return (reinterpret_cast<uintptr_t>(cell) & BLOCK_OFFSET_MASK) / CELL_SIZE;
}

inline bool Heap::IsCellMarked(const CollectorRef* cell)
{
    return CellBlock(cell)->marked.Get(CellOffset(cell));
}

inline void Heap::MarkCell(CollectorRef* cell)
{
    CellBlock(cell)->marked.Set(CellOffset(cell));
}

inline bool CollectorRef::IsMarked() const
{
    return Heap::IsCellMarked(this); 
}

inline void CollectorRef::Mark()
{
    assert(!Heap::IsCellMarked(this));
    Heap::MarkCell(this);
}

inline void* CollectorRef::operator new(size_t size)
{
    return Heap::CurrentHeap()->Allocate(size);
}


#endif // COLLECTOR_H