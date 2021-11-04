#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
#include <exception>
#include <map>
#include <chrono>

#include "BucketAllocator.h" 
#include "queue.h"

void map_with_default_allocator() {

    std::map<int, int> m;
    int fact = 1;
    for (int i = 0; i < 10; i++) {
        m.emplace_hint(m.end(), i, fact);
        fact *= i + 1;
    }
    std::cout << "Map with default allocator" << std::endl;
    for (const auto& [first, second] : m) {
        std::cout << first << " " << second << std::endl;
    }
}

void map_with_new_allocator() {

    std::map<int, int, std::less<int>, BucketAllocator<std::pair<const int, int>, 10>> m;
    int fact = 1;
    for (int i = 0; i < 10; i++) {
        m.emplace_hint(m.end(), i, fact);
        fact *= i + 1;
    }
    std::cout << "Map with new allocator" << std::endl;
    for (const auto& [first, second] : m) {
        std::cout << first << " " << second << std::endl;
    }
}
void queue_with_default_allocator() {

    queue<int> m;
    for (int i = 0; i < 10; i++) {
        m.emplace(i);
    }
    std::cout << "queue with default allocator" << std::endl;
    for (const auto& x : m) {
        std::cout << x << std::endl;
    }
}

void queue_with_new_allocator() {

    queue<int, BucketAllocator<int, 10>> m;
    for (int i = 0; i < 10; i++) {
        m.emplace(i);
    }
    std::cout << "queue with new allocator" << std::endl;
    for (const auto& x : m) {
        std::cout << x << std::endl;
    }
}

int main() {
    try {
        map_with_default_allocator();
        map_with_new_allocator();
        queue_with_default_allocator();
        queue_with_new_allocator();

    } catch (const std::exception& ex) {
        std::cout << "Error: " << ex.what() << std::endl;
    } catch (...) {
        std::cout << "Something really wrong" << std::endl;
    }
    return 0;
} 