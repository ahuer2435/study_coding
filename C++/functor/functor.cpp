#include <iostream>
using namespace std;

class StringAppend{
  public:
    explicit StringAppend(const string & str):ss(str){}
    
    void operator()(const string &str) const{
      cout<<str<<" "<<ss<<endl;
    }
    
  private:
    const string ss;
};

int main(void)
{
  StringAppend myFunc("is world.");		//定义类函数，调用类的构造函数。
  myFunc("hello");				//调用类函数，调用重载运算符()函数。
  return 0;
}
//仿函数作用