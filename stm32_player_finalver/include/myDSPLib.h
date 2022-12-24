#ifndef _MYDSPLIB_
#define _MYDSPLIB_

#include "arm_math.h"
#include "math_helper.h"
#define SEMIHOSTING
#if defined(SEMIHOSTING)
#include <stdio.h>
#endif




void Wrapper_FIR(q7_t*, q7_t*, int, int, int);
void Wrapper_KeyShift(q7_t*, q7_t*, int, int);


#endif