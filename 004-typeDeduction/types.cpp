#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>
#include <memory>

// type_name() is taken from https://stackoverflow.com/a/20170989/4118460
template <class T>
std::string type_name()
{
    typedef typename std::remove_reference<T>::type TR;
    std::unique_ptr<char, void(*)(void*)> own(nullptr, std::free);
    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}

int foo(int x, int y)
{
    return x + 3*y;
}

int global = 5;
// auto bar() -> int& // C++11
int& bar()
{
    return global;
}

void test_auto_assignment()
{
    std::cout << "\n==test auto assignment==\n";
    int a = foo(1,2);
    std::cout << "type(a)=" << type_name<decltype(a)>() << std::endl;
    
    float b = foo(1,2);
    b += 0.2;
    std::cout << "type(b)=" << type_name<decltype(b)>() << std::endl;

    auto c = foo(1,2);
    c += 0.2;
    std::cout << "type(c)=" << type_name<decltype(c)>() << std::endl;
}

void test_auto_iterators()
{
    std::cout << "\n==test auto iterators==\n";

    std::vector<int> v{1,2,3,4,5};

//    for (const auto& e : v){
    for (const int& e : v){
        std::cout << e << " ";
    }
    std::cout << std::endl;

//    for (auto it = v.begin(); it != v.end(); ++it){
    for (std::vector<int>::const_iterator it = v.begin(); it != v.end(); ++it){
        auto item = *it;
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

void test_decltype_vs_auto()
{
    std::cout << "\n==test decltype vs auto==\n";
    decltype(bar()) a = bar(); // int& a = bar();
//    decltype(auto) a_ = bar(); // C++14 same as above
    auto b = bar();            // int b = bar();
    auto& c = bar();           // int& c = bar();
    std::cout << "type(a)= " << type_name<decltype(a)>() << std::endl;
    std::cout << "type(b)= " << type_name<decltype(b)>() << std::endl;
    std::cout << "type(c)= " << type_name<decltype(c)>() << std::endl;

    std::cout << "--Assigned a, b, c\n";
    std::cout << "a=" << a << std::endl;
    std::cout << "b=" << b << std::endl;
    std::cout << "c=" << c << std::endl;
    std::cout << "global=" << global << std::endl;

    std::cout << "--Changed a\n";
    a++;
    std::cout << "a=" << a << std::endl;
    std::cout << "b=" << b << std::endl;
    std::cout << "c=" << c << std::endl;
    std::cout << "global=" << global << std::endl;

    std::cout << "--Changed b\n";
    b=100;
    std::cout << "a=" << a << std::endl;
    std::cout << "b=" << b << std::endl;
    std::cout << "c=" << c << std::endl;
    std::cout << "global=" << global << std::endl;
}

std::string getName(int x)
{
    if (x == 0)
        return "zero";
    else if (x == 1)
        return "one";
    else
        return "something else";
}

int getLengthOne(const std::string& s)
{
    return s.length() + 1;
}

template <typename F, typename V>
//decltype(auto) execute(F func, const V& value) // C++14
//auto execute(F func, const V& value) -> decltype(auto) // C++14
auto execute(F func, const V& value) -> decltype(func(value)) // C++11
{
    return func(value);
}

void test_decltype_template()
{
    std::cout << "\n==test decltype template==\n";
    std::cout << execute(getLengthOne, "hello decltype") << std::endl;
    std::cout << execute(getName, 0) << std::endl;

//    std::vector< decltype(execute(getName, 1)) > v{execute(getName, 1), execute(getName, 0), execute(getName, 3)};
//    for (auto item : v){
//        std::cout << "item=" << item << std::endl;
//    }
}

int main(int argc, char** argv)
{
    std::cout << "Type deduction and definition examples" << std::endl;
//    test_auto_assignment();
//    test_auto_iterators();
//    test_decltype_vs_auto();
    test_decltype_template();

    return 0;
}
