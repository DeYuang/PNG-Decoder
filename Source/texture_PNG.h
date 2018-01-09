#ifndef TEXTURE_PNG_H
#define TEXTURE_PNG_H

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
enum PNGInterlacingType :	uint8		{ None = 0, Adam7 = 1};
enum PNGChunkType :			uint32		{ IHDR = 1380206665, pHYs = 151013488, sRGB = 21451379, IDAT = 507593801, gAMA = 67125607};
enum PNGUnitSpecifier :		uint8		{ unknown = 0, Meter = 1};

typedef struct PNGInfo {

	public:
		uint16	imageWidth;
		uint16	imageHeight;
		uint8	bytesPerChannel;
		PNGColorType pngColorType;
		PNGInterlacingType pngInterlacingType;
		real gamma;
		
} PNGInfo; 

static char* PNGColorTypeAsString(const PNGColorType input){

	switch(input){
	
		case Greyscale:
			return (char*)"Greyscale";
			
		case Truecolour:
			return (char*)"Truecolour";
			
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

static char* PNGChunkTypeAsString(const PNGChunkType input){

	switch(input){
	
		case IHDR:
			return (char*)"Image Header";
	
		case pHYs:
			return (char*)"Physical pixel size";
		
		case sRGB:
			return (char*)"RGB Color Space";
		
		case IDAT:
			return (char*)"Image Data";
	
		case gAMA:
			return (char*)"Image Gamma";
	
		default:{
			iprintf("Texture_PNG.h PNGChunkTypeAsString() Error: Default error!\r\n");
			return (char*)"";
		}
	}
}

static inline bool PNGChunkTypeSupported(const PNGChunkType input){

	if(input == IHDR || input == pHYs || input == sRGB || input == IDAT || input == gAMA)
		return true;
	return false;
}

static char* PNGUnitySpecifierAsString(const PNGUnitSpecifier input){

	switch(input){
	
		case Unknown:
			return (char*)"Unknown";
			
		case Meter:
			return (char*)"Meter";
			
	
		default:{
			iprintf("Texture_PNG.h PNGUnitySpecifierAsString() Error: Default error!\r\n");
			return (char*)"";
		}
	}
}
#endif