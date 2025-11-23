# LBYARCH-MP2 The Acceleration Calculator
by Group 4: Lanz Wynel Choi and Yosh Gabriel Chua

## Overview
This project implements a program that computes the acceleration of multiple cars based on their initial velocity, final velocity, and time taken to travel 1 km. The solution is split between C for input/output and memory management, and Assembly for computation using scalar SIMD registers and floating-point instructions

## Description
Acceleration was calculated using the formula:

<img width="270" height="241" alt="image" src="https://github.com/user-attachments/assets/56ead99a-461d-45aa-b079-dc44c3f0e063" />

1. Vf = final velocity (km/h)
2. Vi = initial velocity (km/h)
3. t = time (seconds)
Final output is acceleration in (m/s^2)

The task is to convert the velocities from km/h to m/s before computing the acceleration and displaying the final result as an integer.

## Implementation and Relevant Files
1. main.c is where the C program is located. It is tasked with collecting the input values for computation, allocating memory for the matrices, calling the assembly function, and printing the results

2. functions.asm is responsible for converting the velocities from km/h to m/s, computing acceleration using floating-point instructions, and converting the result to integer

3. main_with_test_cases.c is an additional file, created to verify the correctness of the outputs of the assembly code to outputs computed by a C program. It is also used to test the performance and execution times of the assembly. This file is made purely for testing, and for the input and output program, use main.c instead.

## Performance Testing
Running the assembly function with input sizes: 10, 100, 1000, 10000
All the tests used randomized input values, and average execution times were computed over 30 runs.
The results of the tests are listed below:

| Input Sizes (Y) | Average Execution Time (in microseconds) | Correctness (PASS OR FAIL) |
| --------------- | ---------------------------------------- | -------------------------- |
| 10              | 0.037 µs                                 | PASS                       |
| 100             | 0.140 µs                                 | PASS                       |
| 1000            | 1.110 µs                                 | PASS                       |
| 10000           | 13.060 µs                                | PASS                       |

The execution time increases as the input size increases, growing linearly as the input sizes gets larger. Small inputs run extremely fast, and even at Y = 10,000 the runtime is only about 13 microseconds. Overall, the program is efficient and scales predictably, with all correctness checks passing.

## Program Output with Checking

<img width="600" height="741" alt="image" src="https://github.com/user-attachments/assets/cc0696e6-7ac5-4a55-b645-9f18549ff8a9" />

<img width="502" height="799" alt="image" src="https://github.com/user-attachments/assets/69f259c0-7d1e-4548-a9d6-2aaf019c6984" />


