#include <iostream>
#include <exception>
#include <string>
#include <cassert>


template<typename T, size_t BucketSize>
struct Bucket {
    
    struct node {
        node* next;
    };
    T arr[BucketSize];
    node node_arr[BucketSize];
    node* free_nodes;

    void init() {
        new(node_arr) node[BucketSize];

        for (size_t i = 0; i < BucketSize - 1; i++) {
            node_arr[i].next = &node_arr[i + 1];
        }
        free_nodes = &node_arr[0];
    }
private:
    node* get_free_node() {
        if (free_nodes == nullptr) {
            throw std::bad_alloc();
        }
        node* ret = free_nodes;
        free_nodes = free_nodes->next;
        return ret;
    }
    void set_free_node(node* n) {
        n->next = free_nodes;
        free_nodes = n;
    }
    node* T_to_node(T const* n) {
        auto index = n - arr;
        assert(index >= 0 && (size_t)index < BucketSize);
        return &node_arr[index];
    }
    T* node_to_T(node const* n) {
        auto index = n - node_arr;
        assert(index >= 0 && (size_t)index < BucketSize);
        return &arr[index];
    }
public:
    T* allocate() {
        return node_to_T(get_free_node());
    }
    void deallocate(T* ptr) {
        set_free_node(T_to_node(ptr));
    }
};

template<typename T, size_t BucketSize>
class BucketOneAllocator {

private:
    
public:
    using value_type = T;

    template<typename U>
    struct rebind {
        using other = BucketOneAllocator<U, BucketSize>;
    };

    BucketOneAllocator() {
        void* p = malloc(sizeof(*bucket));
        if (!p) {
            throw std::bad_alloc();
        }
        bucket = reinterpret_cast<decltype(bucket)>(p);   
        bucket->init();     
    };
    ~BucketOneAllocator() = default;


    T* allocate(std::size_t n, const void * hint = 0) {
        assert(n == 1);
        (void)hint;

        return bucket->allocate();
    }

    void deallocate(T* ptr, std::size_t) {
        bucket->deallocate(ptr);
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) {
        new(p) U(std::forward<Args>(args)...);
    };

    void destroy(T *p) {
        p->~T();
    }
private:
    Bucket<T, BucketSize>* bucket;
};

