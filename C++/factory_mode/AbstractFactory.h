//AbstractFactory.h

#ifndef ABSTRACTFACTORY_H
#define ABSTRACTFACTORY_H

class AbstractProductA;
class AbstractProductB;

class AbstractFactory{                  //抽象类：含有纯虚函数的类被称为抽象类；不能直接实例对象，只能作为基类，但可以定义对象指针。
public:
    virtual ~AbstractFactory();
    virtual AbstractProductA* CreateProductA() = 0;         //纯虚函数，格式：virtual <类型><函数名>(<参数表>)=0; 
    virtual AbstractProductB* CreateProductB() = 0;
protected:
    AbstractFactory();
private:
};

class ConcreteFactory1: public AbstractFactory{
public:
    ConcreteFactory1();
    ~ConcreteFactory1();
    AbstractProductA* CreateProductA();
    AbstractProductB* CreateProductB();
protected:
private:
};

class ConcreteFactory2: public AbstractFactory{
public:
    ConcreteFactory2();
    ~ConcreteFactory2();
    AbstractProductA* CreateProductA();
    AbstractProductB* CreateProductB();
protected:
private:
};

#endif
