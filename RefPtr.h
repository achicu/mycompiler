/*
 *  RefPtr.h
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/10/09.
 *
 */

template <typename Type>
class RefPtr
{
    public:
        RefPtr() : m_ptr (0) { };
        RefPtr(Type* ptr) : m_ptr(ptr) { if (m_ptr != 0) m_ptr->Ref(); }
        RefPtr(const RefPtr& other) : m_ptr(other.m_ptr) { if (m_ptr != 0) m_ptr->Ref(); }
        
        ~RefPtr() { if (m_ptr != 0) m_ptr->Deref(); };
        
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
        
        RefPtr& operator=(Type* ptr)
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
    
        static RefPtr<Type> AdoptRef(Type* ptr)
        {
            return RefPtr(ptr, true);
        }

    private:
        RefPtr(Type* ptr, bool) : m_ptr(ptr) { } // used internally by adoptRef
        
        Type* m_ptr;
};
