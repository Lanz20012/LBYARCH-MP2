;Factors to compute acceleration (m/s^2)
;1. Initial Velocity (Vi) - starting speed
;2. Final Velocity (Vf) - final speed at target distance (1KM)
;3. Time (t) - duration to reach 1KM

;Formula for acceleration: a = (Vf - Vi) / t
;Formula to convert KM to M: 1KM = 1000M
;Formula to convert hours to seconds: 1 hour = 3600 seconds

;Sample Calculation: Given: Vf = 62.5 KM/H, Vi = 0.0 KM/H, t = 10.1 seconds
;1. Convert Vf to m/s: Vi = 62.5 * (1000 / 3600) = 17.36 m/s
;2. Calculate acceleration: a = (17.36-0.0) / 10.1 = 1.7188 m/s^2
;3. Convert to int (1.7188 m/s^2) =  2 m/s^2


;Using the formula (1000/3600) to convert KM/H to M/S
;We get the constant value of 0.2777777777777778
section .data
    convert: dd 0.2777777777777778 ;as float

section .text
global compute_acceleration
compute_acceleration:
    ;Inputs: each row indicates the paramter of a car in matrix
    ;Initial Velocity (Vi) in KM/H, Final Velocity (Vf) in KM/H, Time (t) in seconds
    ;Outputs: Acceleration (a) in m/s^2 as integer

    ;Parameters:
    ;rcx = float* input (Vi, Vf, t, Vi, Vf, t, ...continued)
    ;rdx = int* output (a, a, a, ...continued)
    ;r8 = int n number of cars (sets of Vi, Vf, t)

    push rbp
    mov rbp, rsp
    push r12
    push r13
    push r14

    mov r12, rcx
    mov r13, rdx
    mov r14, r8 ;counter for number of cars

    movss xmm0, [rel convert] ;load conversion into xmm0

    .loop_all_cars:
        cmp r14, 0
        je .end_program

        ;Load Vi, Vf, t for current car
        movss xmm1, [r12 + 0] ;Vi
        movss xmm2, [r12 + 4] ;Vf
        movss xmm3, [r12 + 8] ;t

        ;change in velocity (Vf - Vi)
        subss xmm2, xmm1    ;xmm2 = Vf - Vi

        ;Convert change in velocity to m/s
        mulss xmm2, xmm0    ;xmm2 = (Vf - Vi) * 0.2777777777777778

        ;Calculate acceleration: a = (Vf - Vi) / t
        divss xmm2, xmm3    ;xmm2 = a in m/s^2

        ;Coversion to integer
        cvtss2si ebx, xmm2  ;convert float a to int

        ;Store result
        mov [r13], ebx       ;store acceleration in output

        ;Advance to next car
        add r12, 12          ;move to next set of (Vi, Vf, t)
        add r13, 4           ;move to next output position

        dec r14
        jnz .loop_all_cars

.end_program:
    pop r14
    pop r13
    pop r12
    pop rbp
    ret
