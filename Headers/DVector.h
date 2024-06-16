#pragma once
#include <iterator>
#include <cstddef>

template <typename T>
struct Iterator{

    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using type = T;
    using type_pointer = T*;
    using type_refference = T&;
    type_pointer ptr = nullptr;

    Iterator(type_pointer ptr): ptr(ptr){}

    type_refference operator*(){
        return *ptr;
    }

    type_pointer operator->(){
        return ptr;
    }

    Iterator<T>& operator++(){
        ptr++;
        return *this;
    }
    Iterator<T>& operator++(int){
        Iterator temp = *this;
        ++(*this);
        return temp;
    }

    friend bool operator==(const Iterator<T>& a,const Iterator<T>& b){
        return a.ptr == b.ptr;
    }
    
    friend bool operator!=(const Iterator<T>& a,const Iterator<T>& b){
        return a.ptr != b.ptr;
    }

};

template <typename T>
class DVector{
private:

    T* data = nullptr;
    uint32_t size = 0;
    uint32_t capacity = 0;
    void Resize(uint32_t newSize){
        T* newData = (T*)::operator new(sizeof(T) * newSize);
        for(uint32_t i = 0; i < this->size; i++){
            new(&newData[i]) T(std::move(data[i]));
        }
        this->data = newData;
        this->capacity = newSize;
    }

public:

    DVector(): capacity(3){
        this->Resize(3);
    }
    DVector(uint32_t capacity): capacity(capacity){
        this->Resize(capacity);
    }
    ~DVector(){
        ::operator delete[](this->data);
    }

    uint32_t Size() const {
        return this->capacity;
    }

    void PushBack(T&& newData){
        if(this->size >= this->capacity){
            this->Resize(this->size + 10);
        }
        new(&this->data[this->size++]) T(std::move(newData));
    }

    T& operator [](uint32_t index){
        return this->data[index];
    }

    Iterator<T> begin(){
        return Iterator(&this->data[0]);
    }

    Iterator<T> end(){
        return Iterator(&this->data[this->size]);
    }

};