//
// Created by  Youngdong Kim on 2021/01/10.
//

#include <stdio.h>

int main(){

    char *A= "goodbye";
    int *greet= &A;
    char *greet= A;

    printf("greet = %s\n", A);
    printf("greet = %p\n", &A);

    printf("greet = %s\n", greet);
    printf("greet = %p\n", greet);

}