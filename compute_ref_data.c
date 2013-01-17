
#ifdef __arm__
#include <arm_neon.h>
#else
#include "stm-arm-neon.h"
#endif
#include "stm-arm-neon-ref.h"

/* Initialization helpers; 4 slices are needed for vld2, vld3 and
   vld4.  */
#define MY_INIT_TAB(T,W,N) xNAME(INIT_TAB,N)(T##W##_t)
#define MY_INIT_TAB2(T,W,N) xNAME(INIT_TAB2,N)(T##W##_t)
#define MY_INIT_TAB3(T,W,N) xNAME(INIT_TAB3,N)(T##W##_t)
#define MY_INIT_TAB4(T,W,N) xNAME(INIT_TAB4,N)(T##W##_t)

/* Initialized input buffers.  */
#define VECT_VAR_DECL_INIT(V, T, W, N)			\
  VECT_VAR_DECL(V,T,W,N) [] = { MY_INIT_TAB(T,W,N) };

/* Specialized initializer with 4 entries, as used by vldX_dup and
   vdup tests, which iterated 4 times on input buffers.  */
#define VECT_VAR_DECL_INIT4(V, T, W, N)			\
  VECT_VAR_DECL(V,T,W,N) [] = { MY_INIT_TAB(T,W,4) };

/* Initializers for arrays of vectors.  */
#define VECT_ARRAY_INIT2(V, T, W, N)		\
  T##W##_t VECT_ARRAY_VAR(V,T,W,N,2)[] =	\
  { MY_INIT_TAB(T,W,N)				\
    MY_INIT_TAB2(T,W,N) };

#define VECT_ARRAY_INIT3(V, T, W, N)					\
  T##W##_t VECT_ARRAY_VAR(V,T,W,N,3)[] =				\
  { MY_INIT_TAB(T,W,N)							\
    MY_INIT_TAB2(T,W,N)							\
    MY_INIT_TAB3(T,W,N) };

#define VECT_ARRAY_INIT4(V, T, W, N)					\
  T##W##_t VECT_ARRAY_VAR(V,T,W,N,4)[] =				\
  { MY_INIT_TAB(T,W,N)							\
    MY_INIT_TAB2(T,W,N)							\
    MY_INIT_TAB3(T,W,N)							\
    MY_INIT_TAB4(T,W,N) };

/* Sample initialization vectors.  */
#define INIT_TAB_1(T)				\
  (T)-16,
#define INIT_TAB2_1(T)				\
  (T)-15,
#define INIT_TAB3_1(T)				\
  (T)-14,
#define INIT_TAB4_1(T)				\
  (T)-13,

#define INIT_TAB_2(T)				\
  (T)-16, (T)-15,
#define INIT_TAB2_2(T)				\
  (T)-14, (T)-13,
#define INIT_TAB3_2(T)				\
  (T)-12, (T)-11,
#define INIT_TAB4_2(T)				\
  (T)-10, (T)-9,

/* Initializer for vld3_lane tests.  */
#define INIT_TAB_3(T)				\
  (T)-16, (T)-15, (T)-14,

#define INIT_TAB_4(T)				\
  (T)-16, (T)-15, (T)-14, (T)-13,
#define INIT_TAB2_4(T)				\
  (T)-12, (T)-11, (T)-10, (T)-9,
#define INIT_TAB3_4(T)				\
  (T)-8, (T)-7, (T)-6, (T)-5,
#define INIT_TAB4_4(T)				\
  (T)-4, (T)-3, (T)-2, (T)-1,

#define INIT_TAB_8(T)							\
  (T)-16, (T)-15, (T)-14, (T)-13, (T)-12, (T)-11, (T)-10, (T)-9,
#define INIT_TAB2_8(T)							\
  (T)-8, (T)-7, (T)-6, (T)-5, (T)-4, (T)-3, (T)-2, (T)-1,
#define INIT_TAB3_8(T)							\
  (T)0, (T)1, (T)2, (T)3, (T)4, (T)5, (T)6, (T)7,
#define INIT_TAB4_8(T)							\
  (T)8, (T)9, (T)10, (T)11, (T)12, (T)13, (T)14, (T)15,

