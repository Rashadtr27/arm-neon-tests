/*

Copyright (c) 2009, 2010, 2011, 2012 STMicroelectronics
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

#if defined(__arm__) || defined(__aarch64__)
#include <arm_neon.h>
#else
#include "stm-arm-neon.h"
#endif

#include "stm-arm-neon-ref.h"

#define INSN vqshrun_n
#define TEST_MSG "VQSHRUN_N"

#define FNNAME1(NAME) void exec_ ## NAME (void)
#define FNNAME(NAME) FNNAME1(NAME)

FNNAME (INSN)
{
  /* Basic test: y=vqshrun_n(x,v), then store the result.  */
#define TEST_VQSHRUN_N2(INSN, T1, T2, W, W2, N, V)			\
  Set_Neon_Cumulative_Sat(0, VECT_VAR(vector_res, uint, W2, N));	\
  VECT_VAR(vector_res, uint, W2, N) =					\
    INSN##_##T2##W(VECT_VAR(vector, T1, W, N),				\
		   V);							\
    vst1_u##W2(VECT_VAR(result, uint, W2, N),				\
	       VECT_VAR(vector_res, uint, W2, N));			\
      dump_neon_cumulative_sat(TEST_MSG, xSTR(INSN##_##T2##W),		\
			       xSTR(T1), W, N)

  /* Two auxliary macros are necessary to expand INSN */
#define TEST_VQSHRUN_N1(INSN, T1, T2, W, W2, N, V)	\
  TEST_VQSHRUN_N2(INSN, T1, T2, W, W2, N, V)

#define TEST_VQSHRUN_N(T1, T2, W, W2, N, V)	\
  TEST_VQSHRUN_N1(INSN, T1, T2, W, W2, N, V)


  /* With ARM RVCT, we need to declare variables before any executable
     statement */

  /* vector is twice as large as vector_res */
  DECL_VARIABLE(vector, int, 16, 8);
  DECL_VARIABLE(vector, int, 32, 4);
  DECL_VARIABLE(vector, int, 64, 2);

  DECL_VARIABLE(vector_res, uint, 8, 8);
  DECL_VARIABLE(vector_res, uint, 16, 4);
  DECL_VARIABLE(vector_res, uint, 32, 2);

  clean_results ();

  /* Fill input vector with negative values, to check saturation on limits */
  VDUP(vector, q, int, s, 16, 8, -2);
  VDUP(vector, q, int, s, 32, 4, -3);
  VDUP(vector, q, int, s, 64, 2, -4);

  /* Choose shift amount arbitrarily  */
  fprintf(ref_file, "\n%s cumulative saturation output:\n",
	  TEST_MSG " (negative input)");
  TEST_VQSHRUN_N(int, s, 16, 8, 8, 3);
  TEST_VQSHRUN_N(int, s, 32, 16, 4, 4);
  TEST_VQSHRUN_N(int, s, 64, 32, 2, 2);

  /* FIXME: only a few result buffers are used, but we output all of them */
  dump_results_hex2 (TEST_MSG, " (negative input)");

  /* Fill input vector with max value, to check saturation on limits */
  VDUP(vector, q, int, s, 16, 8, 0x7FFF);
  VDUP(vector, q, int, s, 32, 4, 0x7FFFFFFF);
  VDUP(vector, q, int, s, 64, 2, 0x7FFFFFFFFFFFFFFFLL);

  /* shift by 1  */
  fprintf(ref_file, "\n%s cumulative saturation output:\n",
	  TEST_MSG " (check cumulative saturation)");
  TEST_VQSHRUN_N(int, s, 16, 8, 8, 1);
  TEST_VQSHRUN_N(int, s, 32, 16, 4, 1);
  TEST_VQSHRUN_N(int, s, 64, 32, 2, 1);

  dump_results_hex2 (TEST_MSG, " (check cumulative saturation)");

  /* Fill input vector with positive values, to check normal case */
  VDUP(vector, q, int, s, 16, 8, 0x1234);
  VDUP(vector, q, int, s, 32, 4, 0x87654321);
  VDUP(vector, q, int, s, 64, 2, 0xDEADBEEF);

  /* shift arbitrary amount  */
  fprintf(ref_file, "\n%s cumulative saturation output:\n", TEST_MSG);
  TEST_VQSHRUN_N(int, s, 16, 8, 8, 6);
  TEST_VQSHRUN_N(int, s, 32, 16, 4, 7);
  TEST_VQSHRUN_N(int, s, 64, 32, 2, 8);

  dump_results_hex (TEST_MSG);
}
