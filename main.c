#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

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

// getting time for execution time
double get_time_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1e6;
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

        printf("Enter %d rows (Initian Velocity (km/h), Final Velocity (km/h), Time (seconds):\n", Y);
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

    // Computing the outputs & getting execution time
    double t_start = get_time_ms();
    compute_acceleration(input, output, Y);
    double t_end = get_time_ms();

    double elapsed_time = t_end - t_start;

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
        printf("\tInput: Vi=%.2f km/h, Vf=%.2f km/h, T=%.2f s\n", vi, vf, t);
        printf("\tConverted: Vi=%.2f m/s, Vf=%.2f m/s\n", vi_ms, vf_ms);
        printf("\tAcceleration = (Vf - Vi)/T = (%.2f - %.2f)/%.2f = %.4f m/s^2\n",
            vf_ms, vi_ms, t, a);
        printf("\tC Result = %d\n", ref_output[i]);
        printf("\tAssembly result = %d --> %s\n",
            output[i],
            (output[i] == ref_output[i]) ? "PASS" : "FAIL");
    }

    printf("\nExecution time for Y=%d elements: %.3f ms\n", Y, elapsed_time);

    free(input);
    free(output);
    free(ref_output);
    return 0;
}
