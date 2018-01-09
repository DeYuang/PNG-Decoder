#include "fixed.h"
#include "texture.h"
#include "texture_PNG.h"
#include "string.cpp"

static Texture* LoadTexture(const char* path){

	iprintf("Tex:");
	iprintf(path);
	iprintf("\r\n");

	if(FILE* inf = fopen(path, "rb"))	{
		fseek(inf,0,SEEK_END);
		int32 len = ftell(inf);
		fseek(inf,0,SEEK_SET);

		char *fileContents = (char*)malloc(len+1);
		fileContents[len] = 0;
		if(fread(fileContents, 1, len, inf) != len)
			iprintf("Texture.cpp LoadTexture() Error: Savage error!\r\n");
		else{			
			if(fileContents[0] == 0x42 &&		// BMP magic bits "BM"
				fileContents[1] == 0x4D){
				iprintf("Decoding BMP\r\n");
				return DecodeAndScaleBMP(fileContents);
			}
			if(fileContents[1] == 0x50 &&		// PNG magic bits "PNG"
				fileContents[2] == 0x4E &&		// PNG magic bits 
				fileContents[3] == 0x47){		// PNG magic bits 
				iprintf("Decoding PNG\r\n");

				return DecodePNG(fileContents, len);
			}
			else{
				iprintf("Texture type not recognized!\r\n");	
			}		
		}
			
		free(fileContents);

		fclose(inf);
	}
}

