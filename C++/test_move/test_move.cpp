#include <iostream>
#include <utility>
#include <vector>
#include <string>
std::vector<std::string> v;

void set_vector()
{
    std::string str = "Hello";
    v.push_back(str);
    v.push_back(std::move(str));
}

int main()
{
    set_vector();
    std::cout << "The contents of the vector are " << v[0]<< std::endl;
    std::cout << "The contents of the vector are " << v[1]<< std::endl;
}