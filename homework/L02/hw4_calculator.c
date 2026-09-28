#include <stdio.h>

int main(void){
    double a,c;
    char b;
    scanf("%lf %c %lf", &a, &b, &c);
    switch(b){
        case '+':{
            double d = a + c;
            printf("%.2f + %.2f = %.2f\n", a, c, d);
        }break;
        case '-':{
            double d = a - c;
            printf("%.2f - %.2f = %.2f\n", a, c, d);
        }break;
        case '*':{
            double d = a * c;
            printf("%.2f * %.2f = %.2f\n", a, c, d);
        }break;
        case '/':{
            if(c == 0.0){
                printf("Division by zero. \n");
                break;
            }else{
                double d = a / c;
                printf("%.2f / %.2f = %.2f\n", a, c, d);
        }
    }break;
    default:{
        printf("Unknown operator.\n");
    }
    }
    return 0;
}