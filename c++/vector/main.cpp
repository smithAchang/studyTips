#include<iostream>
#include <vector>

int main(void)
{
  std::vector<char> v;
  v.push_back('a');
  
  std::cout << "Hello World! no-implicit-templates ,item: " << v[0] << std::endl;

  return 0;
}
