#include <nds.h>
#include <stdio.h>
#include "compression.h"

char* DEFLATE (char* data){


	return data;
}

char* INFLATE (char* data){

	iprintf("->Starting INFLATE\r\n");
	uint16 blockCount = 1;
	
	uint8 rawData = data[0];
	
	bool isFinalBlock = rawData && 0b00000001;
	uint8 btype = (rawData && 0b00000110) >> 1;
	
	DEFLATECompressionType compressionType = *(DEFLATECompressionType*)&btype;	
	iprintf("Compression Type: ");
	iprintf(DEFLATECompressionTypeAsString(compressionType));
	iprintf("\r\n");
	
	/*if(isFinalBlock){
		if(blockCount == 1)
			iprintf("Single Block\r\n");
		else*/
			iprintf("Final Block: %u Total Blocks\r\n", blockCount);
	//}
	//else
	//	iprintf("New Block\r\n");
	
	if(compressionType == uncompressed && isFinalBlock){
		// We don't need to anything here, just return what we were given
		// minus the first bit
		//iprintf("\r\n");
		iprintf("->Stopping INFLATE\r\n");
		return (char*)(&data+1);
	}
	
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
	
	iprintf("\r\n");
	
	return data;
}