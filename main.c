#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern void compute_acceleration(float* input, int* output, int n);

// Reference C implementation for correctness check
static void reference_compute_acceleration(float* input, int* output, int n) {
    const float convert = 1000.0f / 3600.0f; // km/h → m/s
    for (int i = 0; i < n; ++i) {
        float vi = input[3 * i + 0];
        float vf = input[3 * i + 1];
        float t = input[3 * i + 2];

        if (t == 0.0f) {
            output[i] = 0;
            continue;
        }

        float vi_ms = vi * convert;
        float vf_ms = vf * convert;
        float a = (vf_ms - vi_ms) / t;

        // round to nearest integer
        output[i] = (int)lrintf(a);
    }
}

int main() {
    int Y;

    printf("Enter number of cars (Y): ");
    scanf("%d", &Y);

    float* input = malloc(Y * 3 * sizeof(float));
    int* output = malloc(Y * sizeof(int));
    int* ref_output = malloc(Y * sizeof(int));

    if (!input || !output || !ref_output) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    int valid;
    do {
        valid = 1;

        printf("Enter %d rows (Vi, Vf, T in KM/H, KM/H, seconds):\n", Y);
        for (int i = 0; i < Y; i++) {
            float vi, vf, t;

            if (scanf("%f, %f, %f", &vi, &vf, &t) != 3) {
                printf("Invalid input format.\n\n");
                valid = 0;
                break;
            }

            if (t <= 0) {
                printf("Time can not be less than or equal to 0.\n\n");
                valid = 0;
                break;
            }

            input[3 * i] = vi;
            input[3 * i + 1] = vf;
            input[3 * i + 2] = t;
        }

    } while (!valid);

    // Computing the outputs
    compute_acceleration(input, output, Y);
    reference_compute_acceleration(input, ref_output, Y);

    // Display results with correctness check
    const float convert = 1000.0f / 3600.0f;
    for (int i = 0; i < Y; i++) {
        float vi = input[3 * i + 0];
        float vf = input[3 * i + 1];
        float t = input[3 * i + 2];

        float vi_ms = vi * convert;
        float vf_ms = vf * convert;
        float a = (vf_ms - vi_ms) / t;

        printf("\nCar %d:\n", i + 1);
        printf("  Input: Vi=%.2f km/h, Vf=%.2f km/h, T=%.2f s\n", vi, vf, t);
        printf("  Converted: Vi=%.2f m/s, Vf=%.2f m/s\n", vi_ms, vf_ms);
        printf("  Acceleration = (Vf - Vi)/T = (%.2f - %.2f)/%.2f = %.4f m/s^2\n",
            vf_ms, vi_ms, t, a);
        printf("  C Result = %d\n", ref_output[i]);
        printf("  Assembly result = %d --> %s\n",
            output[i],
            (output[i] == ref_output[i]) ? "PASS" : "FAIL");
    }

    free(input);
    free(output);
    free(ref_output);
    return 0;
}