static inline Texture* DecodePNG(char* fileContents, uint16 fileLength){

	// The data of the PNG file starts right after the file signature
	uint32 chunkBase = PNGFileSignatureLength;
	
	PNGInfo* pngInfo = (PNGInfo*)malloc(sizeof(PNGInfo));
	
	uint8 unknownChunksToDisplay = 4;
	while(chunkBase < fileLength){
		//uint32 chunkDataLength = SwapEndian(*(uint32*)&fileContents[chunkBase]);
		//uint32 chunkDataLength = *(uint32*)&fileContents[chunkBase+2];
		
		uint32 chunkDataLength;
		{
			uint8 char0 = *(uint8*)&fileContents[chunkBase];
			uint8 char1 = *(uint8*)&fileContents[chunkBase+1];
			uint8 char2 = *(uint8*)&fileContents[chunkBase+2];
			uint8 char3 = *(uint8*)&fileContents[chunkBase+3];
		
			chunkDataLength = (char0 << 24 | char1 << 16 | char2 << 8 | char3);
		}
		
		// Get the next chunk's type	
		PNGChunkType chunkType = *(PNGChunkType*)&fileContents[chunkBase+PNGChunkBaseToType];
		
		if(PNGChunkTypeSupported(chunkType)){
			iprintf("Chunk type: ");
			iprintf(PNGChunkTypeAsString(chunkType));
			iprintf("\r\n");
			
			/*{
				uint8 char0 = *(uint8*)&fileContents[chunkBase];
				uint8 char1 = *(uint8*)&fileContents[chunkBase+1];
				uint8 char2 = *(uint8*)&fileContents[chunkBase+2];
				uint8 char3 = *(uint8*)&fileContents[chunkBase+3];
				
				iprintf("Raw length Bytes:%u %u %u %u\r\n", char0, char1, char2, char3);
			}
			{
				uint8 char0 = *(uint8*)(&chunkDataLength);
				uint8 char1 = *(uint8*)(((uint8*)&chunkDataLength)+1);
				uint8 char2 = *(uint8*)(((uint8*)&chunkDataLength)+2);
				uint8 char3 = *(uint8*)(((uint8*)&chunkDataLength)+3);
				
				iprintf("Length Bytes: %u %u %u %u\r\n", char0, char1, char2, char3);
			}*/
			
			//iprintf("Chunk length in Bytes: %u\r\n", chunkDataLength);
		
			if(chunkType == IHDR){
			
				pngInfo->imageWidth = SwapEndian(*(uint16*)&fileContents[18]);
				pngInfo->imageHeight = SwapEndian(*(uint16*)&fileContents[22]);
				pngInfo->bytesPerChannel = *(uint8*)&fileContents[24];
				pngInfo->pngColorType = (PNGColorType)fileContents[25];
				pngInfo->pngInterlacingType = (PNGInterlacingType)fileContents[28];
				
				//uint16 bitmapWidth = SwapEndian(*(uint16*)&fileContents[18]);
				//uint16 bitmapHeight = SwapEndian(*(uint16*)&fileContents[22]);
				//uint8 colorDepth = *(uint8*)&fileContents[24];
				//PNGColorType colorType = (PNGColorType)fileContents[25];
				//PNGInterlacingType interlacingType = (PNGInterlacingType)fileContents[28];
				
				/*iprintf("Color Depth: %u BPP\r\nImage Size: %ux%u\r\n", colorDepth, bitmapWidth, bitmapHeight);
				iprintf("Color Type: ");
				iprintf(PNGColorTypeAsString(colorType));
				iprintf("\r\nInterlacing Type: ");
				iprintf(PNGInterlacingTypeAsString(interlacingType));
				iprintf("\r\n");*/
			}
			/*else if(chunkType == pHYs){
				//chunkDataLength = 9;
				
				uint32 pixelPerunitX = *(uint32*)&fileContents[chunkBase+PNGChunkBaseToData];
				uint32 pixelPerUnitY = *(uint32*)&fileContents[chunkBase+PNGChunkBaseToData+4];
				PNGUnitSpecifier unitSpecifier = (PNGUnitSpecifier)*(uint8*)&fileContents[chunkBase+PNGChunkBaseToData+5];
				
				//iprintf("pHYs Data length: %u Bytes\r\n", chunkDataLength);
				if(unitSpecifier == Meter)
					iprintf("Pixels per Meter: %u, %u\r\n\r\n", pixelPerunitX, pixelPerUnitY);
				else
					iprintf("Aspect ratio: %u:%u\r\n\r\n", pixelPerunitX, pixelPerUnitY);
			}*/
			else if(chunkType == IDAT){
			
				uint8 compressionMethod = *(uint8*)&fileContents[chunkBase+PNGChunkBaseToData];
				uint8 flags = *(uint8*)&fileContents[chunkBase+PNGChunkBaseToData+1];
				
				iprintf("Compression method: %u\r\nFlags: %u\r\n", compressionMethod, flags);
			}
			else if(chunkType == gAMA){
			
				uint32 gammaBytes;
				{
					uint8 char0 = *(uint8*)&fileContents[chunkBase+PNGChunkBaseToData];
					uint8 char1 = *(uint8*)&fileContents[chunkBase+PNGChunkBaseToData+1];
					uint8 char2 = *(uint8*)&fileContents[chunkBase+PNGChunkBaseToData+2];
					uint8 char3 = *(uint8*)&fileContents[chunkBase+PNGChunkBaseToData+3];
				
					gammaBytes = (char0 << 24 | char1 << 16 | char2 << 8 | char3);
				}
				pngInfo->gamma = div_int_int(gammaBytes, 100000);
				
				iprintf("Raw Gamma: %u\r\n", gammaBytes);
			}
		}
		else if(unknownChunksToDisplay > 0){
			unknownChunksToDisplay --;
			uint8 char0 = fileContents[chunkBase+PNGChunkBaseToType];
			uint8 char1 = fileContents[chunkBase+PNGChunkBaseToType+1];
			uint8 char2 = fileContents[chunkBase+PNGChunkBaseToType+2];
			uint8 char3 = fileContents[chunkBase+PNGChunkBaseToType+3];
			
			iprintf("Unknown chunk type: %c%c%c%c\r\n", char0, char1, char2, char3);
			//iprintf("Type Number: %u\r\n", *(uint32*)&fileContents[chunkBase+PNGChunkBaseToType]);
			//iprintf("Chunk length: %u\r\n", chunkDataLength);
		}
		
		// Next
		chunkBase += PNGChunkBaseToNext(chunkDataLength);
	}

	iprintf("\r\nPNGInfo:\r\nColor Depth: %u BPP\r\nImage Size: %ux%u\r\n", pngInfo->bytesPerChannel, pngInfo->imageWidth, pngInfo->imageHeight);
	iprintf("Color Type: ");
	iprintf(PNGColorTypeAsString(pngInfo->pngColorType));
	iprintf("\r\nInterlacing Type: ");
	iprintf(PNGInterlacingTypeAsString(pngInfo->pngInterlacingType));
	iprintf("\r\nGamma: ");
	printfixed16r16(pngInfo->gamma);
	iprintf("\r\n\r\n");

	Texture* texture = new Texture();
	texture->width = pngInfo->imageWidth;
	texture->height = pngInfo->imageHeight;
	//texture->textureType = BMP16;

	iprintf("Image loaded!\r\n");

	return texture;
}

