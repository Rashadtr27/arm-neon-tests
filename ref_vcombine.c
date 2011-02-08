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

#define TEST_MSG "VCOMBINE"
void exec_vcombine (void)
{
  /* Basic test: vec128=vcombine(vec64_a, vec64_b), then store the result.  */
#define TEST_VCOMBINE(T1, T2, W, N, N2)					\
  VECT_VAR(vector128, T1, W, N2) =					\
    vcombine_##T2##W(VECT_VAR(vector64_a, T1, W, N),			\
		     VECT_VAR(vector64_b, T1, W, N));			\
  vst1q_##T2##W(VECT_VAR(result, T1, W, N2), VECT_VAR(vector128, T1, W, N2))

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VARIABLE_64BITS_VARIANTS(vector64_a);
  DECL_VARIABLE_64BITS_VARIANTS(vector64_b);
  DECL_VARIABLE_128BITS_VARIANTS(vector128);

  TEST_MACRO_64BITS_VARIANTS_2_5(TEST_VLOAD, vector64_a, buffer);

  TEST_VLOAD(vector64_a, buffer, , float, f, 32, 2);

  TEST_VDUP(vector64_b, , int, s, 8, 8, 0x11);
  TEST_VDUP(vector64_b, , int, s, 16, 4, 0x22);
  TEST_VDUP(vector64_b, , int, s, 32, 2, 0x33);
  TEST_VDUP(vector64_b, , int, s, 64, 1, 0x44);
  TEST_VDUP(vector64_b, , uint, u, 8, 8, 0x55);
  TEST_VDUP(vector64_b, , uint, u, 16, 4, 0x66);
  TEST_VDUP(vector64_b, , uint, u, 32, 2, 0x77);
  TEST_VDUP(vector64_b, , uint, u, 64, 1, 0x88);
  TEST_VDUP(vector64_b, , float, f, 32, 2, 3.3);

  clean_results ();

  TEST_VCOMBINE(int, s, 8, 8, 16);
  TEST_VCOMBINE(int, s, 16, 4, 8);
  TEST_VCOMBINE(int, s, 32, 2, 4);
  TEST_VCOMBINE(int, s, 64, 1, 2);
  TEST_VCOMBINE(uint, u, 8, 8, 16);
  TEST_VCOMBINE(uint, u, 16, 4, 8);
  TEST_VCOMBINE(uint, u, 32, 2, 4);
  TEST_VCOMBINE(uint, u, 64, 1, 2);
  TEST_VCOMBINE(float, f, 32, 2, 4);

  dump_results_hex (TEST_MSG);
}