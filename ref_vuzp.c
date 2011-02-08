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

#ifndef INSN_NAME
#define INSN_NAME vuzp
#define TEST_MSG "VUZP/VUZPQ"
#endif

#define FNNAME1(NAME) void exec_ ## NAME (void)
#define FNNAME(NAME) FNNAME1(NAME)

FNNAME (INSN_NAME)
{
  /* In this case, output variables are arrays of vectors */
#define DECL_VUZP(T1, W, N)						\
  VECT_ARRAY_TYPE(T1, W, N, 2) VECT_ARRAY_VAR(result_vec, T1, W, N, 2);	\
  VECT_VAR_DECL(result_bis, T1, W, N)[2 * N]

  /* We need to use a temporary result buffer (result_bis), because
     the one used for other tests is not large enough. A subset of the
     result data is moved from result_bis to result, and it is this
     subset which is used to check the actual behaviour. The next
     macro enables to move another chunk of data from result_bis to
     result.  */
#define TEST_VUZP(INSN, Q, T1, T2, W, N)				\
  VECT_ARRAY_VAR(result_vec, T1, W, N, 2) =				\
    INSN##Q##_##T2##W(VECT_VAR(vector1, T1, W, N),			\
		      VECT_VAR(vector2, T1, W, N));			\
  vst2##Q##_##T2##W(VECT_VAR(result_bis, T1, W, N),			\
		    VECT_ARRAY_VAR(result_vec, T1, W, N, 2));		\
  memcpy(VECT_VAR(result, T1, W, N), VECT_VAR(result_bis, T1, W, N),	\
	 sizeof(VECT_VAR(result, T1, W, N)));

  /* Overwrite "result" with the contents of "result_bis"[X] */
#define TEST_EXTRA_CHUNK(T1, W, N, X)					\
  memcpy(VECT_VAR(result, T1, W, N), &(VECT_VAR(result_bis, T1, W, N)[X]), \
	 sizeof(VECT_VAR(result, T1, W, N)));

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VARIABLE_ALL_VARIANTS(vector1);
  DECL_VARIABLE_ALL_VARIANTS(vector2);

  /* We don't need 64 bits variants */
#define DECL_ALL_VUZP()				\
  DECL_VUZP(int, 8, 8);				\
  DECL_VUZP(int, 16, 4);			\
  DECL_VUZP(int, 32, 2);			\
  DECL_VUZP(uint, 8, 8);			\
  DECL_VUZP(uint, 16, 4);			\
  DECL_VUZP(uint, 32, 2);			\
  DECL_VUZP(float, 32, 2);			\
  DECL_VUZP(int, 8, 16);			\
  DECL_VUZP(int, 16, 8);			\
  DECL_VUZP(int, 32, 4);			\
  DECL_VUZP(uint, 8, 16);			\
  DECL_VUZP(uint, 16, 8);			\
  DECL_VUZP(uint, 32, 4);			\
  DECL_VUZP(float, 32, 4)

  DECL_ALL_VUZP();

  /* Initialize input "vector" from "buffer"  */
  TEST_MACRO_ALL_VARIANTS_2_5(TEST_VLOAD, vector1, buffer);
  TEST_VLOAD(vector1, buffer, , float, f, 32, 2);
  TEST_VLOAD(vector1, buffer, q, float, f, 32, 4);

  /* Choose arbitrary initialization values */
  TEST_VDUP(vector2, , int, s, 8, 8, 0x11);
  TEST_VDUP(vector2, , int, s, 16, 4, 0x22);
  TEST_VDUP(vector2, , int, s, 32, 2, 0x33);
  TEST_VDUP(vector2, , uint, u, 8, 8, 0x55);
  TEST_VDUP(vector2, , uint, u, 16, 4, 0x66);
  TEST_VDUP(vector2, , uint, u, 32, 2, 0x77);
  TEST_VDUP(vector2, , float, f, 32, 2, 33.6);

  TEST_VDUP(vector2, q, int, s, 8, 16, 0x11);
  TEST_VDUP(vector2, q, int, s, 16, 8, 0x22);
  TEST_VDUP(vector2, q, int, s, 32, 4, 0x33);
  TEST_VDUP(vector2, q, uint, u, 8, 16, 0x55);
  TEST_VDUP(vector2, q, uint, u, 16, 8, 0x66);
  TEST_VDUP(vector2, q, uint, u, 32, 4, 0x77);
  TEST_VDUP(vector2, q, float, f, 32, 4, 33.8);

#define TEST_ALL_VUZP(INSN)			\
  TEST_VUZP(INSN, , int, s, 8, 8);		\
  TEST_VUZP(INSN, , int, s, 16, 4);		\
  TEST_VUZP(INSN, , int, s, 32, 2);		\
  TEST_VUZP(INSN, , uint, u, 8, 8);		\
  TEST_VUZP(INSN, , uint, u, 16, 4);		\
  TEST_VUZP(INSN, , uint, u, 32, 2);		\
  TEST_VUZP(INSN, , float, f, 32, 2);		\
  TEST_VUZP(INSN, q, int, s, 8, 16);		\
  TEST_VUZP(INSN, q, int, s, 16, 8);		\
  TEST_VUZP(INSN, q, int, s, 32, 4);		\
  TEST_VUZP(INSN, q, uint, u, 8, 16);		\
  TEST_VUZP(INSN, q, uint, u, 16, 8);		\
  TEST_VUZP(INSN, q, uint, u, 32, 4);		\
  TEST_VUZP(INSN, q, float, f, 32, 4)

#define TEST_ALL_EXTRA_CHUNKS()			\
  TEST_EXTRA_CHUNK(int, 8, 8, 1);		\
  TEST_EXTRA_CHUNK(int, 16, 4, 1);		\
  TEST_EXTRA_CHUNK(int, 32, 2, 1);		\
  TEST_EXTRA_CHUNK(uint, 8, 8, 1);		\
  TEST_EXTRA_CHUNK(uint, 16, 4, 1);		\
  TEST_EXTRA_CHUNK(uint, 32, 2, 1);		\
  TEST_EXTRA_CHUNK(float, 32, 2, 1);		\
  TEST_EXTRA_CHUNK(int, 8, 16, 1);		\
  TEST_EXTRA_CHUNK(int, 16, 8, 1);		\
  TEST_EXTRA_CHUNK(int, 32, 4, 1);		\
  TEST_EXTRA_CHUNK(uint, 8, 16, 1);		\
  TEST_EXTRA_CHUNK(uint, 16, 8, 1);		\
  TEST_EXTRA_CHUNK(uint, 32, 4, 1);		\
  TEST_EXTRA_CHUNK(float, 32, 4, 1)

  /* Check vuzp/vuzpq */
  clean_results ();
  TEST_ALL_VUZP(INSN_NAME);

  dump_results_hex2 (TEST_MSG, " chunk 0");

  TEST_ALL_EXTRA_CHUNKS();
  dump_results_hex2 (TEST_MSG, " chunk 1");
}