static inline Texture* DecodeAndScaleBMP(char* fileContents){

	uint32 headerLength = *(uint32*)&fileContents[14];
	BMPHeaderVersion FileHeaderVersion = GetBMPHeaderVersion(headerLength);
	
	void* pixelDataBase = (void*)&fileContents[10];
	
	uint16 bitmapWidth = *(uint16*)&fileContents[18];
	uint16 bitmapHeight = *(uint16*)&fileContents[22];
	uint16 colorDepth = *(uint16*)&fileContents[28];
	
	BMPCompressionType compressionType = (BMPCompressionType)*(uint32*)&fileContents[30];
	
	iprintf("\r\nCompression Type: ");
	iprintf(BMPCompressionTypeAsString(compressionType));
	iprintf("\r\nColor Depth: %u BPP\r\nImage Size: %ux%u\r\n\r\n", colorDepth, bitmapWidth, bitmapHeight);
	iprintf("Compression Target: BMP16\r\nTarget Color depth: 15 BPP\r\nTarget Image Size: 256x192\r\n");
	
	// TODO: resize image if the size is not the target size
	const uint16 targetImageWidth = 256;
	const uint16 targetImageHeight = 192;
	real scalingfactorVertical = div_int_int(bitmapWidth, targetImageWidth);
	real scalingfactorHorizontal = div_int_int(bitmapHeight, targetImageHeight);
	iprintf("Scaling Factor: ");
	printfixed16r16(scalingfactorVertical);
	iprintf("x");
	printfixed16r16(scalingfactorHorizontal);
	iprintf("\r\n");
	
	// Decode to BgType_Bmp16?
	// Bitmap background with 16 bit color values of the form aBBBBBGGGGGRRRRR
	// (if 'a' is set the pixel will be rendered... if not, the pixel will be transparent)
	
	// What we got now is BBBBBBBBGGGGGGGGGRRRRRRRRR
	// so then we just need to scale
	
	// BGR: 8 > 5 bits
	
	Texture* texture = new Texture();
	texture->width = targetImageWidth;
	texture->height = targetImageHeight;
	texture->textureType = BMP16;
	
	uint32 pixelCount = targetImageWidth * targetImageHeight;
	//uint32 sourceStoreSpace = *(uint32*)&fileContents[0x22];
	uint32 bmp16StoreSpace = pixelCount * sizeof(uint16);
	
	// Get Free Store space
	uint16* data = (uint16*)malloc(bmp16StoreSpace);
	uint16* bmp16StoreCurrent = (uint16*)data + pixelCount - targetImageWidth;
	uint8*	sourceDataCurrent = (uint8*)pixelDataBase;
	uint16* bmp16StoreEnd = data + pixelCount;
	
	// Note: we need to flip output vertically
	// BMP Pixel 0:
	// O  O  O
	// O  O  O
	// X  O  O
	// BMP16 Pixel 0
	// X  O  O
	// O  O  O
	// O  O  O
	// Solution: We are reading the source file front-to-back
	// however, the dest is being written to out-of-order.
	
	uint16 currentPixelInRow = 0;
	uint16 currentRow = targetImageHeight;
	
	// Now, to convert from 8 bits to 5 bits, we need to divide the source by 2^3
	const uint16 alphaBit = 1 << 15;
	
	//while(bmp16StoreCurrent < bmp16StoreEnd){	
	while(currentRow > 0){
		
		*bmp16StoreCurrent++ = alphaBit | ((*sourceDataCurrent >> 3) << 10)
		| (((*sourceDataCurrent+1) >> 3) << 5) | ((*sourceDataCurrent+2) >> 3);
		sourceDataCurrent += fixed_int(mul_fixed_int(scalingfactorVertical, 3));
		
		if(++currentPixelInRow == targetImageWidth){
			// Go to next row
			currentPixelInRow = 0;
			currentRow--;
			bmp16StoreCurrent -= bitmapWidth*2;
			
			// If, at the end of a line, the bytes don't line up with a factor of 4, the next few bits should be skipped
			sourceDataCurrent += (uint32)sourceDataCurrent % 4;
		}
	}
	texture->data = (void*)data;
	texture->dataSize = bmp16StoreSpace;
	
	return texture;
}

