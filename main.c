#include <stdio.h>
#include <stdlib.h>

extern void compute_acceleration(float* input, int* output, int n);

int main() {
    int Y;
    printf("Enter number of cars (Y): ");
    scanf("%d", &Y);

    float* input = malloc(Y * 3 * sizeof(float));
    int* output = malloc(Y * sizeof(int));

    if (!input || !output) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    printf("Enter %d rows (Vi,Vf,T in KM/H,KM/H,seconds):\n", Y);
    for (int i = 0; i < Y; i++) {
        float vi, vf, t;
        scanf("%f ,%f ,%f", &vi, &vf, &t);
        input[3 * i] = vi;
        input[3 * i + 1] = vf;
        input[3 * i + 2] = t;
    }

    compute_acceleration(input, output, Y);

    for (int i = 0; i < Y; i++) {
        printf("%d\n", output[i]);
    }

    free(input);
    free(output);
    return 0;
}
