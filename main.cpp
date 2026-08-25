/**
 * RGL - Regular Library Verification Suite
 * Copyright (C) 2026 [Isra-222]
*/

#include "rgl/string/stringSwitch.h"

#include "rgl/algorithm/minmax.h"
#include "rgl/algorithm/search.h"
#include "rgl/algorithm/sort.h"
#include "rgl/algorithm/modifiers.h"

#include "rgl/core/types.h"
#include "rgl/allocator/pool.h"
#include "rgl/allocator/linear.h"
#include "rgl/allocator/stack.h"
#include "rgl/containers/vector.h"
#include "rgl/containers/smallVector.h"
#include "rgl/containers/map.h"
#include "rgl/string/string.h"
#include "rgl/io/iostream.h"
#include "rgl/io/ofstream.h"
#include "rgl/math/random.h"
#include "rgl/async/thread.h"
#include "rgl/async/atomic.h"
#include "rgl/async/mutex.h"
#include "rgl/memory/unique_ptr.h"
#include "rgl/time/anemo.h"
#include "rgl/core/utility/function.h"

#include "rgl/math/calculus.h"
#include "rgl/math/trig.h"

void test_rgl_algorithms() {
    rgl::vector<int> v;
    v.push_back(10); v.push_back(20); v.push_back(30); v.push_back(40); v.push_back(50);
    
    auto it = rgl::algo::find(v.begin(), v.end(), 30);
    if (it != v.end()) {
        rgl::out << "Find: Element 30 found." << rgl::endl;
    }

    rgl::vector<int> v2;
    v2.push_back(5); v2.push_back(2); v2.push_back(9); 
    v2.push_back(1); v2.push_back(5); v2.push_back(6);
    
    rgl::algo::sort(v2.begin(), v2.end());
    rgl::out << "Sort: ";
    for(size_t i = 0; i < v2.size(); ++i) rgl::out << v2[i] << " ";
    rgl::out << rgl::endl;

    rgl::vector<int> v3;
    v3.push_back(1); v3.push_back(2); v3.push_back(3); 
    v3.push_back(4); v3.push_back(5);
    
    rgl::algo::rotate(v3.begin(), v3.begin() + 2, v3.end());
    rgl::out << "Rotate (expected: 3,4,5,1,2): ";
    for(size_t i = 0; i < v3.size(); ++i) rgl::out << v3[i] << " ";
    rgl::out << rgl::endl;

    rgl::out << "Max (10, 20): " << rgl::algo::max(10, 20) << rgl::endl;
    rgl::out << "Min (10, 20): " << rgl::algo::min(10, 20) << rgl::endl;
}

struct TestObject { 
    int id; 
    float value; 
};

rgl::atomic<int> shared_counter(0);
rgl::mutex mtx;
rgl::condition_variable cv;
bool ready = false;

void worker_task(int id) {
    shared_counter.fetch_add(1);
    mtx.lock();
    rgl::out << "Thread " << id << " is processing." << rgl::endl;
    mtx.unlock();
    mtx.lock();
    while(!ready) { ready = true; cv.notify_one(); }
    mtx.unlock();
}

void test_sync() {
    rgl::thread t1(worker_task, 1);
    rgl::thread t2(worker_task, 2);
    t1.join();
    t2.join();
    rgl::out << "Final counter (expected: 2): " << shared_counter.load() << rgl::endl;
}

void test_allocators() {
    rgl::linearAllocator linear(1024);
    int* data = linear.push<int>(sizeof(int) * 10);
    if (data) rgl::out << "Linear: Memory reserved successfully." << rgl::endl;
    linear.reset();

    rgl::PoolAllocator<TestObject> pool(5);
    TestObject* obj1 = pool.allocate();
    obj1->id = 1;
    rgl::out << "Pool: Object allocated with ID: " << obj1->id << rgl::endl;
    pool.deallocate(obj1);
    rgl::out << "Pool: Object deallocated." << rgl::endl;

    rgl::stackAllocator stack(1024);
    int* val1 = stack.push<int>(sizeof(int));
    *val1 = 100;
    double* val2 = stack.push<double>(sizeof(double));
    *val2 = 3.14;
    rgl::out << "Stack: Elements allocated. Performing pop operations..." << rgl::endl;
    stack.pop();
    stack.pop();
    rgl::out << "Stack: Pop operations completed successfully." << rgl::endl;
}

