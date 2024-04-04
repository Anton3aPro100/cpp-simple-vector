// вставьте сюда ваш код для класса SimpleVector
// внесите необходимые изменения для поддержки move-семантики

#pragma once

#include "array_ptr.h"

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <stdexcept>
#include <utility>
using namespace std;

class ReserveProxyObj{
    public: 
    ReserveProxyObj( size_t capacity_to_reserve){
        capacity_=capacity_to_reserve;
    }
    size_t GetCapacity(){
        return capacity_;
    }
    private:
       size_t capacity_;
};


ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}; 



template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size):
        ptr_(size),
        size_(size),
        capacity_(size)
    {
        fill(ptr_.Get(),ptr_.Get()+size_,Type());
    }
    
  
    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value):
        ptr_(size),
        size_(size),
        capacity_(size)
    {   
        fill(ptr_.Get(),ptr_.Get()+size_,value);
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) :
        ptr_(init.size()),
        size_(init.size()),
        capacity_(init.size())
    {
        copy(init.begin(),init.end(),ptr_.Get());        
    }
    
    SimpleVector(ReserveProxyObj proxy):
        ptr_(proxy.GetCapacity()),
        size_(0),
        capacity_(proxy.GetCapacity())
    {
       
      
    }
    
    SimpleVector(const SimpleVector& other):
        ptr_(other.GetCapacity()),
        size_(other.GetSize()),
        capacity_(other.GetCapacity())
    {
        copy(other.ptr_.Get(),other.ptr_.Get()+other.GetSize(),ptr_.Get());
       
    }
    
    SimpleVector(SimpleVector&& other):
        ptr_(other.GetCapacity()),
        size_(other.GetSize()),
        capacity_(other.GetCapacity())
    {
        for (size_t i=0;i<size_;++i){
           ptr_[i]=move(other[i]);    
        }
        other.Clear();
        
    }
    
    SimpleVector& operator=(const SimpleVector& rhs) {
        size_t new_size=rhs.GetSize();
        if (capacity_>=new_size){
           copy(rhs.ptr_.Get(),rhs.ptr_.Get()+new_size,ptr_.Get());
           size_=new_size;
        }
        else{
            SimpleVector temp_vector(rhs);
            ptr_.swap(temp_vector.ptr_);
            size_=new_size;
            capacity_=new_size;
            
            
        }
        return *this;
    }
    
    
 

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
       
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {

        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return (size_==0);
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        return ptr_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        // Напишите тело самостоятельно
        return ptr_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (index>=size_){
            throw out_of_range("out_of_range! index >= size");
        }
        return ptr_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        return ptr_[index];
       
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        size_=0;
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
       if (new_size<size_){
           size_=new_size;
       }
       if (new_size>size_){
           if (new_size<=capacity_){
                for (size_t i=size_;i<new_size;++i){
                    ptr_[i]=Type();    
                }
                size_=new_size;
           }
           else{
               size_t new_capacity=max(capacity_*2,new_size);
               ArrayPtr<Type> ptr_new(new_capacity);
               for (size_t i=0;i<size_;++i){
                   ptr_new[i]=move(ptr_[i]);    
               }
               for (size_t i=size_;i<new_size;++i){
               ptr_new[i]=Type();    
               }
               ptr_.swap(ptr_new);
               capacity_=new_capacity;
               size_=new_size;
           
           }
       }
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        return Iterator(ptr_.Get());
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        return Iterator(ptr_.Get())+size_;
        
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        return ConstIterator(ptr_.Get());
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        return ConstIterator(ptr_.Get())+size_;
       
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        return ConstIterator(ptr_.Get());
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        return ConstIterator(ptr_.Get())+size_;
        
    }
    
    

    

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type& item) {
        if (capacity_==size_){
            size_t new_capacity=max(capacity_*2,static_cast<size_t>(1));
            ArrayPtr<Type> ptr_new(new_capacity);
            for (size_t i=0;i<size_;++i){
                ptr_new[i]=ptr_[i];    
            }
            ptr_new[size_]=item;
            ptr_.swap(ptr_new);
            capacity_=new_capacity;            
        }
        else {
        ptr_[size_]=item;
        }
        ++size_;
    }
    
        void PushBack(Type&& item) {
        if (capacity_==size_){
            size_t new_capacity=max(capacity_*2,static_cast<size_t>(1));
            ArrayPtr<Type> ptr_new(new_capacity);
            for (size_t i=0;i<size_;++i){
                ptr_new[i]=move(ptr_[i]);    
            }
            ptr_new[size_]=move(item);
            ptr_.swap(ptr_new);
            capacity_=new_capacity;            
        }
        else {
        ptr_[size_]=move(item);
        }
        ++size_;
    }

    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, const Type& value) {
        
        if (distance(ConstIterator(ptr_.Get()),pos)<0){
            throw out_of_range("out_of_range! position is before begin!");
        }
        size_t num=distance(ConstIterator(ptr_.Get()),pos);
        
        if (capacity_==size_){
            size_t new_capacity=max(capacity_*2,static_cast<size_t>(1));
            ArrayPtr<Type> ptr_new(new_capacity);
            for (size_t i=0;i<num;++i){
                ptr_new[i]=ptr_[i];    
            }
            ptr_new[num]=value;
            for (size_t i=num;i<size_;++i){
                ptr_new[i+1]=ptr_[i];    
            }           
            ptr_.swap(ptr_new);
            capacity_=new_capacity;            
        }
        else {
            Iterator first=Iterator(ptr_.Get());
            advance(first,num);
            Iterator last=Iterator(ptr_.Get());
            advance(last,size_);
            Iterator dest=last;
            ++dest;
            copy_backward(first,last,dest);
            ptr_[num]=value;
        }
        ++size_;
        
        return Iterator(ptr_.Get())+num;
        
    }
    
    Iterator Insert(ConstIterator pos, Type&& value) {
        if (distance(ConstIterator(ptr_.Get()),pos)<0){
            throw out_of_range("out_of_range! position is before begin!");
        }
        size_t num=distance(ConstIterator(ptr_.Get()),pos);
        if (capacity_==size_){
            size_t new_capacity=max(capacity_*2,static_cast<size_t>(1));
            // когда я попробовал заменить статик_каст на 1u выдал такую ошибку и я вернул обратно:
          // /exposed/submission/simple_vector.h:318:36: error: no matching function for call to ‘max(size_t, unsigned int)’
//  318 |             size_t new_capacity=max(capacity_*2,1u);
            ArrayPtr<Type> ptr_new(new_capacity);
            
            for (size_t i=0;i<num;++i){
                ptr_new[i]=move(ptr_[i]);    
            }
            ptr_new[num]=move(value);
            for (size_t i=num;i<size_;++i){
                ptr_new[i+1]=move(ptr_[i]);    
            }           
            ptr_.swap(ptr_new);
            capacity_=new_capacity;            
        }
        else {
            Iterator first=Iterator(ptr_.Get());
            advance(first,num);
            Iterator last=Iterator(ptr_.Get());
            advance(last,size_);
            Iterator dest=last;
            ++dest;
            move_backward(first,last,dest);
                  
            ptr_[num]=move(value);
        }
        ++size_;
        return Iterator(ptr_.Get())+num;
        
    }

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
        --size_;
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        size_t num=distance(ConstIterator(ptr_.Get()),pos)+1;
        for (size_t i=num; i<=size_;++i){
            ptr_[i-1]=move(ptr_[i]);
        }
        --size_;
        
        Iterator iter=Iterator(ptr_.Get());
        --num;
        advance(iter,num);
        return iter;
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        ptr_.swap(other.ptr_);
        std::swap(size_,other.size_); 
        std::swap(capacity_,other.capacity_); 
        
    }
    void Reserve(size_t new_capacity){
        if (new_capacity>capacity_){
           ArrayPtr<Type> ptr_new(new_capacity);
           for (size_t i=0;i<size_;++i){
                ptr_new[i]=ptr_[i];    
           }
        ptr_.swap(ptr_new); 
        capacity_=new_capacity;
        }
    
    }
    
    private: 
    ArrayPtr<Type> ptr_;
    size_t size_=0;
    size_t capacity_=0;
};

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return (equal(lhs.begin(),lhs.end(),rhs.begin(),rhs.end()));
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs==rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return lexicographical_compare(lhs.begin(),lhs.end(),rhs.begin(),rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
   return (lhs<rhs||lhs==rhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return  !(lhs<=rhs);
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs<rhs);  
} 