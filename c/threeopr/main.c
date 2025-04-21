#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>



int main(int argc, char* argv[]) {
   
    char szArr[256];
    printf("\nThree opers and bitwise, whose speed is more efficient?\n");
    
    char c;
    c = argc < sizeof(szArr) ? szArr[argc] : '\0';
    c = szArr[argc &(sizeof(szArr) -1)];
    
    return 0;
}