#include<iostream>
using namespace std;

typedef enum WEEK
{
  SUN,
  MON
} WEEK;

int main(void)
{
  cout << "Hello World! Using enum value directly to see whether it exists warning! enum WEEK value:" <<SUN<<endl;
  
  return 0;
}
