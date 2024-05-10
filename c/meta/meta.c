#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


#ifndef  likely
        #define  likely(x) __builtin_expect(!!(x), 1)
#endif /* ifndef  likely */

#ifndef  unlikely
        #define  unlikely(x) __builtin_expect(!!(x), 0)
#endif /* ifndef  unlikely */


#define EMIT(event, ...) event ## _handler(__VA_ARGS__)

#define ON(event, handler) \
        void event ## _handler() { \
          handler(); \
        }


void my_loaded_handler() {
  printf("Loaded\n");
}

ON(load, my_loaded_handler);


#define MACRO(...) printf(__VA_ARGS__)
#define MACRO1(format_str_para, ...) printf(format_str_para, __VA_ARGS__)

#define MACRO2() printf("I'm a macro ...\n")

#define  MACRO_CALL_MACRO2() \
        printf("Will use other macro ...\n"); \
        MACRO2()

#define MACRO3() printf("I'm a macro being called by passed arg ...\n")

#define  MACRO_CALL_MACRO3_BY_ARG(MACRO_ARG) \
        printf("Will use other macro by arg ...\n"); \
        MACRO_ARG()


#define MACRO4(value) printf("I'm a macro being called by passed arg and with input arg: %d...\n", value)

#define  MACRO_CALL_MACRO_AND_PASSING_ARG(MACRO_ARG, arg_for_macro) \
        printf("Will use other macro by arg and passing arg to it ...\n"); \
        MACRO_ARG(arg_for_macro)


#define MACRO5 123

#define TOKEN_TO_STR(token) # token


#define EXPAN_MACRO_ARG_TO_STR(MACRO_ARG) TOKEN_TO_STR(MACRO_ARG)

int main(void)
{
  printf("hello world meta programming ...\n\n");

  EMIT(load);


  MACRO("Hello, world!\n");
  MACRO1("str para: %s, int para: %d\n", "Hello, world!", 10);

  MACRO_CALL_MACRO2();

  MACRO_CALL_MACRO3_BY_ARG(MACRO3);

  MACRO_CALL_MACRO_AND_PASSING_ARG(MACRO4, 100);

  printf("MACRO5: %d\n",           MACRO5);
  printf("MACRO5 to STR: %s\n",    TOKEN_TO_STR(MACRO5));
  printf("MACRO5 to STR v2: %s\n", EXPAN_MACRO_ARG_TO_STR(MACRO5));

  printf("\nwill exit!\n");

  return 0;
}
