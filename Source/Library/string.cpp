#ifndef STRING
#define STRING
#include <nds.h>
#include <stdint.h>
#include "string.h"
#include "fixed.h"

static inline void printfixed16r16(fixed16r16 input){

	int16 whole = fixed_int(input);
	uint16 fractional;
	if(whole < 0)
		fractional = 65536 - fixed_int(div_int_fixed(int_fix_fractional(input), div_int_int(65536, 10000)));
	else
		fractional = fixed_int(div_int_fixed(int_fix_fractional(input), div_int_int(65536, 10000)));
	
	
	if(fractional == 0)
		iprintf("%d.0000", whole);
	else if(fractional  < 10)
		iprintf("%d.000%u", whole, fractional);
	else if(fractional  < 100)
		iprintf("%d.00%u", whole, fractional);
	else if(fractional  < 1000)
		iprintf("%d.0%u", whole, fractional);
	else
		iprintf("%d.%u", whole, fractional);
}
#endif