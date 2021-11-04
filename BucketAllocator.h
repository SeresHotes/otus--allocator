#include <iostream>
#include <exception>
#include <string>

template<typename T, size_t BucketSize>
class BucketAllocator {
public:
    using value_type = T;

    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    template<typename U>
    struct rebind {
        using other = BucketAllocator<U, BucketSize>;
    };

    BucketAllocator() : index(0) {
        void* p = malloc(BucketSize * sizeof(T));
        if (!p) {
            throw std::bad_alloc();
        }
        bucket = reinterpret_cast<T*>(p);
    };
    ~BucketAllocator() = default;

    template<typename U>
    BucketAllocator(const BucketAllocator<U, BucketSize>&) {

    }

    T* allocate(std::size_t n, const void * hint = 0) {
        (void)hint;

        if (index + n > BucketSize) {
            throw std::bad_alloc();
        }
        
        T* ret = &bucket[index];
        index += n;
        return ret;
    }

    void deallocate(T*, std::size_t) {
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) {
        new(p) U(std::forward<Args>(args)...);
    };

    void destroy(T *p) {
        p->~T();
    }
private:
    T* bucket;
    size_t index;
};

