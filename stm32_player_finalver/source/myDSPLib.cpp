#include "myDSPLib.h"
#include <cstdio>


#define BLOCK_SIZE 32
#define NUM_TAPS_ARRAY_SIZE 29
#define NUM_TAPS 29

static q7_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1];

q15_t TestComplex[2*512] = {
    -4, 0, 0, 0, 0, 0, 0, 0, 
    -23924, -4758, 0, 0, 0, 0, 0, 0, 
    -5, -2, 0, 0, 0, 0, 0, 0, 
    -29, -17, 0, 0, 0, 0, 0, 0, 
    -3, -3, 0, 0, 0, 0, 0, 0, 
    -4520, -6761, 0, 0, 0, 0, 0, 0, 
    -2, -4, 0, 0, 0, 0, 0, 0, 
    -8, -28, 0, 0, 0, 0, 0, 0, 
    0, -4, 0, 0, 0, 0, 0, 0, 
    -5, 18, 0, 0, 0, 0, 0, 0, 
    1, -4, 0, 0, 0, 0, 0, 0, 
    -2, 0, 0, 0, 0, 0, 0, 0, 
    2, -3, 0, 0, 0, 0, 0, 0, 
    -10, 6, 0, 0, 0, 0, 0, 0, 
    3, -2, 0, 0, 0, 0, 0, 0, 
    -2, 0, 0, 0, 0, 0, 0, 0, 
    4, 0, 0, 0, 0, 0, 0, 0, 
    3, 1, 0, 0, 0, 0, 0, 0, 
    3, 1, 0, 0, 0, 0, 0, 0, 
    21, 14, 0, 0, 0, 0, 0, 0, 
    2, 2, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    1, 3, 0, 0, 0, 0, 0, 0, 
    6, 33, 0, 0, 0, 0, 0, 0, 
    0, 4, 0, 0, 0, 0, 0, 0, 
    3, -20, 0, 0, 0, 0, 0, 0, 
    -2, 3, 0, 0, 0, 0, 0, 0, 
    13, -20, 0, 0, 0, 0, 0, 0, 
    -3, 3, 0, 0, 0, 0, 0, 0, 
    8, -6, 0, 0, 0, 0, 0, 0, 
    -5, 1, 0, 0, 0, 0, 0, 0, 
    -20, 3, 0, 0, 0, 0, 0, 0, 
    -4, 0, 0, 0, 0, 0, 0, 0, 
    9, 1, 0, 0, 0, 0, 0, 0, 
    -4, -2, 0, 0, 0, 0, 0, 0, 
    20, 15, 0, 0, 0, 0, 0, 0, 
    -3, -3, 0, 0, 0, 0, 0, 0, 
    -11, -15, 0, 0, 0, 0, 0, 0, 
    -2, -5, 0, 0, 0, 0, 0, 0, 
    -2, -7, 0, 0, 0, 0, 0, 0, 
    0, -4, 0, 0, 0, 0, 0, 0, 
    0, -2, 0, 0, 0, 0, 0, 0, 
    1, -5, 0, 0, 0, 0, 0, 0, 
    15, -24, 0, 0, 0, 0, 0, 0, 
    2, -4, 0, 0, 0, 0, 0, 0, 
    4, -3, 0, 0, 0, 0, 0, 0, 
    3, -3, 0, 0, 0, 0, 0, 0, 
    27, -5, 0, 0, 0, 0, 0, 0, 
    4, 0, 0, 0, 0, 0, 0, 0, 
    11, 3, 0, 0, 0, 0, 0, 0, 
    3, 2, 0, 0, 0, 0, 0, 0, 
    -19, -11, 0, 0, 0, 0, 0, 0, 
    2, 3, 0, 0, 0, 0, 0, 0, 
    -6, -7, 0, 0, 0, 0, 0, 0, 
    1, 4, 0, 0, 0, 0, 0, 0, 
    -2, -5, 0, 0, 0, 0, 0, 0, 
    0, 4, 0, 0, 0, 0, 0, 0, 
    3, -14, 0, 0, 0, 0, 0, 0, 
    -2, 4, 0, 0, 0, 0, 0, 0, 
    -13, 19, 0, 0, 0, 0, 0, 0, 
    -3, 3, 0, 0, 0, 0, 0, 0, 
    -11, 8, 0, 0, 0, 0, 0, 0, 
    -4, 1, 0, 0, 0, 0, 0, 0, 
    -8, 3, 0, 0, 0, 0, 0, 0, 
    -4, 0, 0, 0, 0, 0, 0, 0, 
    -8, -3, 0, 0, 0, 0, 0, 0, 
    -4, -1, 0, 0, 0, 0, 0, 0, 
    -11, -8, 0, 0, 0, 0, 0, 0, 
    -3, -3, 0, 0, 0, 0, 0, 0, 
    -13, -19, 0, 0, 0, 0, 0, 0, 
    -2, -4, 0, 0, 0, 0, 0, 0, 
    3, 14, 0, 0, 0, 0, 0, 0, 
    0, -4, 0, 0, 0, 0, 0, 0, 
    -2, 5, 0, 0, 0, 0, 0, 0, 
    1, -4, 0, 0, 0, 0, 0, 0, 
    -6, 7, 0, 0, 0, 0, 0, 0, 
    2, -3, 0, 0, 0, 0, 0, 0, 
    -19, 11, 0, 0, 0, 0, 0, 0, 
    3, -2, 0, 0, 0, 0, 0, 0, 
    11, -3, 0, 0, 0, 0, 0, 0, 
    4, 0, 0, 0, 0, 0, 0, 0, 
    27, 5, 0, 0, 0, 0, 0, 0, 
    3, 3, 0, 0, 0, 0, 0, 0, 
    4, 3, 0, 0, 0, 0, 0, 0, 
    2, 4, 0, 0, 0, 0, 0, 0, 
    15, 24, 0, 0, 0, 0, 0, 0, 
    1, 5, 0, 0, 0, 0, 0, 0, 
    0, 2, 0, 0, 0, 0, 0, 0, 
    0, 4, 0, 0, 0, 0, 0, 0, 
    -2, 7, 0, 0, 0, 0, 0, 0, 
    -2, 5, 0, 0, 0, 0, 0, 0, 
    -11, 15, 0, 0, 0, 0, 0, 0, 
    -3, 3, 0, 0, 0, 0, 0, 0, 
    20, -15, 0, 0, 0, 0, 0, 0, 
    -4, 2, 0, 0, 0, 0, 0, 0, 
    9, -1, 0, 0, 0, 0, 0, 0, 
    -4, 0, 0, 0, 0, 0, 0, 0, 
    -20, -3, 0, 0, 0, 0, 0, 0, 
    -5, -1, 0, 0, 0, 0, 0, 0, 
    8, 6, 0, 0, 0, 0, 0, 0, 
    -3, -3, 0, 0, 0, 0, 0, 0, 
    13, 20, 0, 0, 0, 0, 0, 0, 
    -2, -3, 0, 0, 0, 0, 0, 0, 
    3, 20, 0, 0, 0, 0, 0, 0, 
    0, -4, 0, 0, 0, 0, 0, 0, 
    6, -33, 0, 0, 0, 0, 0, 0, 
    1, -3, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    2, -2, 0, 0, 0, 0, 0, 0, 
    21, -14, 0, 0, 0, 0, 0, 0, 
    3, -1, 0, 0, 0, 0, 0, 0, 
    3, -1, 0, 0, 0, 0, 0, 0, 
    4, 0, 0, 0, 0, 0, 0, 0, 
    -2, 0, 0, 0, 0, 0, 0, 0, 
    3, 2, 0, 0, 0, 0, 0, 0, 
    -10, -6, 0, 0, 0, 0, 0, 0, 
    2, 3, 0, 0, 0, 0, 0, 0, 
    -2, 0, 0, 0, 0, 0, 0, 0, 
    1, 4, 0, 0, 0, 0, 0, 0, 
    -5, -18, 0, 0, 0, 0, 0, 0, 
    0, 4, 0, 0, 0, 0, 0, 0, 
    -8, 28, 0, 0, 0, 0, 0, 0, 
    -2, 4, 0, 0, 0, 0, 0, 0, 
    -4520, 6761, 0, 0, 0, 0, 0, 0, 
    -3, 3, 0, 0, 0, 0, 0, 0, 
    -29, 17, 0, 0, 0, 0, 0, 0, 
    -5, 2, 0, 0, 0, 0, 0, 0, 
    -23924, 4758, 0, 0, 0, 0, 0, 0, 
};

