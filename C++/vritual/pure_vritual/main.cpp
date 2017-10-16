#include "Cman.h"
#include "Cchild.h"
int main(void)
{
	//Cman m_man;
	Cchild m_child;
	Cman *p ;//这才是使用的精髓，如果不定义基类的指针去使用，没有太大的意义
	//p = &m_man ;
	//p->Eat(); //始终调用CMan的Eat成员函数，不会调用 CChild 的
	p = &m_child;
	p->Eat(); //如果子类实现(覆盖)了该方法，则始终调用CChild的Eat函数,不会调用CMan 的 Eat 方法；如果子类没有实现该函数，则调用CMan的Eat函数
	p->Move(); //子类中没有该成员函数，所以调用的是基类中的
	return 0;
}
