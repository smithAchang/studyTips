#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


#define PARA_CHECK_RETRUN_GNU(expr, rc, format, args...) do{\
  if(expr)\
  {\
   printf(format"\n", ##args);\
  }\
  return rc; \
}while(0)

#define PARA_CHECK_NORETRUN_GNU(expr, format, args...) PARA_CHECK_RETRUN_GNU(expr, , format, ##args)


#define PARA_CHECK_RETRUN_C99(expr, rc, format, ...) do{\
  if(expr)\
  {\
   printf(format"\n", ##__VA_ARGS__);\
  }\
  return rc; \
}while(0)

#define PARA_CHECK_NORETRUN_C99(expr, format, ...) PARA_CHECK_RETRUN_GNU(expr, , format, ##__VA_ARGS__)



void f_void_has_no_print_para_ret()
{
  int b = 0;


  PARA_CHECK_NORETRUN_GNU(b == 1, "hello %s additional para: %d", __func__, b);
  PARA_CHECK_NORETRUN_GNU(b == 1, "hello no additional para");

  PARA_CHECK_NORETRUN_C99(b == 1, "hello c99 %s additional para: %d", __func__, b);
  PARA_CHECK_NORETRUN_C99(b == 1, "hello c99 no additional para");

  PARA_CHECK_NORETRUN_GNU(b == 0, "hello no additional para");
}

void f_void_has_print_para_ret()
{
  int b = 0;

  PARA_CHECK_NORETRUN_GNU(b == 0, "hello %s additional para: %d", __func__, b);
}

void f_void_has_print_para_ret_c99()
{
  int b = 0;

  PARA_CHECK_NORETRUN_C99(b == 0, "hello c99 %s additional para: %d", __func__, b);
}

bool f_bool_has_no_print_para_ret()
{
  int b = 0;


  PARA_CHECK_RETRUN_GNU(b == 1,   true,    "hello %s additional para: %d", __func__, b);
  PARA_CHECK_RETRUN_GNU(b == 1,   false, "hello no additional para");

  PARA_CHECK_RETRUN_C99(b == 1,   true,  "hello c99 %s additional para: %d", __func__, b);
  PARA_CHECK_RETRUN_C99(b == 1,   false, "hello c99 no additional para");

  PARA_CHECK_RETRUN_GNU(b == 0,   false, "hello no additional para");
}

bool f_bool_has_print_para_ret()
{
  int b = 0;

  PARA_CHECK_RETRUN_GNU(b == 0, true, "hello %s additional para: %d", __func__, b);
}

bool f_bool_has_print_para_ret_c99()
{
  int b = 0;

  PARA_CHECK_RETRUN_C99(b == 0, true, "hello c99 %s additional para: %d", __func__, b);
}


int main(void)
{
  printf("hello world ...\n\n");

  f_void_has_no_print_para_ret();
  f_void_has_print_para_ret();
  f_void_has_print_para_ret_c99();

  printf("\nhas return code test ...\n\n");

  f_bool_has_no_print_para_ret();
  f_bool_has_print_para_ret();
  f_bool_has_print_para_ret_c99();
  

  printf("\nwill exit!\n");
  return 0;
}