/* ----------------------------------------------------------------------
** FIR Coefficients buffer generated using fir1() MATLAB function.
** fir1(28, 6/24)
** Or use Filter Designer
** ------------------------------------------------------------------- */
const q7_t firCoeffs32_HP[NUM_TAPS_ARRAY_SIZE] = {
     -2,   -3,   -3,   -4,   -4,   -4,   -5,   -5,   -5,   -6,   -6,   -6,
     -6,   -6,  124,   -6,   -6,   -6,   -6,   -6,   -5,   -5,   -5,   -4,
     -4,   -4,   -3,   -3,   -2
};
const q7_t firCoeffs32_LP[NUM_TAPS_ARRAY_SIZE] = {
      2,    3,    3,    3,    4,    4,    4,    5,    5,    5,    5,    6,
      6,    6,    6,    6,    6,    6,    5,    5,    5,    5,    4,    4,
      4,    3,    3,    3,    2
};


void Wrapper_FIR(q7_t* inputData_q7t, q7_t* outputData_q7t, int receive_chunksize, int process_blocksize, int passband){
    
    
    arm_fir_instance_q7 S;
    arm_status status;
    q7_t  *inputQ7, *outputQ7;

    /* Initialize input and output buffer pointers */
    inputQ7 = &inputData_q7t[0];
    outputQ7 = &outputData_q7t[0];

    /* Call FIR init function to initialize the instance structure. */
    if(passband == 0){
        // LowPass
        arm_fir_init_q7(&S, NUM_TAPS, (q7_t *)&firCoeffs32_LP[0], &firStateF32[0], process_blocksize);
    }
    if(passband == 1){
        // HighPass
        arm_fir_init_q7(&S, NUM_TAPS, (q7_t *)&firCoeffs32_HP[0], &firStateF32[0], process_blocksize);
    }

  

    /* ----------------------------------------------------------------------
    ** Call the FIR process function for every blockSize samples
    ** ------------------------------------------------------------------- */
    uint32_t numblocks = receive_chunksize/process_blocksize;
    for(int i=0; i < numblocks; i++){
    arm_fir_q7(&S, inputQ7 + (i * process_blocksize), outputQ7 + (i * process_blocksize), process_blocksize);
    }

    printf("FINISH\n");

  
}

