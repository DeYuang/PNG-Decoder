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
		
			if(chunkType == IHDR){
			
				pngInfo->imageWidth = SwapEndian(*(uint16*)&fileContents[18]);
				pngInfo->imageHeight = SwapEndian(*(uint16*)&fileContents[22]);
				pngInfo->bytesPerChannel = *(uint8*)&fileContents[24];
				pngInfo->pngColorType = (PNGColorType)fileContents[25];
				pngInfo->pngInterlacingType = (PNGInterlacingType)fileContents[28];
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
	texture->textureType = BMP16;

	iprintf("Image loaded!\r\n");

	return texture;
}