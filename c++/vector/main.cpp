#include<iostream>
#include <vector>
#include <iterator>

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

/*explicitly instantiate template*/
template class std::vector<char>;

int main(void)
{
  std::vector<char> v;
  v.push_back('a');
  
  cout << "Hello World! no-implicit-templates ,item: " << v[0] << endl;

  cout << "Hello World! Copy Constructor can use private field directly!" <<endl;
  A a;
  cout << "Construct A instance, filed value: " << a.getField() << endl;

  return 0;
}
