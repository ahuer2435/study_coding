#ifndef CMAN_H
#define CMAN_H
class Cman
{
public:
virtual void Eat()=0;	//纯虚函数，虚基类，不可直接实例化对象；与子类相当于一个借口，一般没有数据。动物和狗的关系。
void Move();
private:
};
#endif
