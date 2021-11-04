#include <memory>
#include <iterator>


template<typename T, typename Alloc = std::allocator<T>>
class queue {
private:
    struct node {
        T val;
        node* next;

        template<typename ... Args>
        node(Args ... args) : val(std::forward<Args>(args)...), next(nullptr) {
        }
    };


    using Alloc_node = typename std::allocator_traits<Alloc>::template rebind_alloc<node>;
    using Alloc_node_traits = std::allocator_traits<Alloc_node>;
    using Alloc_traits = std::allocator_traits<Alloc>;
public:
    struct iterator {
        node* ptr;
        iterator() : ptr(nullptr) {
            
        }
        iterator(node *ptr) : ptr(ptr) {
            
        }
        iterator& operator++() {
            ptr = ptr->next; 
            return *this;
        }
        iterator operator++(int) {
            return iterator{ptr->next};
        }
        T& operator*() {
            return ptr->val;
        }
        T* operator->() {
            return &ptr->val;
        }
        friend bool operator==(iterator a, iterator b) {
            return a.ptr == b.ptr;
        }
        friend bool operator!=(iterator a, iterator b) {
            return a.ptr != b.ptr;
        }
    };


    using value_type = T;
    using allocator_type  = Alloc;
    using reference = T&;
    using const_reference = T const&;
    using size_type = typename Alloc_traits::size_type; 
    using difference_type = typename Alloc_traits::difference_type; 
    using pointer = typename Alloc_traits::pointer; 
    using const_pointer = typename Alloc_traits::const_pointer; 


    using const_iterator = const iterator;

    iterator begin() {
        return iterator{first};
    }
    iterator end() {
        return iterator();
    }

    const_iterator cbegin() {
        return first;
    }
    const_iterator cend() {
        return iterator();
    }

    template<typename ... Args>
    void emplace(Args&& ... args) {
        node* ptr = Alloc_node_traits::allocate(alloc_node, 1);
        ptr->next = nullptr;
        Alloc_node_traits::construct(alloc_node, ptr, std::forward<Args>(args)...);
        
        if (last != nullptr) {
            last->next = ptr;
        } else {
            first = ptr;
        }
        last = ptr;

        _size++;
    }
    void push(const T& val) {
        node* ptr = Alloc_node_traits::allocate(alloc_node, 1);
        ptr->next = nullptr;
        Alloc_node_traits::construct(alloc_node, ptr, val);
        
        if (last != nullptr) {
            last->next = ptr;
        } else {
            first = ptr;
        }
        last = ptr;

        _size++;
    }
    void pop() {
        assert(first == nullptr);
        node* deleting = first;
        first = first->next;
        Alloc_node_traits::destroy(alloc_node, &deleting);
        Alloc_node_traits::deallocate(alloc_node, &deleting);

        _size--;
    }

    queue() {}

    queue(const queue& q) {
        for (auto it : q) {
            push(*it);
        }
    }
    size_t size() const {
        return _size;
    }

    bool empty() const {
        return last == nullptr;
    }

    friend bool operator==(const queue& a, const queue &b) {
        if (a._size != b._size) {
            return false;
        }

        iterator ait = a.begin();
        iterator bit = b.begin();

        while (ait != a.end()) {
            if (*ait != *bit) {
                return false;
            }
            ait++;
            bit++;
        }
        return true;
    }
    
    friend bool operator!=(const queue& a, const queue &b) {
        return !(a == b);
    }
private:
    size_t _size = 0;
    node* first = nullptr;
    node* last = nullptr;
    Alloc_node alloc_node;
};