#include "texture_PNG.h"
#include "string.h"
#include "compression.h"

Texture* DecodePNG(char* fileContents, uint16 fileLength){

	// The data of the PNG file starts right after the file signature
	uint32 chunkBase = PNGFileSignatureLength;
	
	PNGInfo* pngInfo = (PNGInfo*)malloc(sizeof(PNGInfo));
	pngInfo->gamma = div_int_int(22, 10);
	
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
		
		uint32 chunkNumber;
		{
			uint8 char0 = *(uint8*)&fileContents[chunkBase+PNGChunkBaseToType];
			uint8 char1 = *(uint8*)&fileContents[chunkBase+PNGChunkBaseToType+1];
			uint8 char2 = *(uint8*)&fileContents[chunkBase+PNGChunkBaseToType+2];
			uint8 char3 = *(uint8*)&fileContents[chunkBase+PNGChunkBaseToType+3];
			
			chunkNumber = (char3 << 24 | char2 << 16 | char1 << 8 | char0);
		}
		
		PNGChunkType chunkType = *(PNGChunkType*)&chunkNumber;
		
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
			
				//uint8 compressionMethod = fileContents[chunkBase+PNGChunkBaseToData];
				//uint8 flags = fileContents[chunkBase+PNGChunkBaseToData+1];
				
				//iprintf("Compression method: %u\r\nFlags: %u\r\n", compressionMethod, flags);
				
				INFLATE(&fileContents[chunkBase+PNGChunkBaseToData]);
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
			else if(chunkType == IEND){
			
				break;
			}
		}
		else if(PNGChunkTypeRecognized(chunkType)){
			iprintf("Skipped: ");
			iprintf(PNGChunkTypeAsString(chunkType));
			iprintf("\r\n");
		}
		else if(unknownChunksToDisplay > 0){
			unknownChunksToDisplay --;
			uint8 char0 = fileContents[chunkBase+PNGChunkBaseToType];
			uint8 char1 = fileContents[chunkBase+PNGChunkBaseToType+1];
			uint8 char2 = fileContents[chunkBase+PNGChunkBaseToType+2];
			uint8 char3 = fileContents[chunkBase+PNGChunkBaseToType+3];
			
			iprintf("Unknown chunk type: %c%c%c%c\r\n", char0, char1, char2, char3);
			iprintf("(%u)\r\n", chunkNumber);
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