void Wrapper_KeyShift(q7_t* inputData_q7, q7_t* outputData_q7, int receive_chunksize, int shiftFreq){
    
    arm_rfft_instance_q15 S;
    arm_cfft_radix4_instance_q15 S_CFFT;
    arm_status status;
    
    // initialize BufferA, BufferB
    q15_t* BufferA_q15 = new q15_t[receive_chunksize];
    q15_t* BufferB_q15 = new q15_t[receive_chunksize*2];

    status = arm_rfft_init_q15 (&S, &S_CFFT, receive_chunksize, 0, 1);


    // BufferA in use
    q15_t* inputData_q15 = BufferA_q15;
    arm_q7_to_q15(inputData_q7 , inputData_q15, receive_chunksize);



    // BufferB in use
    q15_t* rfft_output_cmplx_q15 = BufferB_q15;
    arm_rfft_q15(&S, inputData_q15, rfft_output_cmplx_q15);
    // BufferA free

    

    
    // 0 is DC
    // 1 ~ 256-1 is front 2 ~ 2*(256-1)
    // 256 is highest freq
    // 256+1 ~ 2*256-1 is back 2*(256+1) ~ 2*(2*256-1)
    for(int i = receive_chunksize/2 - 1; i >= 1 ; i--){
        if(i-shiftFreq >= 1) {
            rfft_output_cmplx_q15[2*i  ] = rfft_output_cmplx_q15[2*i  -2*shiftFreq];
            rfft_output_cmplx_q15[2*i+1] = rfft_output_cmplx_q15[2*i+1-2*shiftFreq];
        }
        else{
            rfft_output_cmplx_q15[2*i  ] = 0;
            rfft_output_cmplx_q15[2*i+1] = 0;
        }
    }
    for(int i = receive_chunksize/2 + 1; i <= receive_chunksize-1 ; i++){
        if(2*i+1+2*shiftFreq <= 2*receive_chunksize-1){
            rfft_output_cmplx_q15[2*i  ] = rfft_output_cmplx_q15[2*i  +2*shiftFreq];
            rfft_output_cmplx_q15[2*i+1] = rfft_output_cmplx_q15[2*i+1+2*shiftFreq];
        }
        else{
            rfft_output_cmplx_q15[2*i  ] = 0;
            rfft_output_cmplx_q15[2*i+1] = 0;
        }
    }

    // Print FFT result(complex)
    for(int j = 0; j < 2*receive_chunksize; j=j+8)
        printf("%d + %dj, %d + %dj, %d + %dj, %d + %dj ... \n", rfft_output_cmplx_q15[j], rfft_output_cmplx_q15[j+1], rfft_output_cmplx_q15[j+2], rfft_output_cmplx_q15[j+3], rfft_output_cmplx_q15[j+4], rfft_output_cmplx_q15[j+5], rfft_output_cmplx_q15[j+6], rfft_output_cmplx_q15[j+7]);
    printf("-----------------------------------\n");


    /* TODO: Frequency Domain DSP Here */
    
    
    



    status = arm_rfft_init_q15 (&S, &S_CFFT, receive_chunksize, 1, 1);

    // BufferA re use
    q15_t* outputData_q15 = BufferA_q15;
    arm_rfft_q15(&S, rfft_output_cmplx_q15, outputData_q15);
    // BufferB free
    
    arm_shift_q15(outputData_q15, 8, outputData_q15, receive_chunksize);
    arm_q15_to_q7(outputData_q15, outputData_q7, receive_chunksize);
    // BufferA free

    // // Print FFT result(complex)
    // for(int j=0; j < receive_chunksize; j+=8)
    //     printf("%d, %d, %d, %d, %d, %d, %d, %d ... \n", outputData_q7[j], outputData_q7[j+1], outputData_q7[j+2], outputData_q7[j+3], outputData_q7[j+4], outputData_q7[j+5], outputData_q7[j+6], outputData_q7[j+7]);

    // delete BufferA, BufferB
    delete []inputData_q15;
    delete []rfft_output_cmplx_q15;

}
