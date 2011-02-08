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

/* Template file for binary saturating operator validation */

#ifdef __arm__
#include <arm_neon.h>
#else
#error Target not supported
#endif

#include "stm-arm-neon-ref.h"

#define FNNAME1(NAME) void exec_ ## NAME (void)
#define FNNAME(NAME) FNNAME1(NAME)

FNNAME (INSN_NAME)
{
  /* vector_res = OP(vector1,vector2), then store the result.  */
#define TEST_BINARY_SAT_OP1(INSN, Q, T1, T2, W, N)	\
  Neon_Overflow = 0;					\
  VECT_VAR(vector_res, T1, W, N) =			\
    INSN##Q##_##T2##W(VECT_VAR(vector1, T1, W, N),	\
		      VECT_VAR(vector2, T1, W, N));	\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N),		\
		    VECT_VAR(vector_res, T1, W, N));	\
  dump_neon_overflow(TEST_MSG, xSTR(INSN##Q##_##T2##W))

#define TEST_BINARY_SAT_OP(INSN, Q, T1, T2, W, N)	\
  TEST_BINARY_SAT_OP1(INSN, Q, T1, T2, W, N)

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VARIABLE_ALL_VARIANTS(vector1);
  DECL_VARIABLE_ALL_VARIANTS(vector2);
  DECL_VARIABLE_ALL_VARIANTS(vector_res);

  clean_results ();

  /* Initialize input "vector1" from "buffer"  */
  TEST_MACRO_ALL_VARIANTS_2_5(TEST_VLOAD, vector1, buffer);

  /* Choose arbitrary initialization values */
  TEST_VDUP(vector2, , int, s, 8, 8, 0x11);
  TEST_VDUP(vector2, , int, s, 16, 4, 0x22);
  TEST_VDUP(vector2, , int, s, 32, 2, 0x33);
  TEST_VDUP(vector2, , int, s, 64, 1, 0x44);
  TEST_VDUP(vector2, , uint, u, 8, 8, 0x55);
  TEST_VDUP(vector2, , uint, u, 16, 4, 0x66);
  TEST_VDUP(vector2, , uint, u, 32, 2, 0x77);
  TEST_VDUP(vector2, , uint, u, 64, 1, 0x88);

  TEST_VDUP(vector2, q, int, s, 8, 16, 0x11);
  TEST_VDUP(vector2, q, int, s, 16, 8, 0x22);
  TEST_VDUP(vector2, q, int, s, 32, 4, 0x33);
  TEST_VDUP(vector2, q, int, s, 64, 2, 0x44);
  TEST_VDUP(vector2, q, uint, u, 8, 16, 0x55);
  TEST_VDUP(vector2, q, uint, u, 16, 8, 0x66);
  TEST_VDUP(vector2, q, uint, u, 32, 4, 0x77);
  TEST_VDUP(vector2, q, uint, u, 64, 2, 0x88);

  fprintf(ref_file, "\n%s overflow output:\n", TEST_MSG);
  TEST_BINARY_SAT_OP(INSN_NAME, , int, s, 8, 8);
  TEST_BINARY_SAT_OP(INSN_NAME, , int, s, 16, 4);
  TEST_BINARY_SAT_OP(INSN_NAME, , int, s, 32, 2);
  TEST_BINARY_SAT_OP(INSN_NAME, , int, s, 64, 1);
  TEST_BINARY_SAT_OP(INSN_NAME, , uint, u, 8, 8);
  TEST_BINARY_SAT_OP(INSN_NAME, , uint, u, 16, 4);
  TEST_BINARY_SAT_OP(INSN_NAME, , uint, u, 32, 2);
  TEST_BINARY_SAT_OP(INSN_NAME, , uint, u, 64, 1);

  TEST_BINARY_SAT_OP(INSN_NAME, q, int, s, 8, 16);
  TEST_BINARY_SAT_OP(INSN_NAME, q, int, s, 16, 8);
  TEST_BINARY_SAT_OP(INSN_NAME, q, int, s, 32, 4);
  TEST_BINARY_SAT_OP(INSN_NAME, q, int, s, 64, 2);
  TEST_BINARY_SAT_OP(INSN_NAME, q, uint, u, 8, 16);
  TEST_BINARY_SAT_OP(INSN_NAME, q, uint, u, 16, 8);
  TEST_BINARY_SAT_OP(INSN_NAME, q, uint, u, 32, 4);
  TEST_BINARY_SAT_OP(INSN_NAME, q, uint, u, 64, 2);

  dump_results_hex (TEST_MSG);

#ifdef EXTRA_TESTS
  EXTRA_TESTS();
#endif
}