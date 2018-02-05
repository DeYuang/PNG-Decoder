#include "texture_PNG.h"
#include "string.h"
#include "compression.h"

Texture* DecodePNG(char* fileContents, uint16 fileLength){

	// The data of the PNG file starts right after the file signature
	uint32 chunkBase = PNGFileSignatureLength;
	
	PNGInfo* pngInfo = (PNGInfo*)malloc(sizeof(PNGInfo));
	pngInfo->gamma = div_int_int(22, 10);
	
	Texture* texture = new Texture();
	
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
			
				{
					uint8 char0 = *(uint8*)&fileContents[18];
					uint8 char1 = *(uint8*)&fileContents[19];
					uint8 char2 = *(uint8*)&fileContents[22];
					uint8 char3 = *(uint8*)&fileContents[23];
					
					pngInfo->imageWidth = (char0 << 8 | char1);
					pngInfo->imageHeight = (char2 << 8 | char3);
				}
				
				pngInfo->bytesPerChannel = *(uint8*)&fileContents[24];
				pngInfo->pngColorType = (PNGColorType)fileContents[25];
				pngInfo->pngCompressionType = (PNGCompressionType)fileContents[chunkBase+PNGChunkBaseToData+10];
				pngInfo->pngFilterMethod = (PNGFilterMethod)fileContents[chunkBase+PNGChunkBaseToData+11];
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
			else if(chunkType == gAMA){
			
				uint32 gammaBytes;
				{
					uint8 char0 = *(uint8*)&fileContents[chunkBase+PNGChunkBaseToData];
					uint8 char1 = *(uint8*)&fileContents[chunkBase+PNGChunkBaseToData+1];
					uint8 char2 = *(uint8*)&fileContents[chunkBase+PNGChunkBaseToData+2];
					uint8 char3 = *(uint8*)&fileContents[chunkBase+PNGChunkBaseToData+3];
				
					gammaBytes = (char0 << 24 | char1 << 16 | char2 << 8 | char3);
				}
				pngInfo->gamma = div_int_fixed(1, div_int_int(gammaBytes, 100000));
				
				iprintf("->Raw Gamma: %u\r\n", gammaBytes);
			}
			else if(chunkType == IDAT){				
				if(pngInfo->pngCompressionType == Deflate){
					char* imageData = INFLATE(&fileContents[chunkBase+PNGChunkBaseToData]);
					char* imageStoreCurrent = imageData;
					
					uint8 sourceBitDepth = pngInfo->bytesPerChannel;
					uint16 length = pngInfo->imageHeight;
					uint16 width = pngInfo->imageWidth;
					uint32 pixelCount = length*width;
					
					// Get Free Store space
					uint32 bmp16StoreSpace = pixelCount * sizeof(uint16);
					uint16* data = (uint16*)malloc(bmp16StoreSpace);
					uint16* bmp16StoreCurrent = data;					
					
					const uint16 alphaBit = 1 << 15;
					const uint8 targetBitDepth = 5;
					uint8 shiftFactor = sourceBitDepth - targetBitDepth;
					uint16 pixelInScanline = width-1;
					
					PNGAdaptiveFilter pngAdaptiveFilter = *(PNGAdaptiveFilter*)&imageData;
					imageData++;
					iprintf("->Adaptive Filter:");
					iprintf(PNGAdaptiveFilterAsString(pngAdaptiveFilter));
					iprintf("\r\n");
					
					/*PNGAdaptiveFilter pngAdaptiveFilter2 = *(PNGAdaptiveFilter*)(&imageData + (width*3));
					iprintf("->Filter 2:");
					iprintf(PNGAdaptiveFilterAsString(pngAdaptiveFilter2));
					iprintf("\r\n");*/
					
					while(pixelCount --> 0){
						uint8 r, g, b;
						r = (*(uint8*)imageData) >> shiftFactor;
						g = (*(uint8*)(imageData+1)) >> shiftFactor;
						b = (*(uint8*)(imageData+2)) >> shiftFactor;
						*bmp16StoreCurrent++ = (alphaBit | (r << 10)
						| (g << 5) | b);
						
						imageData += 3;
						if(--pixelInScanline == 0){ 
							pixelInScanline = width-1;
							
							PNGAdaptiveFilter newFilter = *(PNGAdaptiveFilter*)&imageData;
							
							if(newFilter != pngAdaptiveFilter){
								pngAdaptiveFilter = newFilter;
								iprintf("->New Filter: ");
								iprintf(PNGAdaptiveFilterAsString(pngAdaptiveFilter));
								iprintf("\r\n");
							}
							imageData++;
						}
					}
					
					texture->data = (void*)data;
					texture->dataSize = bmp16StoreSpace;
				}
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

	iprintf("\r\nPNGInfo:\r\n->Color Depth: %u BPP\r\n->Image Size: %ux%u\r\n", (pngInfo->bytesPerChannel*3), pngInfo->imageWidth, pngInfo->imageHeight);
	
	iprintf("->Color Type: ");
	iprintf(PNGColorTypeAsString(pngInfo->pngColorType));
	
	iprintf("\r\n->Compression: ");
	iprintf(PNGCompressionTypeAsString(pngInfo->pngCompressionType));
	
	//if(pngInfo->pngInterlacingType != None){
		iprintf("\r\n->Interlacing Type: ");
		iprintf(PNGInterlacingTypeAsString(pngInfo->pngInterlacingType));
		iprintf("\r\n->Filter Method: ");
		iprintf(PNGFilterMethodAsString(pngInfo->pngFilterMethod));
	//}
	
	iprintf("\r\n->Gamma: ");
	printfixed16r16(pngInfo->gamma);
	iprintf("\r\n\r\n");

	texture->width = pngInfo->imageWidth;
	texture->height = pngInfo->imageHeight;
	//texture->textureType = BMP16;

	iprintf("Image loaded!\r\n");

	return texture;
}