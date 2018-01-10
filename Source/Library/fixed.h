#ifndef FIXED_H
#define FIXED_H
#include <nds.h>
#include "assembly.cpp"

// Note(Anne): 16r16 extentions
typedef int32 fixed16r16;
#define fixed_1					65536
#define fixed_infinitessimal	1
#define int_fixed(n)			(n << 16)
#define fixed_int(n)			(n >> 16)
#define float_fixed(n)			((fixed16r16)(n * 65536.0f))
#define fixed_float(n) 			((float)n / 65536.0f)
#define mul_fixed_fixed(a, b)	(mul_fixed_fixed_macro(a, b))
#define mul_fixed_int(a, b)		((fixed16r16)((s64)a * (s64)b))
#define div_fixed_fixed(a, b)	((fixed16r16)(((s64)a << 16) / b))
#define div_int_int(a, b)		((fixed16r16)(((s64)a << 32) / ((s64)b << 16)))
#define div_int_fixed(a, b)		((fixed16r16)(((s64)a << 32) / b))
#define div_fixed_int(a, b)		(a / b)
#define int_fix_fractional(n)	(uint16)((*(uint16*)&n))
#define bucketFactor 			(int_fixed(360))

#define sqrt_inv_fixed(n)		(float_fixed(sqrt_inv_float(fixed_float(n))))
#define sqrt_fixed(n)			(div_int_fixed(1, n))

/*class fixed{

	private:
		int32 data;
	public:
		fixed(int input){
			data = int_fixed(input);
		}
		fixed(float input){
			data = float_fixed(input);
		}
		operator int(){
			return fixed_int(data);
		}
		operator float(){
			return fixed_float(data);
		}
		fixed operator +(fixed rValue){
			return (fixed)data+rValue.data;
		}
		fixed operator -(fixed rValue){
			return (fixed)data-rValue.data;
		}
		fixed operator *(fixed rValue){
			return (fixed)mul_fixed_fixed(data, rValue.data);
		}
		fixed operator /(fixed rValue){
			return (fixed)div_fixed_fixed(data, rValue.data);
		}
};*/

// Note(Anne): 20r12 extentions 
typedef int32 fixed20r12;
typedef fixed20r12 f32;
#define fixed_f32(n)			(n >> 4)
#define f32_fixed(n)			(n << 4)

// Note(Anne): Generic datatype setup
typedef fixed16r16 fixed;		// The default setup for the "fixed" datatype is 16r16
typedef fixed real;				// The default method of storing a real number is as a fixed point.
#define sqrt_inv(n)				(sqrt_inv_fixed(n))
#define sqrt(n)					(sqrt_fixed(n))
#endif