typedef uint8 Subpixel;
enum TextureType :			uint8	 	{ Undefined = 0, BMP16 };

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

// Swap endian (big -> little) or (little -> big)
static inline uint32 SwapEndian(const uint32 in){
	
	return __builtin_bswap32(in);
}

static inline uint16 SwapEndian(const uint16 in){
	
	return __builtin_bswap16(in);
}

static inline Texture* LoadTexture(const char* fileName);
static inline Texture* DecodePNG(char* fileContents, uint16 fileLength);