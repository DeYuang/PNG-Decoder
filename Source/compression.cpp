#include <nds.h>
#include <stdio.h>
#include "compression.h"

void DEFLATE (char* data){


	return;
}

void INFLATE (char* data){

	iprintf("\r\nStarting INFLATE\r\n");
	
	uint8 rawData = data[0];
	
	bool final = rawData && 0b00000001;
	uint8 btype = (rawData && 0b00000110) >> 1;
	
	if(final)
		iprintf("Final Block\r\n");
	
	DEFLATECompressionType compressionType = *(DEFLATECompressionType*)&btype;
	
	iprintf("Compression Type: ");
	DEFLATECompressionTypeAsString(compressionType);
	iprintf("\r\n\r\n");
	
/*  if stored with no compression
        skip any remaining bits in current partially processed byte
        read LEN and NLEN (see next section)
        copy LEN bytes of data to output
    otherwise
        if compressed with dynamic Huffman codes
            read representation of code trees (see subsection below)
            do (until end of block code recognized)
                decode literal/length value from input stream
                if value < 256
                    copy value (literal byte) to output stream
                otherwise
                    if value = end of block (256)
                        break;
                    otherwise (value = 257..285)
                        decode distance from input stream

                move backwards distance bytes in the output stream, and copy length bytes from this position to the output stream.
            end loop
            while (not last block)
	*/
	
	return;
}