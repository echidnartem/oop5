#include "include/dynamic_array.h"
#include <exception>


using namespace std;


void runProgram() {
    CustomMemoryResource mem_res;


    struct Foo { int x; std::string s; };
    std::pmr::polymorphic_allocator<Foo> allocFoo(&mem_res);
    DynamicArray<Foo> arr2(allocFoo);

    arr2.push_back(Foo{1, "Hello, "});
    arr2.push_back(Foo{2, "world!"});

    for (auto& f : arr2) std::cout << f.x << " " << f.s << "\n";



    std::pmr::polymorphic_allocator<int> alloc(&mem_res);

    DynamicArray<int> arr(alloc);

    for (int i = 1; i <= 25; ++i) arr.push_back(i);

    cout << "Elements: ";
    for (auto& x : arr) cout << x << " ";
    cout << "\n";

    cout << "arr[10] = " << arr[10] << "\n";
    cout << "arr[100] = " << arr[100] << "\n";
}

int main() {
    try {
        runProgram();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error!" << std::endl;
        return 1;
    }
    return 0;
}