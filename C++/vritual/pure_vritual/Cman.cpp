#include "Cman.h"
#include <iostream>

using namespace std;
void Cman::Eat(){						//纯虚函数在虚基类可以实现，但是毫无意义，因为不能实例化其对象，永远也不会调用
	cout << "man eat."<< endl;		
}

void Cman::Move(){
	cout << "man move" << endl;
}