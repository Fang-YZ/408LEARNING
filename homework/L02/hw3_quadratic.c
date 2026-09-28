#include <stdio.h>
#include <math.h>

int main(void){
    double a, b, c;
    scanf("%lf %lf %lf", &a, &b, &c);
    double d = b * b - 4 * a * c;
    if(fabs(d) < 1e-9){
        double x = -b / (2 * a);
        printf("One real root: x = %.2f\n", x);
    } else if(d > 0){
        double delta = sqrt(d);
        double x1 = (-b + delta) / (2 * a);
        double x2 = (-b - delta) / (2 * a);
        printf("x1 = %.2f, x2 = %.2f\n", x1, x2);
    } else {
        printf("No real roots\n");
    }
    return 0;
}