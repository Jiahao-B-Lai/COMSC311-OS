/* 
 * CS:APP Data Lab 
 * 
 * Author: Jiahao Lai;
 * Email: jvl6364@psu.edu; 
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2012 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* We do support the IEC 559 math functionality, real and complex.  */
/* wchar_t uses ISO/IEC 10646 (2nd ed., published 2011-03-15) /
   Unicode 6.0.  */
/* We do not support C11 <threads.h>.  */
/* 
 * allEvenBits - return 1 if all even-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allEvenBits(0xFFFFFFFE) = 0, allEvenBits(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allEvenBits(int x) {
  /* create a constant 0101010101010101 by shift left; 
   * use y and ~ operation to create another constant which all and only odd-numbered bits are 1: 101010.....101010;  
   * and then use | to compare the input x with the constant z,
   * because every odd-numbered bit in z is 1, whatever the odd-umbered bits are in x,
   *x|z will always get 1 on odd-numbered bits;
   * if and ony if all the even-numbered bits are 1, (x|z) can get all 1s, which is 0xffffffff;
   * because only !(0) can get 1, we use ~ to change all 1s in 0xffffffff into all 0s, and then we get 0;
   * so, finally we can use ! to get the correct output;
   */  
  int y = ( 85 << 8 ) + 85;
  int z;
  z = ~ ( ( y << 16 ) + y);
  return ! ( ~ ( x | z ) );
}
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
  /* the way to check whether x is 0:
   * firstly we use ~ to change all 0s into all 1s,if and only if x==0, we can get 1111...111, which is 0xffffffff;
   * then add 1 on that number; only when that number is 0xffffffff (get from ~0), we can get o again after adding 1,
   * other than 0xffffffff, other number add 1 will not get 0;
   * after that we use | to compare the original x with ~(x+1), also, only when x==0, we can get 0 after | operation,
   * then we use right shift 31bits to get the sign bit of the result on previous step;
   * NOTICE that if the sign bit is 1, right shift will fill 1s, 
   * so if x is 0, we get 0, else, we get 0xffffffff in this step;
   * finally, add 1 in the end to print the correct output, 
   * if x is not 0, return 0xffffffff + 1 which is 0, if x is 0, plus 1 to return 1 ;
   */ 
  x = x |( ~ x + 1 );
  x = ( x >> 31 ) + 1;  
  return x;
}
/* 
 * floatIsEqual - Compute f == g for floating point arguments f and g.
 *   Both the arguments are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   If either argument is NaN, return 0.
 *   +0 and -0 are considered equal.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 25
 *   Rating: 2
 */
int floatIsEqual(unsigned uf, unsigned ug) {
  /* To decide whether uf and ug are eauql, firstly set the sign bit to 0;
   * Because we assume +0 is equal to -0, so we can shift left 1 bit to uf and ug,
   * if they are +0 or -0, <<1 will get all 0, and return 1 here;
   * Then we need to judge whether either of them is NaN;
   * Because NaN is that the exp part is 11111111, and the frac part is not all 0;
   * if the frac part iss all 0, its infinity, and we consider infinity as NaN also;
   * if either of uf and ug is NaN, we return 0;
   * Otherwise, just use == to decide if they are equal;
   */
  unsigned SetSignF = uf & 0x7fffffff;
  unsigned SetSignG = ug & 0x7fffffff;
  if ( ( ( uf << 1 ) == 0 ) && ( ( ( ug << 1 ) == 0 ) ) )
    return 1;
  if ( ( SetSignF <= 0x7f800000 ) && ( SetSignG <= 0x7f800000 )  )
  {
  if ( uf == ug )
    return 1;
  else
    return 0;
  }
  else
    return 0;
}
/* 
 * floatUnsigned2Float - Return bit-level equivalent of expression (float) u
 *   Result is returned as unsigned int, but
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatUnsigned2Float(unsigned u) {
 
  int SignBit = 0; 
  int ShiftHelper;
  int EXP;
  int Frac;
  int Bias = 127;
  int E;
  int e;
  int Rounding;
  if ( u == 0)
   return 0; 
  else if ( u == 1 )
   return 0x00800000;
  else
  {
   ShiftHelper = u;
   e = 0;
   while ( ( ShiftHelper & 0x80000000 ) != 0x80000000 )
   {
    ShiftHelper = ShiftHelper << 1;
    e = e + 1;   
   }
  E = 31 - e;
  EXP = E + Bias;
  EXP = EXP << (32 - 1 - 8);
  Frac = (( 1 << ( E - 1 ) ) & u);
  Rounding = 0;
  if ( E > 23 )
  {
   if ( ( ( Frac << 22 ) ^ 0xc0000000 ) == 0)
     Rounding = 1;
   else if ( ( ( Frac << 23 ) ^ 0x80000000 ) == 0)
     Rounding = 1;
   else
     Rounding = 0;
  } 
  Frac = (Frac >> (E - 23 )) & 0x007fffff;
   return ( SignBit + EXP + Frac + Rounding) ;
  }
}


/* 
 * isLess - if x < y  then return 1, else return 0 
 *   Example: isLess(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLess(int x, int y) {
  /* To compare whether x is less than y, we should consider whether the sign is same or not,
   * so we need a CheckSign using right shift and ^ to check if the sign is the same;
   * Because we cannot use subtract, we can apply x-y = x+~y+1 to solve it!
   * So, if the sign for x and y is the same, which is (~CheckSign) equals 0xffffffff, we can check the MSB of x+~y+1:
   * if it is 1, than x is less than y, if it is 0, x is larger than y;
   * If the sign is different, which is (CheckSign) equals to 0xffffffff, we can just compare the sign bit of x and y;
   * if the sign bit of x is 1, then x should be less than y, because the sign bit of y is the opposite;
   * and we can output 1.  If the sign bit of x is 0, than it must be larger than y;
   */    
  int CheckSign = ( x >> 31) ^ ( y >> 31); 
  int Diffirence = ( x + ( ~ y) + 1 ) >> 31;
  return ((!(~ Diffirence )) & ( ~ CheckSign )) + ((!(~ ( (x >> 31) ^ ( ~ CheckSign)))) & (CheckSign));
}
/* 
 * isNonNegative - return 1 if x >= 0, return 0 otherwise 
 *   Example: isNonNegative(-1) = 0.  isNonNegative(0) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int isNonNegative(int x) {
  /* shift right 31 positions to get the sign bit; 
   * use NOT-! to print 0 if negative, print 1 if nonnegative; 
   */ 
  x = x >> 31;
  return !x;
}
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  /* we know that Tmax is 0x7fffffff, we want get 1 when x is Tmax and we know that only !(0) is 1;
   * so we try to get 0: because only sign bit for Tmax is 0, and only sign bit is 1 if we add 1 on Tmax,
   * so we use ^ to compare x and x+1 to check if x is Tamx, if that is true, we will get all 1s, which is 0xffffffff;
   * and then we could use ~ to change all 0s into 1s, 
   * if x is Tmax, then we will get 0 at this step , and use ! to print 1 if x is Tmax;
   * HOWEVER! there is the other number which also satisfy this condition, which is -1! (0xffffffff);
   * so, we need to eliminate -1;
   * need helper here: if x is -1, then !(x+1) will be 1, otherwise, it will be 0;
   * finally we use ! (x|helper) to get the correct output; only when x is Tmax, we can get 1;
   * if x is -1, helper here will be 1, and !(-1 | 1) will be 0, which means -1 is not the Tmax!;
   */
  int helper;
  helper = ! ( x + 1); 
  x = ~( x ^ ( x + 1) );
  return ! ( x | helper ); 
}
/* 
 * rotateLeft - Rotate x to the left by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateLeft(0x87654321,4) = 0x76543218
 *   Legal ops: ~ & ^ | + << >> !
 *   Max ops: 25
 *   Rating: 3 
 */
