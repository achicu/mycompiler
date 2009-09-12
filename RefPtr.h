/*
 *  RefPtr.h
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/10/09.
 *
 */

#ifndef REFPTR_H
#define REFPTR_H

template <typename Type>
class RefPtr;

template <typename Type>
class PassRef;

template <typename Type>
PassRef<Type> AdoptRef(Type* ptr);

template <typename Type>
class PassRef
{
        friend class RefPtr<Type>;
        friend PassRef AdoptRef<Type>(Type* ptr);
    public:
        PassRef() : m_ptr (0) { };
        PassRef(Type* ptr) : m_ptr(ptr) { if (m_ptr != 0) m_ptr->Ref(); }
        PassRef(const PassRef& other) : m_ptr(other.m_ptr) { other.m_ptr = 0; }
        ~PassRef() { if (m_ptr != 0) m_ptr->Deref(); }
        
        Type* operator->() const
        {
            return m_ptr;
        }
        
        void operator==(Type* ptr) const
        {
            return m_ptr == ptr;
        }
        
        void ClearRef()
        {
            if (m_ptr != 0)
                m_ptr->Deref();
            
            m_ptr = 0;
        }
        
        RefPtr<Type>& operator=(Type* ptr)
        {
            if (m_ptr != 0)
                m_ptr->Deref();
                
            m_ptr = ptr;
            
            if (m_ptr != 0)
                m_ptr->Ref();
            
            return *this;
        }
        
        Type* Ptr() const
        {
            return m_ptr;
        }
            
    private:
        PassRef(Type* ptr, bool) : m_ptr(ptr) { } // used internally by AdoptRef
    
        mutable Type* m_ptr;
};

template <typename Type>
class RefPtr
{
    public:
        RefPtr() : m_ptr (0) { };
        RefPtr(Type* ptr) : m_ptr(ptr) { if (m_ptr != 0) m_ptr->Ref(); }
        RefPtr(const RefPtr& other) : m_ptr(other.m_ptr) { if (m_ptr != 0) m_ptr->Ref(); }
        RefPtr(const PassRef<Type>& other) : m_ptr(other.m_ptr) { other.m_ptr = 0; }
                
        ~RefPtr() { if (m_ptr != 0) m_ptr->Deref(); };
        
        Type* operator->() const
        {
            return m_ptr;
        }
        
        void operator=(const RefPtr<Type>& other)
        {
            if (m_ptr != 0)
                m_ptr->Deref();
                
            m_ptr = other.m_ptr;
            
            if (m_ptr != 0)
                m_ptr->Ref();
            
        }
        
        void operator=(const PassRef<Type>& other) { m_ptr = other.m_ptr; other.m_ptr = 0; }
        
        bool operator==(Type* ptr) const
        {
            return m_ptr == ptr;
        }
        
        void ClearRef()
        {
            if (m_ptr != 0)
                m_ptr->Deref();
            
            m_ptr = 0;
        }
        
        RefPtr& operator=(Type* ptr)
        {
            if (m_ptr != 0)
                m_ptr->Deref();
                
            m_ptr = ptr;
            
            if (m_ptr != 0)
                m_ptr->Ref();
            
            return *this;
        }
        
        PassRef<Type> ReleaseRef()
        {
            Type* ptr = m_ptr;
            m_ptr = 0;
            return ::AdoptRef(ptr);
        }
        
        Type* Ptr() const
        {
            return m_ptr;
        }
        
        void AdoptRef(Type* ptr)
        {
            assert(m_ptr == 0);
            assert(ptr->HasOneRef());
            m_ptr = ptr;
        }

    private:
        RefPtr(Type* ptr, bool) : m_ptr(ptr) { } // used internally by AdoptRef
        
        Type* m_ptr;
};

template <typename Type>
PassRef<Type> AdoptRef(Type* ptr)
{
    return PassRef<Type>(ptr, true);
}

#endif // REFPTR_H