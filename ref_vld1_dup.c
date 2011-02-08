/*

Copyright (c) 2009, 2010, 2011 STMicroelectronics
Written by Christophe Lyon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#ifdef __arm__
#include <arm_neon.h>
#else
#error Target not supported
#endif

#include "stm-arm-neon-ref.h"

#define TEST_MSG "VLD1_DUP/VLD1_DUPQ"
void exec_vld1_dup (void)
{
  int i;

  /* Fill vector with buffer item #i  */
#define TEST_VLD1_DUP(VAR, BUF, Q, T1, T2, W, N)			\
  VECT_VAR(VAR, T1, W, N) =						\
    vld1##Q##_dup_##T2##W(&VECT_VAR(BUF, T1, W, N)[i]);			\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N), VECT_VAR(VAR, T1, W, N))

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VARIABLE_ALL_VARIANTS(vector);

  /* Try to read different places from the input buffer */
  for (i=0; i<3; i++) {
    clean_results ();

    TEST_MACRO_ALL_VARIANTS_2_5(TEST_VLD1_DUP, vector, buffer);

    TEST_VLD1_DUP(vector, buffer, , float, f, 32, 2);
    TEST_VLD1_DUP(vector, buffer, q, float, f, 32, 4);

    dump_results_hex (TEST_MSG);
  }
}