int rotateLeft(int x, int n) {
  /* firstly, there is a special case that when n is 0, x do not need to change,
   * so we need a helper3 to define if n is 0: if n is 0, just output x itself; if n is not 0, then rotate;
   * When doing rotation, firstly we create a helper4 to help store the values of bits going to the right side;
   * ~0 is 0xffffffff, so we can shift left n bits and then use ~ to reverse it, we get a plcae to save value;
   * Then shift x left with n bits; and move the leftest n bits to the rightest bits, here we use (32+~n+1) as (32-n);
   * Because we have a helper4 with rightest n bits with 1s and other leftside bits with 0s,
   * we can use adder here to create the part that will be added at the end;
   * After adding the adder we can get the right output!
   */  
  int helper3 = ( ( ! ( n ) ) << 31 ) >> 31;
  int helper4 = ~ ( ~0 << n )   ;
  int rightside = x << n;
  int leftside = x >> (32 + ~n + 1);
  int adder;
  adder =  helper4 & leftside;
  return ( ( rightside + adder ) & ( ~ helper3 ) ) + ( x & helper3 );
}
/* 
 * signMag2TwosComp - Convert from sign-magnitude to two's complement
 *   where the MSB is the sign bit
 *   Example: signMag2TwosComp(0x80000005) = -5.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 4
 */
int signMag2TwosComp(int x) {
  /* firstly, we get the sign bit,
   * because in this function, the Twos Complement of positive number is itself,
   * and the Twos complement of negative number is reverse all the bits and add 1 at the end,
   * so we need a helper if x is negative,
   * if the sign bit number we get is 0x00000000, that means we don't need to change x value,
   * and the process of converting negative number will skip, so we use & sign and &(~sign),
   * if the sign bit number is 0xffffffff, that means we should convert the negative number,
   * by change the sign bit into 0 and thenreverse all the remaining bits and add 1 at the end,
   * And finally add the origina sign bit back, which is sign<<31,
   * For number 0x80000000, the original sign bit is 1, and the MSB after converting is 1, too;
   * so we need an add operator to set that 1 into 0, so we can output 0;
   */ 
  int sign = x >> 31;
  int helper = ~ x + 1 ;  
  return (( helper + (sign << 31) ) & sign) + (x & ( ~ sign )) ; 
}
/* 
 * thirdBits - return word with every third bit (starting from the LSB) set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int thirdBits(void) {
  /* firstly create a constant 01001001
   * then use left shift 9 bits and add itself to create a new constant 01001001001001001;
   * we shift left the new constant by 15 bits , 
   * so the rightside 15 bits are set to 0s but should be 001001001001001;
   * to get that number , we can shift right x by 3 bits(here x is not left shift 15 bits yet);
   * then add them together, we can get what we want, the 'thirdBits' constant number!
   */
  int x = 73;
  x = ( x << 9 ) + x;
  x = ( x << 15 ) + ( x >> 3 ); 
  return x;
}
