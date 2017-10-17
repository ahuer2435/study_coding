//main.cpp

#include"AbstractFactory.h" 
#include<iostream>

using namespace std;

int main(int argc,char*argv[])
{ 
    AbstractFactory* concretefactory1  = new ConcreteFactory1(); 
    concretefactory1->CreateProductA(); 
    concretefactory1->CreateProductB(); 

    AbstractFactory* concretefactory2  = new ConcreteFactory2(); 
    concretefactory2->CreateProductA(); 
    concretefactory2->CreateProductB(); 
    return 0;
}