#define INIT_TAB_16(T)							\
  (T)-16, (T)-15, (T)-14, (T)-13, (T)-12, (T)-11, (T)-10, (T)-9,	\
  (T)-8, (T)-7, (T)-6, (T)-5, (T)-4, (T)-3, (T)-2, (T)-1,
#define INIT_TAB2_16(T)							\
  (T)0, (T)1, (T)2, (T)3, (T)4, (T)5, (T)6, (T)7,			\
  (T)8, (T)9, (T)10, (T)11, (T)12, (T)13, (T)14, (T)15,
#define INIT_TAB3_16(T)							\
  (T)16, (T)17, (T)18, (T)19, (T)20, (T)21, (T)22, (T)23,		\
   (T)24, (T)25, (T)26, (T)27, (T)28, (T)29, (T)30, (T)31,
#define INIT_TAB4_16(T)							\
  (T)32, (T)33, (T)34, (T)35, (T)36, (T)37, (T)38, (T)39,		\
  (T)40, (T)41, (T)42, (T)43, (T)44, (T)45, (T)46, (T)47,

/* Input buffers, one of each size.  */
/* Insert some padding to try to exhibit out of bounds accesses.  */
VECT_VAR_DECL_INIT(buffer, int, 8, 8);
PAD(buffer_pad, int, 8, 8);
VECT_VAR_DECL_INIT(buffer, int, 16, 4);
PAD(buffer_pad, int, 16, 4);
VECT_VAR_DECL_INIT(buffer, int, 32, 2);
PAD(buffer_pad, int, 32, 2);
VECT_VAR_DECL_INIT(buffer, int, 64, 1);
PAD(buffer_pad, int, 64, 1);
VECT_VAR_DECL_INIT(buffer, uint, 8, 8);
PAD(buffer_pad, uint, 8, 8);
VECT_VAR_DECL_INIT(buffer, uint, 16, 4);
PAD(buffer_pad, uint, 16, 4);
VECT_VAR_DECL_INIT(buffer, uint, 32, 2);
PAD(buffer_pad, uint, 32, 2);
VECT_VAR_DECL_INIT(buffer, uint, 64, 1);
PAD(buffer_pad, uint, 64, 1);
VECT_VAR_DECL_INIT(buffer, float, 32, 2);
PAD(buffer_pad, float, 32, 2);
VECT_VAR_DECL_INIT(buffer, int, 8, 16);
PAD(buffer_pad, int, 8, 16);
VECT_VAR_DECL_INIT(buffer, int, 16, 8);
PAD(buffer_pad, int, 16, 8);
VECT_VAR_DECL_INIT(buffer, int, 32, 4);
PAD(buffer_pad, int, 32, 4);
VECT_VAR_DECL_INIT(buffer, int, 64, 2);
PAD(buffer_pad, int, 64, 2);
VECT_VAR_DECL_INIT(buffer, uint, 8, 16);
PAD(buffer_pad, uint, 8, 16);
VECT_VAR_DECL_INIT(buffer, uint, 16, 8);
PAD(buffer_pad, uint, 16, 8);
VECT_VAR_DECL_INIT(buffer, uint, 32, 4);
PAD(buffer_pad, uint, 32, 4);
VECT_VAR_DECL_INIT(buffer, uint, 64, 2);
PAD(buffer_pad, uint, 64, 2);
VECT_VAR_DECL_INIT(buffer, float, 32, 4);
PAD(buffer_pad, float, 32, 4);

/* The tests for vld1_dup and vdup expect at least 4 entries in the
   input buffer, so force 1- and 2-elements initializers to have 4
   entries.  */
