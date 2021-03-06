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

#define INSN vqrdmulh
#define TEST_MSG "VQRDMULH_LANE"

#define FNNAME1(NAME) void exec_ ## NAME ## _lane (void)
#define FNNAME(NAME) FNNAME1(NAME)

FNNAME (INSN)
{
  /* vector_res = vqrdmulh_lane(vector,vector2,lane), then store the result.  */
#define TEST_VQRDMULH_LANE2(INSN, Q, T1, T2, W, N, N2, L)		\
  Set_Neon_Cumulative_Sat(0, VECT_VAR(vector_res, T1, W, N));		\
  VECT_VAR(vector_res, T1, W, N) =					\
    INSN##Q##_lane_##T2##W(VECT_VAR(vector, T1, W, N),			\
			   VECT_VAR(vector2, T1, W, N2),		\
			   L);						\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N),				\
		    VECT_VAR(vector_res, T1, W, N));			\
  dump_neon_cumulative_sat(TEST_MSG, xSTR(INSN##Q##_lane_##T2##W),	\
			   xSTR(T1), W, N)

  /* Two auxliary macros are necessary to expand INSN */
#define TEST_VQRDMULH_LANE1(INSN, Q, T1, T2, W, N, N2, L)	\
  TEST_VQRDMULH_LANE2(INSN, Q, T1, T2, W, N, N2, L)

#define TEST_VQRDMULH_LANE(Q, T1, T2, W, N, N2, L)	\
  TEST_VQRDMULH_LANE1(INSN, Q, T1, T2, W, N, N2, L)


  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VARIABLE(vector, int, 16, 4);
  DECL_VARIABLE(vector, int, 32, 2);
  DECL_VARIABLE(vector, int, 16, 8);
  DECL_VARIABLE(vector, int, 32, 4);

  DECL_VARIABLE(vector_res, int, 16, 4);
  DECL_VARIABLE(vector_res, int, 32, 2);
  DECL_VARIABLE(vector_res, int, 16, 8);
  DECL_VARIABLE(vector_res, int, 32, 4);

  /* vector2: vqrdmulh_lane and vqrdmulhq_lane have a 2nd argument with
     the same number of elements, so we need only one variable of each
     type.  */
  DECL_VARIABLE(vector2, int, 16, 4);
  DECL_VARIABLE(vector2, int, 32, 2);

  clean_results ();

  VLOAD(vector, buffer, , int, s, 16, 4);
  VLOAD(vector, buffer, , int, s, 32, 2);

  VLOAD(vector, buffer, q, int, s, 16, 8);
  VLOAD(vector, buffer, q, int, s, 32, 4);

  /* Initialize vector2 */
  VDUP(vector2, , int, s, 16, 4, 0x55);
  VDUP(vector2, , int, s, 32, 2, 0xBB);

  /* Choose lane arbitrarily */
  fprintf(ref_file, "\n%s cumulative saturation output:\n", TEST_MSG);
  TEST_VQRDMULH_LANE(, int, s, 16, 4, 4, 2);
  TEST_VQRDMULH_LANE(, int, s, 32, 2, 2, 1);
  TEST_VQRDMULH_LANE(q, int, s, 16, 8, 4, 3);
  TEST_VQRDMULH_LANE(q, int, s, 32, 4, 2, 0);

  /* FIXME: only a subset of the result buffers are used, but we
     output all of them */
  dump_results_hex (TEST_MSG);


  VDUP(vector, , int, s, 16, 4, 0x8000);
  VDUP(vector, , int, s, 32, 2, 0x80000000);
  VDUP(vector, q, int, s, 16, 8, 0x8000);
  VDUP(vector, q, int, s, 32, 4, 0x80000000);
  VDUP(vector2, , int, s, 16, 4, 0x8000);
  VDUP(vector2, , int, s, 32, 2, 0x80000000);

  fprintf(ref_file, "\n%s cumulative saturation output:\n",
	  TEST_MSG " (check mul cumulative saturation)");
  TEST_VQRDMULH_LANE(, int, s, 16, 4, 4, 2);
  TEST_VQRDMULH_LANE(, int, s, 32, 2, 2, 1);
  TEST_VQRDMULH_LANE(q, int, s, 16, 8, 4, 3);
  TEST_VQRDMULH_LANE(q, int, s, 32, 4, 2, 0);
  dump_results_hex2 (TEST_MSG, " (check mul cumulative saturation)");


  VDUP(vector, , int, s, 16, 4, 0x8000);
  VDUP(vector, , int, s, 32, 2, 0x80000000);
  VDUP(vector, q, int, s, 16, 8, 0x8000);
  VDUP(vector, q, int, s, 32, 4, 0x80000000);
  VDUP(vector2, , int, s, 16, 4, 0x8001);
  VDUP(vector2, , int, s, 32, 2, 0x80000001);

  fprintf(ref_file, "\n%s cumulative saturation output:\n",
	  TEST_MSG " (check rounding cumulative saturation)");
  TEST_VQRDMULH_LANE(, int, s, 16, 4, 4, 2);
  TEST_VQRDMULH_LANE(, int, s, 32, 2, 2, 1);
  TEST_VQRDMULH_LANE(q, int, s, 16, 8, 4, 3);
  TEST_VQRDMULH_LANE(q, int, s, 32, 4, 2, 0);
  dump_results_hex2 (TEST_MSG, " (check rounding cumulative saturation)");
}
