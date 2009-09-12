/*
 *  RefCounted.h
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/11/09.
 *
 */

#ifndef REFCOUNTED_H
#define REFCOUNTED_H

#include <assert.h>

class RefCounted
{
public:
    RefCounted()
        : m_refCount(1)
    {
    }
    
    virtual ~RefCounted()
    {
        assert(m_refCount == 0);
    }
    
    void Ref()
    {
        ++m_refCount;
    }
    
    int Deref()
    {
        if (--m_refCount != 0)
        {
            return m_refCount;
        }
        
        delete this;
        return 0;
    }
    
#ifndef NDEBUG
    bool HasOneRef() const { return m_refCount == 1; }
#endif

    
private:
    int m_refCount;

};


#endif // REFCOUNTED_H