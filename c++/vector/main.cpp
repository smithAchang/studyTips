#include<iostream>
#include <vector>
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
  std::vector<char> v;
  v.insert('a');
  
  cout << "Hello World! Copy Constructor can use private field directly!" <<endl;
  A a;
  cout << "Construct A instance, filed value: " << a.getField() << endl;

  return 0;
}
