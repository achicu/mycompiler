/*
 *  Collector.cpp
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/14/09.
 *
 */

#include "Collector.h"
#include "RegisterFile.h"
#include "Interpreter.h"

#include <mach/mach_port.h>
#include <mach/mach_init.h>
#include <mach/task.h>
#include <mach/thread_act.h>
#include <mach/vm_map.h>

static CollectorBlock* allocateBlock();
static void freeBlock(CollectorBlock* block);

Heap* Heap::s_currentHeap = 0;

void* Heap::Allocate(size_t size)
{

//  To find collection errors just decomment the following line 
//  Collect();
    
    assert(size < CELL_SIZE);
    for (int i=0; i<m_blocks.size(); ++i)
    {
        CollectorBlock* const collectorBlock = m_blocks.at(i);
        CollectorCell* const freeList = collectorBlock->freeList;
        if (freeList)
        {
            assert(freeList->u.freeCell.zeroIfFree == 0);
            collectorBlock->freeList += freeList->u.freeCell.next;
             ++collectorBlock->usedCells;
            return freeList;
        }
    }
    
    // collect and try again
    
    if (!Collect())
    {
        if (!CreateNewBlock())
        {
            printf("out of memory\n");
            exit(1);
        }
    }
    
    return Allocate(size);
}

bool Heap::Collect()
{
    for (int i=0; i<m_blocks.size(); ++i)
    {
        CollectorBlock* const collectorBlock = m_blocks.at(i);
        collectorBlock->marked.ClearAll();
    }
    
    MarkRegisterFile();
    
    int numberCleaned = 0;
    
    for (int i=0; i<m_blocks.size(); ++i)
    {
        CollectorBlock* const collectorBlock = m_blocks.at(i);
        CollectorCell* lastFreeCell = 0;
        CollectorCell* blockNextFreeCell = collectorBlock->freeList;
        
        for (int i=0; i<CELLS_PER_BLOCK; ++i)
        {
            // free the item at i
            CollectorCell* const cell = &collectorBlock->cells[i];
            if (cell->u.freeCell.zeroIfFree != 0 && !collectorBlock->marked.Get(i))
            {
                CollectorRef* const refObject = reinterpret_cast<CollectorRef*>(cell);
                refObject->~CollectorRef();
                
                cell->u.freeCell.zeroIfFree = 0;
                if (lastFreeCell)
                {
                    lastFreeCell->u.freeCell.next = cell - lastFreeCell;
                } else {
                    collectorBlock->freeList = cell;
                }
                
                if (blockNextFreeCell)
                {
                    assert(blockNextFreeCell != cell);
                    if (blockNextFreeCell > cell)
                    {
                        cell->u.freeCell.next = blockNextFreeCell - cell;
                    }
                    else
                    {
                        blockNextFreeCell->u.freeCell.next = cell - blockNextFreeCell;
                        blockNextFreeCell = 0;
                    }
                }
                lastFreeCell = cell;
                --collectorBlock->usedCells;
                ++numberCleaned;
            }
        }
    }
    
    return numberCleaned > 0;
}


#define IS_HALF_CELL_ALIGNED(p) ((p & (CELL_MASK >> 1)) == 0)

void Heap::MarkRegisterFile()
{
    RegisterValue* r = m_registerFile->GetBlock();
    RegisterValue* registersEnd = m_registerFile->GetLastUsed();
    
    const size_t lastCellOffset = sizeof(CollectorCell) * (CELLS_PER_BLOCK - 1);
    
    while (r < registersEnd)
    {
        uintptr_t xAsBits = reinterpret_cast<uintptr_t>(r->asReference);
        ++r;
        
        if (IS_HALF_CELL_ALIGNED(xAsBits) && xAsBits)
        { 
            xAsBits &= CELL_ALIGN_MASK;
            uintptr_t offset = xAsBits & BLOCK_OFFSET_MASK;
            if (offset > lastCellOffset) continue;
            
            CollectorBlock* blockAddr = reinterpret_cast<CollectorBlock*>(xAsBits - offset);
            
            for (int i=0; i<m_blocks.size(); ++i)
            {
                if (m_blocks.at(i) == blockAddr)
                {
                    // mark this one
                    if (!IsCellMarked(reinterpret_cast<CollectorRef*>(xAsBits)))
                    {
                        CollectorRef* const refObject = reinterpret_cast<CollectorRef*>(xAsBits);
                        refObject->Mark();
                    }
                    continue;
                }
            }
        }
    }
}

static CollectorBlock* allocateBlock()
{
    vm_address_t address = 0;
    vm_map(current_task(), &address, BLOCK_SIZE, BLOCK_OFFSET_MASK, VM_FLAGS_ANYWHERE, MEMORY_OBJECT_NULL, 0, FALSE, VM_PROT_DEFAULT, VM_PROT_DEFAULT, VM_INHERIT_DEFAULT);
    return reinterpret_cast<CollectorBlock*>(address);
}

bool Heap::CreateNewBlock()
{
    CollectorBlock* block = allocateBlock();
    if (!block)
        return false;
    
    block->freeList = &block->cells[0];
    for (int i=0; i<CELLS_PER_BLOCK; i++)
    {
        block->cells[i].u.freeCell.zeroIfFree = 0;
        block->cells[i].u.freeCell.next = sizeof(CollectorCell);
    }
    
    m_blocks.push_back(block);
    return true;
}

Heap::~Heap()
{
    Collect();
    for (int i=0; i<m_blocks.size(); ++i)
    {
        CollectorBlock* const collectorBlock = m_blocks.at(i);
        freeBlock(collectorBlock);
    }
}

static void freeBlock(CollectorBlock* block)
{
    vm_deallocate(current_task(), reinterpret_cast<vm_address_t>(block), BLOCK_SIZE);
}

CollectorRef::~CollectorRef()
{
}

