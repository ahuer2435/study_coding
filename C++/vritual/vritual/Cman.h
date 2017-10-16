#ifndef CMAN_H
#define CMAN_H
class Cman
{
public:
virtual void Eat();			//虚函数，但不是纯虚函数。所以可以实例化，与子类是继承关系，狗和旺财的关系 。可以有数据
void Move();
private:
};
#endif
