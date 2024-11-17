/*

My Own Implementation of Vector Based On youtube.com/Cherno

Key Features About Vector:
* RESIZEABLE array
* heap allocated

How We Implement It:
* a pointer to heap allocated memory
* a proper way to move to new memory chunk when reaching max size
    * move instead of copy
*/

#pragma once

template<typename T>
class Vector{
public:
    Vector(){
        // start by alloc space for 2 elements
        ReAlloc(2);
    };

    ~Vector(){
        Clear();
        ::operator delete(m_Data, m_Capacity * sizeof(T));
    }

    void PushBack(const T& value){
        if(m_Size >= m_Capacity){
            ReAlloc(m_Capacity * 1.5);
        }
        m_Data[m_Size] = value;
        m_Size++;
    }

    void PushBack(T&& value){
        if(m_Size >= m_Capacity){
            ReAlloc(m_Capacity * 1.5);
        }
        m_Data[m_Size] = std::move(value);
        m_Size++;
    }

    template<typename... Args>
    T& EmplaceBack(Args&&... args){
        if(m_Size >= m_Capacity){
            ReAlloc(m_Capacity * 1.5);
        }
        new(&m_Data[m_Size]) T(std::forward<Args>(args)...);
        return m_Data[m_Size++];
    }

    void RemoveAt(size_t index){
        if(index < 0 || index >= m_Size) return;
        m_Data[index].~T();
        for(size_t i = index + 1; i < m_Size; i++){
            m_Data[i - 1] = std::move(m_Data[i]);
        }
        m_Size--;
    }

    void PopBack(){
        if(m_Size <= 0) return;
        m_Size--;
        m_Data[m_Size].~T();
    }

    void Clear(){
        for(size_t i = 0; i < m_Size; i++){
            m_Data[i].~T();
        }
        m_Size = 0;
    }

    const T& operator[](size_t index) const{
        if(index >= m_Size){
            exit(-1);
        }
        return m_Data[index];
    }

    T& operator[](size_t index){
        if(index >= m_Size){
            exit(-1);
        }
        return m_Data[index];
    }

    size_t Size() const { return m_Size; };

private:
    void ReAlloc(size_t new_Capacity){
        // 1. alloc a new block of memory
        T* new_Block = (T*)::operator new(new_Capacity * sizeof(T));

        // 2. move current data to new place
        if(new_Capacity < m_Size){
            m_Size = new_Capacity;
        }
        for (size_t i = 0; i < m_Size; i++){
            new_Block[i] = std::move(m_Data[i]);
        }

        // 3. free old memory chunk
        for(size_t i = 0; i < m_Size; i++){
            m_Data[i].~T();
        }
        ::operator delete(m_Data, m_Capacity * sizeof(T));
        m_Data = new_Block;
        m_Capacity = new_Capacity;
    }

    T* m_Data = nullptr;

    size_t m_Size = 0;
    size_t m_Capacity = 0;
};