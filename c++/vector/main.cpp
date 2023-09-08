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

/*template inst*/
template class std::vector<char>;
template class std::allocator<char>;
template class std::initializer_list<char>;
template class __gnu_cxx::__normal_iterator<char const*, std::vector<char, std::allocator<char> >>;

//template std::vector<char, std::allocator<char> >& std::vector<char, std::allocator<char> >::operator=(std::initializer_list<char>);
template char* std::__niter_base<char*, std::vector<char, std::allocator<char> > >(__gnu_cxx::__normal_iterator<char*, std::vector<char, std::allocator<char> > >);
template char* std::copy<std::move_iterator<char*>, char*>(std::move_iterator<char*>, std::move_iterator<char*>, char*);
template char const* std::__niter_base<char const*, std::vector<char, std::allocator<char> > >(__gnu_cxx::__normal_iterator<char const*, std::vector<char, std::allocator<char> > >);
template char* std::copy<std::move_iterator<__gnu_cxx::__normal_iterator<char*, std::vector<char, std::allocator<char> > > >, char*>(std::move_iterator<__gnu_cxx::__normal_iterator<char*, std::vector<char, std::allocator<char> > > >, std::move_iterator<__gnu_cxx::__normal_iterator<char*, std::vector<char, std::allocator<char> > > >, char*);
template void std::vector<char, std::allocator<char> >::_M_assign_aux<std::move_iterator<__gnu_cxx::__normal_iterator<char*, std::vector<char, std::allocator<char> > > > >(std::move_iterator<__gnu_cxx::__normal_iterator<char*, std::vector<char, std::allocator<char> > > >, std::move_iterator<__gnu_cxx::__normal_iterator<char*, std::vector<char, std::allocator<char> > > >, std::forward_iterator_tag);


int main(void)
{
  std::vector<char> v;
  v.push_back('a');
  
  cout << "Hello World! Copy Constructor can use private field directly!" <<endl;
  A a;
  cout << "Construct A instance, filed value: " << a.getField() << endl;

  return 0;
}