void test_containers() {
    rgl::array<int, 3> arr;
    arr[0] = 1; arr[1] = 2; arr[2] = 3;
    rgl::out << "Array: " << arr[0] << ", " << arr[1] << ", " << arr[2] << rgl::endl;

    rgl::vector<int> vec;
    vec.push_back(10); vec.push_back(20);
    rgl::out << "Vector: " << vec[0] << " (size: " << vec.size() << ")" << rgl::endl;

    rgl::smallVector<int, 4> svec;
    svec.push_back(100); svec.push_back(200);

    rgl::map<int, const char*> myMap;
    myMap.insert(1, "One");
    myMap.insert(2, "Two");
    auto it = myMap.find(1);
    if (it != myMap.end()) {
        rgl::out << "Map: Key 1 found -> " << it->second << rgl::endl;
    }
    enum class Enumeration : rgl::uint8_t {
        Zero = 0,
        One,
        Two,
        Three
    };
    rgl::map<Enumeration, rgl::uint32_t> NumberMap;
    NumberMap[Enumeration::Zero] = 0;
}

float square(float x) { return x * x; }

void test_math_and_random() {
    const float PI = 3.14159265f;
    float rad = 60.0f * (PI / 180.0f); // 60 degrees to radians
    
    rgl::out << "Sin(60 degrees / 1.047 rad): " << rgl::math::sin(rad) << rgl::endl;
}

void test_random_features() {
    rgl::out << "--- Starting Random Features Test Suite ---" << rgl::endl;
    
    int rand_int = rgl::random(1, 10);
    rgl::out << "Random int [1, 10]: " << rand_int << rgl::endl;

    float rand_float = rgl::random(0.0f, 1.0f);
    rgl::out << "Random float [0.0, 1.0]: " << rand_float << rgl::endl;
    
    int arr[] = {1, 2, 3, 4, 5};
    rgl::shuffle(arr);
    rgl::out << "Array shuffled: ";
    for(int x : arr) rgl::out << x << " ";
    rgl::out << rgl::endl;
    
    rgl::out << "-------------------------------------------" << rgl::endl;
}
void print_license_header() {
    rgl::out << "RGL - Regular Library Verification Suite" << rgl::endl;
    rgl::out << "Copyright (C) 2026 Isra-222" << rgl::endl;
    rgl::out << "This program comes with ABSOLUTELY NO WARRANTY." << rgl::endl;
    rgl::out << "This is free software under the GNU LGPL license." << rgl::endl;
    rgl::out << "--------------------------------------------------" << rgl::endl;
}

int test_string_switch(rgl::string_view value){
    return rgl::stringSwitch<int>(value)
    .Case("a", 1)
    .Case("ab", 2)
    .Case("a", 3)
    .Case("ccc", 8)
    .Default(0);
}

int main() {
    rgl::string noc("XD", 3);
    print_license_header();
    auto start = rgl::anemo::steady_clock::now();
    rgl::out << "Initializing RGL Library Test Suite..." << rgl::endl;
    rgl::out << "Typing a number and we can add one :" << rgl::endl;
    int number;
    rgl::in >> number;
    rgl::out << "Result is: " << ++number << rgl::endl;

    test_rgl_algorithms();
    test_sync();
    test_allocators();
    test_containers();
    test_math_and_random();
    test_random_features();
    int x = test_string_switch("noc");

    rgl::out << "Testing function" << rgl::endl;
    rgl::function<float(float)> callback;
    callback = square;
    rgl:: out << "Squere (2) from callback: " << static_cast<rgl::uint32_t>(callback(2)) << rgl::endl;

    rgl::out << "Value of x: " << x << rgl::endl;

    auto end = rgl::anemo::steady_clock::now();
    auto us = rgl::anemo::duration_cast<rgl::anemo::microseconds>(end - start);

    rgl::out << "-------------------------------------------" << rgl::endl;
    rgl::out << "Test suite completed in: " << us.count() << " us" << rgl::endl;
    rgl::out << "-------------------------------------------" << rgl::endl;
    
    return 0;
}