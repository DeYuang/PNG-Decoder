#ifndef TEXTURE_PNG_H
#define TEXTURE_PNG_H
#include <nds.h>
#include "fixed.h"
#include "texture.h"

#define PNGFileSignatureLength 8
#define	PNGChunkBaseToLength 4
#define	PNGChunkTypeLength 4
#define PNGchunkCRCLength 4

#define PNGChunkBaseToType (PNGChunkBaseToLength)
#define PNGChunkBaseToData (PNGChunkBaseToLength+PNGChunkTypeLength)
#define PNGChunkBaseToCRC(n) (PNGChunkBaseToData+n)
#define PNGchunkHeaderLength (PNGChunkBaseToData+PNGchunkCRCLength) 
#define PNGChunkBaseToNext(n) (n + PNGchunkHeaderLength)

enum PNGColorType :			uint8		{ Greyscale = 0, Truecolour = 2, IndexedColour = 3, GreyscaleAlpha = 4,
										   TruecolourAlpha = 6};
enum PNGCompressionType :	uint8		{ Deflate = 0 };
enum PNGInterlacingType :	uint8		{ None = 0, Adam7 = 1};
enum PNGFilterMethod	 :	uint8		{ Adaptive = 0};
enum PNGAdaptiveFilter	:	uint8		{ NoFilter = 0, Sub = 1, Up = 2, Average = 3, Paeth = 4};
enum PNGChunkType :			uint32		{ IHDR = 1380206665, pHYs = 1935231088, sRGB = 1111970419, IDAT = 1413563465, gAMA = 1095582055, iCCP = 1346585449, cHRM = 1297238115, IEND = 1145980233};
//enum PNGUnitSpecifier :		uint8		{ unknown = 0, Meter = 1};

typedef struct PNGInfo {

	public:
		uint16				imageWidth;
		uint16				imageHeight;
		uint8				bytesPerChannel;
		PNGColorType		pngColorType;
		PNGCompressionType	pngCompressionType;
		PNGFilterMethod		pngFilterMethod;
		PNGInterlacingType	pngInterlacingType;
		real				gamma;// = div_int_int(22, 10);
		
} PNGInfo; 

Texture* DecodePNG(char* fileContents, uint16 fileLength);

static char* PNGColorTypeAsString(const PNGColorType input){

	switch(input){
	
		case Greyscale:
			return (char*)"Greyscale";
			
		case Truecolour:
			return (char*)"TrueColour";
			
		case IndexedColour:
			return (char*)"Indexed Colour";
			
		case GreyscaleAlpha:
			return (char*)"Greyscale with Alpha";
			
		case TruecolourAlpha:
			return (char*)"Truecolour with Alpha";
	
		default:{
			iprintf("Texture_PNG.h PNGColorTypeAsString() Error: Default error!\r\n");
			return (char*)"";
		}
	}
}

static char* PNGCompressionTypeAsString(const PNGCompressionType input){

	switch(input){
	
		case Deflate:
			return (char*)"Deflate";
	
		default:{
			iprintf("Texture_PNG.h PNGCompressionTypeAsString() Error: Default error!\r\n");
			return (char*)"";
		}
	}
}

static char* PNGInterlacingTypeAsString(const PNGInterlacingType input){

	switch(input){
	
		case None:
			return (char*)"None";
			
		case Adam7:
			return (char*)"Adam7 Interlacing";
			
	
		default:{
			iprintf("Texture_PNG.h PNGInterlacingTypeAsString() Error: Default error!\r\n");
			return (char*)"";
		}
	}
}

static char* PNGFilterMethodAsString(const PNGFilterMethod input){

	switch(input){
	
		case Adaptive:
			return (char*)"Adaptive";
	
		default:{
			iprintf("Texture_PNG.h PNGFilterMethodAsString() Error: Default error!\r\n");
			return (char*)"";
		}
	}
}

static char* PNGAdaptiveFilterAsString(const PNGAdaptiveFilter input){


	iprintf("(%u) ", (uint8)input);
	switch(input){
	
		case NoFilter:
			return (char*)"No Filter";
			
		case Sub:
			return (char*)"Sub";
		
		case Up:
			return (char*)"Up";
		
		case Average:
			return (char*)"Average";
		
		case Paeth:
			return (char*)"Paeth";
	
		default:{
			return (char*)"Unknown";
		}
	}
}

static char* PNGChunkTypeAsString(const PNGChunkType input){

	switch(input){
	
		case IHDR:
			return (char*)"Image Header";
			
		case IDAT:
			return (char*)"Image Data";
			
		case IEND:
			return (char*)"Image End";
	
		case pHYs:
			return (char*)"Physical pixel size";
		
		case sRGB:
			return (char*)"RGB Color Space";
	
		case gAMA:
			return (char*)"Image Gamma";
			
		case iCCP:
			return (char*)"ICC Profile";
			
		case cHRM:
			return (char*)"Chromaticities Data";	
	
		default:{
			iprintf("Texture_PNG.h PNGChunkTypeAsString() Error: Default error!\r\n");
			return (char*)"";
		}
	}
}

static inline bool PNGChunkTypeRecognized(const PNGChunkType input){

	if(input == IHDR || input == IDAT || input == IEND ||
		   input == pHYs || input == sRGB || input == gAMA ||
		   input == iCCP || input == cHRM)
		return true;
	return false;
}

static inline bool PNGChunkTypeSupported(const PNGChunkType input){

	if(input == IHDR || input == IDAT || input == IEND || input == gAMA)
		return true;
	return false;
}

/*static char* PNGUnitSpecifierAsString(const PNGUnitSpecifier input){

	switch(input){
	
		case unknown:
			return (char*)"Unknown";
			
		case Meter:
			return (char*)"Meter";
			
	
		default:{
			iprintf("Texture_PNG.h PNGUnitySpecifierAsString() Error: Default error!\r\n");
			return (char*)"";
		}
	}
}*/
#endif