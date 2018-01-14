#include "fixed.h"
#include "texture.h"
#include "texture_PNG.h"
#include "string.h"
#include "compression.h"

Texture* LoadTexture(const char* path){

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