static inline Texture* DecodeBMP(char* fileContents){

	uint32 headerLength = *(uint32*)&fileContents[14];
	BMPHeaderVersion FileHeaderVersion = GetBMPHeaderVersion(headerLength);
	
	void* pixelDataBase = (void*)&fileContents[10];
	
	int32 bitmapWidth = *(uint16*)&fileContents[18];
	int32 bitmapHeight = *(uint16*)&fileContents[22];
	uint16 colorDepth = *(uint16*)&fileContents[28];
	
	BMPCompressionType compressionType = (BMPCompressionType)*(uint32*)&fileContents[30];
	
	iprintf("\r\nCompression Type: ");
	iprintf(BMPCompressionTypeAsString(compressionType));
	iprintf("\r\nColor Depth: %u BPP\r\nImage Size: %ux%u\r\n\r\n", colorDepth, bitmapWidth, bitmapHeight);


	iprintf("Compression Target: BMP16\r\nTarget Color depth: 15 BPP\r\nTarget Image Size: 256x192\r\n");
	
	// TODO: resize image if the size is not the target size
	
	// Decode to BgType_Bmp16?
	// Bitmap background with 16 bit color values of the form aBBBBBGGGGGRRRRR
	// (if 'a' is set the pixel will be rendered... if not, the pixel will be transparent)
	
	// What we got now is BBBBBBBBGGGGGGGGGRRRRRRRRR
	// so then we just need to scale
	
	// BGR: 8 > 5 bits
	
	Texture* texture = new Texture();
	texture->width = bitmapWidth;
	texture->height = bitmapHeight;
	texture->textureType = BMP16;
	
	uint32 pixelCount = bitmapWidth * bitmapHeight;
	//uint32 sourceStoreSpace = *(uint32*)&fileContents[0x22];
	uint32 bmp16StoreSpace = pixelCount * sizeof(uint16);
	
	// Get Free Store space
	uint16* data = (uint16*)malloc(bmp16StoreSpace);
	uint16* bmp16StoreCurrent = (uint16*)data + pixelCount - bitmapWidth;
	uint8*	sourceDataCurrent = (uint8*)pixelDataBase;
	uint16* bmp16StoreEnd = data + pixelCount;
	
	
	// Note: we need to flip output vertically
	// BMP Pixel 0:
	// O  O  O
	// O  O  O
	// X  O  O
	// BMP16 Pixel 0
	// X  O  O
	// O  O  O
	// O  O  O
	// Solution: We are reading the source file front-to-back
	// however, the dest is being written to out-of-order.
	
	uint16 currentPixelInRow = 0;
	uint16 currentRow = bitmapHeight;
	
	// Now, to convert from 8 bits to 5 bits, we need to divide the source by 2^3
	// But this is highly lossy and will cause compression artifacts
	const uint8 scaler = 8;
	iprintf("Compression scaler: %u\r\n", scaler);
	const uint16 alphaBit = 1 << 15;
	
	//while(bmp16StoreCurrent < bmp16StoreEnd){	
	while(currentRow > 0){
		
		*bmp16StoreCurrent++ = alphaBit | ((*sourceDataCurrent >> 3) << 10)
		| (((*sourceDataCurrent+1) >> 3) << 5) | ((*sourceDataCurrent+2) >> 3);
		sourceDataCurrent += 3;
		
		if(++currentPixelInRow == bitmapWidth){
			// Go to next row
			currentPixelInRow = 0;
			currentRow--;
			bmp16StoreCurrent -= bitmapWidth*2;
			
			// If, at the end of a line, the bytes don't line up with a factor of 4, the next few bits should be skipped
			sourceDataCurrent += (uint32)sourceDataCurrent % 4;
		}
	}
	texture->data = (void*)data;
	texture->dataSize = bmp16StoreSpace;
	
	return texture;
}

static inline BMPHeaderVersion GetBMPHeaderVersion(const uint32 headerLength){

	if(headerLength == 40){
		iprintf("Header version: ");
		iprintf(BMPHeaderVersionAsString(BITMAPINFOHEADER));
		return BITMAPINFOHEADER;
	}
	else{
		iprintf("Texture.cpp GetBMPHeaderVersionError: Header with length of %u is not supported!\r\n", headerLength);
		return Unknown;
	}
}