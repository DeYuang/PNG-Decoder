typedef uint8 Subpixel;
enum TextureType :			uint8	 	{ Undefined = 0, BMP16 };
enum BMPHeaderVersion :		uint8		{ Unknown = 0, BITMAPINFOHEADER = 40};
enum BMPCompressionType	:	uint8		{ BI_RGB = 0, BI_RLE8 = 1, BI_RLE4 = 2, BI_BITFIELDS = 3, BI_JPEG = 4,
										   BI_PNG = 5, BI_ALPHABITFIELDS = 6,
										   BI_CMYK = 11, BI_CMYKRLE8 = 12, BI_CMYKRLE4 = 13 };

// BgType_Bmp16:
// Bitmap background with 16 bit color values of the form aBBBBBGGGGGRRRRR
// (if 'a' is set the pixel will be rendered...if not the pixel will be transparent)

class Texture{
	
	public:
		TextureType textureType;
		uint16 width; uint16 height;
		
		void* data;
		uint32 dataSize;
		char* path;
		
		bool isLoaded(){
			if(data != NULL)
				return true;
			return false;
		}
};

static char* BMPHeaderVersionAsString(const BMPHeaderVersion input){

	switch(input){
	
		//case BITMAPCOREHEADER:
		//	return (char*)"BITMAPCOREHEADER";
			
		case BITMAPINFOHEADER:
			return (char*)"BITMAPINFOHEADER";
			
		//case BITMAPV2INFOHEADER:
			//return (char*)"BITMAPV2INFOHEADER";
			
		//case BITMAPV3INFOHEADER:
		//	return (char*)"BITMAPV3INFOHEADER";
			
		//case BITMAPV4HEADER:
		//	return (char*)"BITMAPV4HEADER";
			
		//case BITMAPV5HEADER:
		//	return (char*)"BITMAPV5HEADER";
	
		default:{
			iprintf("Texture.h BMPHeaderVersionAsString() Error: Default error!\r\n");
			return (char*)"";
		}
	}
}

static char* BMPCompressionTypeAsString(const BMPCompressionType input){

	switch(input){
	
		case BI_RGB:
			return (char*)"BI_RGB";
			
		case BI_RLE8:
			return (char*)"BI_RLE8";
			
		case BI_RLE4:
			return (char*)"BI_RLE4";
			
		case BI_BITFIELDS:
			return (char*)"BI_BITFIELDS";
			
		case BI_JPEG:
			return (char*)"BI_JPEG";
			
		case BI_PNG:
			return (char*)"BI_PNG";
			
		case BI_ALPHABITFIELDS:
			return (char*)"BI_ALPHABITFIELDS";
			
		case BI_CMYK:
			return (char*)"BI_CMYK";
			
		case BI_CMYKRLE8:
			return (char*)"BI_CMYKRLE8";
			
		case BI_CMYKRLE4:
			return (char*)"BI_CMYKRLE4";
	
		default:{
			iprintf("Texture.h BMPCompressionTypeAsString() Error: Default error!\r\n");
			return (char*)"";
		}
	}
}

static inline uint32 SwapEndian(const uint32 in){

	// Swap endian (big -> little) or (little -> big)
	/*uint32 b0,b1,b2,b3;

	b0 = in >> 24;
	b1 = (in & 0x0000ff00) << 8;
	b2 = (in & 0x00ff0000) >> 8;
	b3 = in << 24;

	// why does 9 become 156?

	return (b0 | b1 | b2 | b3);*/
	
	/*int32_t tmp = (in << 16) |
                 ((in >> 16) & 0x00ffff);
    return ((tmp >> 8) & 0x00ff00ff) | ((tmp & 0x00ff00ff) << 8);*/
	
	/*swab ((void*)&in, (void*)&in, 4);
	
	return in;*/
	
	return __builtin_bswap32(in);
	
	/*uint32 out;
	{
		uint8 char0 = *(uint8*)((void*)&in);
		uint8 char1 = *(uint8*)((void*)&in+1);
		uint8 char2 = *(uint8*)((void*)&in+2);
		uint8 char3 = *(uint8*)((void*)&in+3);
		
		out = (char0 << 24 | char1 << 16 | char2 << 8 | char3);
	}
	return out;*/
}

static inline uint16 SwapEndian(const uint16 in){

	// Swap endian (big -> little) or (little -> big)
	/*uint16 b0,b1;

	b0 = in << 8;
	b1 = in >> 8;

	return (b0 | b1);*/
	
	/*swab ((void*)&in, (void*)&in, 2);
	
	return in;*/
	
	return __builtin_bswap16(in);
}

static char* BMPHeaderVersionAsString(const BMPHeaderVersion input);
static char* BMPCompressionTypeAsString(const BMPCompressionType input);
static inline BMPHeaderVersion GetBMPHeaderVersion(const uint32 headerLength);

static inline Texture* LoadTexture(const char* fileName);
static inline Texture* DecodePNG(char* fileContents, uint16 fileLength);
static inline Texture* DecodeBMP(char* fileContents);
static inline Texture* DecodeAndScaleBMP(char* fileContents);