VECT_VAR_DECL_INIT(buffer_dup, int, 8, 8);
VECT_VAR_DECL(buffer_dup_pad, int, 8, 8);
VECT_VAR_DECL_INIT(buffer_dup, int, 16, 4);
VECT_VAR_DECL(buffer_dup_pad, int, 16, 4);
VECT_VAR_DECL_INIT4(buffer_dup, int, 32, 2);
VECT_VAR_DECL(buffer_dup_pad, int, 32, 2);
VECT_VAR_DECL_INIT4(buffer_dup, int, 64, 1);
VECT_VAR_DECL(buffer_dup_pad, int, 64, 1);
VECT_VAR_DECL_INIT(buffer_dup, uint, 8, 8);
VECT_VAR_DECL(buffer_dup_pad, uint, 8, 8);
VECT_VAR_DECL_INIT(buffer_dup, uint, 16, 4);
VECT_VAR_DECL(buffer_dup_pad, uint, 16, 4);
VECT_VAR_DECL_INIT4(buffer_dup, uint, 32, 2);
VECT_VAR_DECL(buffer_dup_pad, uint, 32, 2);
VECT_VAR_DECL_INIT4(buffer_dup, uint, 64, 1);
VECT_VAR_DECL(buffer_dup_pad, uint, 64, 1);
VECT_VAR_DECL_INIT4(buffer_dup, float, 32, 2);
VECT_VAR_DECL(buffer_dup_pad, float, 32, 2);
VECT_VAR_DECL_INIT(buffer_dup, int, 8, 16);
VECT_VAR_DECL(buffer_dup_pad, int, 8, 16);
VECT_VAR_DECL_INIT(buffer_dup, int, 16, 8);
VECT_VAR_DECL(buffer_dup_pad, int, 16, 8);
VECT_VAR_DECL_INIT(buffer_dup, int, 32, 4);
VECT_VAR_DECL(buffer_dup_pad, int, 32, 4);
VECT_VAR_DECL_INIT4(buffer_dup, int, 64, 2);
VECT_VAR_DECL(buffer_dup_pad, int, 64, 2);
VECT_VAR_DECL_INIT(buffer_dup, uint, 8, 16);
VECT_VAR_DECL(buffer_dup_pad, uint, 8, 16);
VECT_VAR_DECL_INIT(buffer_dup, uint, 16, 8);
VECT_VAR_DECL(buffer_dup_pad, uint, 16, 8);
VECT_VAR_DECL_INIT(buffer_dup, uint, 32, 4);
VECT_VAR_DECL(buffer_dup_pad, uint, 32, 4);
VECT_VAR_DECL_INIT4(buffer_dup, uint, 64, 2);
VECT_VAR_DECL(buffer_dup_pad, uint, 64, 2);
VECT_VAR_DECL_INIT(buffer_dup, float, 32, 4);
VECT_VAR_DECL(buffer_dup_pad, float, 32, 4);

/* Input buffers for vld2, 1 of each size */
VECT_ARRAY_INIT2(buffer_vld2, int, 8, 8);
PAD(buffer_vld2_pad, int, 8, 8);
VECT_ARRAY_INIT2(buffer_vld2, int, 16, 4);
PAD(buffer_vld2_pad, int, 16, 4);
VECT_ARRAY_INIT2(buffer_vld2, int, 32, 2);
PAD(buffer_vld2_pad, int, 32, 2);
VECT_ARRAY_INIT2(buffer_vld2, int, 64, 1);
PAD(buffer_vld2_pad, int, 64, 1);
VECT_ARRAY_INIT2(buffer_vld2, uint, 8, 8);
PAD(buffer_vld2_pad, uint, 8, 8);
VECT_ARRAY_INIT2(buffer_vld2, uint, 16, 4);
PAD(buffer_vld2_pad, uint, 16, 4);
VECT_ARRAY_INIT2(buffer_vld2, uint, 32, 2);
PAD(buffer_vld2_pad, uint, 32, 2);
VECT_ARRAY_INIT2(buffer_vld2, uint, 64, 1);
PAD(buffer_vld2_pad, uint, 64, 1);
VECT_ARRAY_INIT2(buffer_vld2, float, 32, 2);
PAD(buffer_vld2_pad, float, 32, 2);
VECT_ARRAY_INIT2(buffer_vld2, int, 8, 16);
PAD(buffer_vld2_pad, int, 8, 16);
VECT_ARRAY_INIT2(buffer_vld2, int, 16, 8);
PAD(buffer_vld2_pad, int, 16, 8);
VECT_ARRAY_INIT2(buffer_vld2, int, 32, 4);
PAD(buffer_vld2_pad, int, 32, 4);
VECT_ARRAY_INIT2(buffer_vld2, int, 64, 2);
PAD(buffer_vld2_pad, int, 64, 2);
VECT_ARRAY_INIT2(buffer_vld2, uint, 8, 16);
PAD(buffer_vld2_pad, uint, 8, 16);
VECT_ARRAY_INIT2(buffer_vld2, uint, 16, 8);
PAD(buffer_vld2_pad, uint, 16, 8);
VECT_ARRAY_INIT2(buffer_vld2, uint, 32, 4);
PAD(buffer_vld2_pad, uint, 32, 4);
VECT_ARRAY_INIT2(buffer_vld2, uint, 64, 2);
PAD(buffer_vld2_pad, uint, 64, 2);
VECT_ARRAY_INIT2(buffer_vld2, float, 32, 4);
PAD(buffer_vld2_pad, float, 32, 4);

