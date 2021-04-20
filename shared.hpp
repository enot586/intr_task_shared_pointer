#pragma once

#include <atomic>
#include <iostream>

template<class T>
struct storage
{
    std::atomic<int> counter;
    T* pointer;
};

template<class T>
class _shared_pointer
{
    public:
        _shared_pointer();
        _shared_pointer(const _shared_pointer<T>& v);
        _shared_pointer(_shared_pointer<T>&& v) = default;
        _shared_pointer(T* v);

        ~_shared_pointer();

        _shared_pointer& operator=(const _shared_pointer<T>& v);
        _shared_pointer& operator=(_shared_pointer<T>&& v) = default;

        void reset(T* v);

        T& operator*();

    private:
        void dec_ref_counter();
        storage<T>* st;
};

template<class T>
_shared_pointer<T>::_shared_pointer()
{
    st = nullptr;
    std::cout << "new default ctor" << "\n";
}

template<class T>
void _shared_pointer<T>::dec_ref_counter()
{
    if (st)
    {
        if (st->counter) {
            --st->counter;
            std::cout << "dtor: dec ref counter(" << st->counter << ")\n";

            if(!st->counter && st->pointer != nullptr)
            {
                delete st->pointer;
                st->pointer = nullptr;
                std::cout << "destroy object" << "\n";

                delete st;
                st = nullptr;
            }
        }
    }
}


template<class T>
_shared_pointer<T>::~_shared_pointer()
{
    dec_ref_counter();
}

template<class T>
_shared_pointer<T>::_shared_pointer(T* v)
{
    st = new storage<T>{ {1}, v };
    std::cout << "new pointer ctor" << "\n";
}

template<class T>
_shared_pointer<T>::_shared_pointer(const _shared_pointer<T>& v)
{
    st = v.st;

    if (st)
    {
        std::cout << "copy ctor: inc ref counter" << "\n";
        ++st->counter;
    }
}

template<class T>
_shared_pointer<T>& _shared_pointer<T>::operator=(const _shared_pointer<T>&v)
{
    st = v.st;

    if (st)
    {
        ++st->counter;
        std::cout << "operator=: inc ref counter" << "\n";
    }

    return *this;
}

template<class T>
T& _shared_pointer<T>::operator*()
{
    return *st->pointer;
}

template<class T>
void _shared_pointer<T>::reset(T* v)
{
    dec_ref_counter();

    if (!st)
    {
        st = new storage<T>{ {1}, v };
        std::cout << "reset(): new storage" << "\n";
    } else {
        st->counter = 1;
        st->pointer = v;
    }
}
