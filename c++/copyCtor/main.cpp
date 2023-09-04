#include<iostream>
using namespace std;

class A
{
  public:
  A():i(0) {};

  A(const A& a):i(a.i) {};

  int getField() { return i; }

  private:
  int i;
};

int main(void)
{
  cout << "Hello World! Copy Constructor can use private field directly!" <<endl;
  A a;
  cout << "Construct A instance, filed value: " << a.getField() << endl;

  return 0;
}