/* Input buffers for vld3, 1 of each size */
VECT_ARRAY_INIT3(buffer_vld3, int, 8, 8);
PAD(buffer_vld3_pad, int, 8, 8);
VECT_ARRAY_INIT3(buffer_vld3, int, 16, 4);
PAD(buffer_vld3_pad, int, 16, 4);
VECT_ARRAY_INIT3(buffer_vld3, int, 32, 2);
PAD(buffer_vld3_pad, int, 32, 2);
VECT_ARRAY_INIT3(buffer_vld3, int, 64, 1);
PAD(buffer_vld3_pad, int, 64, 1);
VECT_ARRAY_INIT3(buffer_vld3, uint, 8, 8);
PAD(buffer_vld3_pad, uint, 8, 8);
VECT_ARRAY_INIT3(buffer_vld3, uint, 16, 4);
PAD(buffer_vld3_pad, uint, 16, 4);
VECT_ARRAY_INIT3(buffer_vld3, uint, 32, 2);
PAD(buffer_vld3_pad, uint, 32, 2);
VECT_ARRAY_INIT3(buffer_vld3, uint, 64, 1);
PAD(buffer_vld3_pad, uint, 64, 1);
VECT_ARRAY_INIT3(buffer_vld3, float, 32, 2);
PAD(buffer_vld3_pad, float, 32, 2);
VECT_ARRAY_INIT3(buffer_vld3, int, 8, 16);
PAD(buffer_vld3_pad, int, 8, 16);
VECT_ARRAY_INIT3(buffer_vld3, int, 16, 8);
PAD(buffer_vld3_pad, int, 16, 8);
VECT_ARRAY_INIT3(buffer_vld3, int, 32, 4);
PAD(buffer_vld3_pad, int, 32, 4);
VECT_ARRAY_INIT3(buffer_vld3, int, 64, 2);
PAD(buffer_vld3_pad, int, 64, 2);
VECT_ARRAY_INIT3(buffer_vld3, uint, 8, 16);
PAD(buffer_vld3_pad, uint, 8, 16);
VECT_ARRAY_INIT3(buffer_vld3, uint, 16, 8);
PAD(buffer_vld3_pad, uint, 16, 8);
VECT_ARRAY_INIT3(buffer_vld3, uint, 32, 4);
PAD(buffer_vld3_pad, uint, 32, 4);
VECT_ARRAY_INIT3(buffer_vld3, uint, 64, 2);
PAD(buffer_vld3_pad, uint, 64, 2);
VECT_ARRAY_INIT3(buffer_vld3, float, 32, 4);
PAD(buffer_vld3_pad, float, 32, 4);

