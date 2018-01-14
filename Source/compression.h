#ifndef COMPRESSION_H
#define COMPRESSION_H

enum DEFLATECompressionType : uint8 { uncompressed = 0, fixedHuffman = 1, dynamicHuffman = 2, reserved = 3 };

void DEFLATE (char* data);
void INFLATE (char* data);

static inline char* DEFLATECompressionTypeAsString(const DEFLATECompressionType input){

	switch(input){
			
		case uncompressed:
			return (char*)"Uncompressed";
			
		case fixedHuffman:
			return (char*)"Fixed Huffman Tree";
			
		case dynamicHuffman:
			return (char*)"Dynamic Huffman Tree";
			
		case reserved:
			return (char*)"Reserved";
	
		default:{
			iprintf("\r\ncompression.h DEFLATECompressionTypeAsString() Error: Default error!\r\n");
			return (char*)"";
		}
	}
}
#endif