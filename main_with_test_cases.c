#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

extern void compute_acceleration(float* input, int* output, int n);

//Using compute_acceleration locally to check for correctness
static void reference_compute_acceleration(float* input, int* output, int n) {
    const float convert = 1000.0f / 3600.0f; // 0.277777777778
    for (int i = 0; i < n; ++i) {
        float vi = input[3 * i + 0];
        float vf = input[3 * i + 1];
        float t  = input[3 * i + 2];
        if (t == 0.0f) {
            output[i] = 0;
            continue;
        }
        float vi_ms = vi * convert;
        float vf_ms = vf * convert;
        float a = (vf_ms - vi_ms) / t;
        //lrinf used to round-to-nearest
        output[i] = (int)lrintf(a);
    }
}

static double qpc_to_seconds(LARGE_INTEGER start, LARGE_INTEGER end, LARGE_INTEGER freq) {
    return (double)(end.QuadPart - start.QuadPart) / (double)freq.QuadPart;
}

int main(void) {
    const int sizes[] = {10, 100, 1000, 10000 }; //sizes to test
    const int sizes_count = sizeof(sizes) / sizeof(sizes[0]);
    const int runs = 30; // number of repetition

    srand(12345); //random generation

    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);

    for (int si = 0; si < sizes_count; ++si) {
        int Y = sizes[si];

        // Allocating arrays
        float* input = (float*)malloc((size_t)Y * 3 * sizeof(float));
        int* output = (int*)malloc((size_t)Y * sizeof(int));
        int* ref_output = (int*)malloc((size_t)Y * sizeof(int));

        if (!input || !output || !ref_output) {
            fprintf(stderr, "Memory allocation failed for Y=%d\n", Y);
            free(input); free(output); free(ref_output);
            continue;
        }

        for (int i = 0; i < Y; ++i) {
            float vi = ((float)rand() / (float)RAND_MAX) * 200.0f;
            float vf = ((float)rand() / (float)RAND_MAX) * 200.0f;
            float t  = 0.1f + ((float)rand() / (float)RAND_MAX) * (10.0f - 0.1f);
            input[3 * i + 0] = vi;
            input[3 * i + 1] = vf;
            input[3 * i + 2] = t;
        }

        // Compute reference outputs
        reference_compute_acceleration(input, ref_output, Y);

        //Compute outputs from asm function
        compute_acceleration(input, output, Y);

        // Verify correctness for test cases
        int mismatch_index = -1;
        for (int i = 0; i < Y; ++i) {
            if (output[i] != ref_output[i]) { mismatch_index = i; break; }
        }

        if (mismatch_index != -1) {
            printf("Y=%d: correctness check FAILED at %d (asm=%d, ref=%d)\n",
                Y, mismatch_index, output[mismatch_index], ref_output[mismatch_index]);
        } else {
            printf("Y=%d: correctness check PASSED \n", Y);
        }

        //Timing the asm function and getting the average
        double total_seconds = 0.0;
        for (int r = 0; r < runs; ++r) {
            LARGE_INTEGER t0, t1;
            QueryPerformanceCounter(&t0);
            compute_acceleration(input, output, Y);
            QueryPerformanceCounter(&t1);

            double secs = qpc_to_seconds(t0, t1, freq);
            total_seconds += secs;
        }

        double avg_seconds = total_seconds / (double)runs;
        double avg_us = avg_seconds * 1e6;
        printf("Y=%d: average execution time over %d runs = %.3f us (%.6f s)\n",
               Y, runs, avg_us, avg_seconds);

        // Print outputs to verify
        int to_print = (Y < 10) ? Y : 10;
        printf("Sample outputs (first %d):\n", to_print);
        for (int i = 0; i < to_print; ++i) {
            printf("  index %d: asm=%d  ref=%d\n", i, output[i], ref_output[i]);
        }

        free(input);
        free(output);
        free(ref_output);
        printf("\n");
    }
    return 0;
}