/* Input buffers for vld4, 1 of each size */
VECT_ARRAY_INIT4(buffer_vld4, int, 8, 8);
PAD(buffer_vld4_pad, int, 8, 8);
VECT_ARRAY_INIT4(buffer_vld4, int, 16, 4);
PAD(buffer_vld4_pad, int, 16, 4);
VECT_ARRAY_INIT4(buffer_vld4, int, 32, 2);
PAD(buffer_vld4_pad, int, 32, 2);
VECT_ARRAY_INIT4(buffer_vld4, int, 64, 1);
PAD(buffer_vld4_pad, int, 64, 1);
VECT_ARRAY_INIT4(buffer_vld4, uint, 8, 8);
PAD(buffer_vld4_pad, uint, 8, 8);
VECT_ARRAY_INIT4(buffer_vld4, uint, 16, 4);
PAD(buffer_vld4_pad, uint, 16, 4);
VECT_ARRAY_INIT4(buffer_vld4, uint, 32, 2);
PAD(buffer_vld4_pad, uint, 32, 2);
VECT_ARRAY_INIT4(buffer_vld4, uint, 64, 1);
PAD(buffer_vld4_pad, uint, 64, 1);
VECT_ARRAY_INIT4(buffer_vld4, float, 32, 2);
PAD(buffer_vld4_pad, float, 32, 2);
VECT_ARRAY_INIT4(buffer_vld4, int, 8, 16);
PAD(buffer_vld4_pad, int, 8, 16);
VECT_ARRAY_INIT4(buffer_vld4, int, 16, 8);
PAD(buffer_vld4_pad, int, 16, 8);
VECT_ARRAY_INIT4(buffer_vld4, int, 32, 4);
PAD(buffer_vld4_pad, int, 32, 4);
VECT_ARRAY_INIT4(buffer_vld4, int, 64, 2);
PAD(buffer_vld4_pad, int, 64, 2);
VECT_ARRAY_INIT4(buffer_vld4, uint, 8, 16);
PAD(buffer_vld4_pad, uint, 8, 16);
VECT_ARRAY_INIT4(buffer_vld4, uint, 16, 8);
PAD(buffer_vld4_pad, uint, 16, 8);
VECT_ARRAY_INIT4(buffer_vld4, uint, 32, 4);
PAD(buffer_vld4_pad, uint, 32, 4);
VECT_ARRAY_INIT4(buffer_vld4, uint, 64, 2);
PAD(buffer_vld4_pad, uint, 64, 2);
VECT_ARRAY_INIT4(buffer_vld4, float, 32, 4);
PAD(buffer_vld4_pad, float, 32, 4);

/* Input buffers for vld2_lane */
VECT_VAR_DECL_INIT(buffer_vld2_lane, int, 8, 2);
VECT_VAR_DECL_INIT(buffer_vld2_lane, int, 16, 2);
VECT_VAR_DECL_INIT(buffer_vld2_lane, int, 32, 2);
VECT_VAR_DECL_INIT(buffer_vld2_lane, int, 64, 2);
VECT_VAR_DECL_INIT(buffer_vld2_lane, uint, 8, 2);
VECT_VAR_DECL_INIT(buffer_vld2_lane, uint, 16, 2);
VECT_VAR_DECL_INIT(buffer_vld2_lane, uint, 32, 2);
VECT_VAR_DECL_INIT(buffer_vld2_lane, uint, 64, 2);
VECT_VAR_DECL_INIT(buffer_vld2_lane, float, 32, 2);

/* Input buffers for vld3_lane */
VECT_VAR_DECL_INIT(buffer_vld3_lane, int, 8, 3);
VECT_VAR_DECL_INIT(buffer_vld3_lane, int, 16, 3);
VECT_VAR_DECL_INIT(buffer_vld3_lane, int, 32, 3);
VECT_VAR_DECL_INIT(buffer_vld3_lane, int, 64, 3);
VECT_VAR_DECL_INIT(buffer_vld3_lane, uint, 8, 3);
VECT_VAR_DECL_INIT(buffer_vld3_lane, uint, 16, 3);
VECT_VAR_DECL_INIT(buffer_vld3_lane, uint, 32, 3);
VECT_VAR_DECL_INIT(buffer_vld3_lane, uint, 64, 3);
VECT_VAR_DECL_INIT(buffer_vld3_lane, float, 32, 3);

/* Input buffers for vld4_lane */
VECT_VAR_DECL_INIT(buffer_vld4_lane, int, 8, 4);
VECT_VAR_DECL_INIT(buffer_vld4_lane, int, 16, 4);
VECT_VAR_DECL_INIT(buffer_vld4_lane, int, 32, 4);
VECT_VAR_DECL_INIT(buffer_vld4_lane, int, 64, 4);
VECT_VAR_DECL_INIT(buffer_vld4_lane, uint, 8, 4);
VECT_VAR_DECL_INIT(buffer_vld4_lane, uint, 16, 4);
VECT_VAR_DECL_INIT(buffer_vld4_lane, uint, 32, 4);
VECT_VAR_DECL_INIT(buffer_vld4_lane, uint, 64, 4);
VECT_VAR_DECL_INIT(buffer_vld4_lane, float, 32, 4);