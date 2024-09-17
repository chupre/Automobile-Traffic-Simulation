#define NUMBER_OF_TEXTURES 2
#define BACKGROUND_TEXTURE 2
#define CAR_TEXTURE 3

extern unsigned int textures[NUMBER_OF_TEXTURES];
extern int textures_loaded;

void initTextures();
void loadTexture(char * image_name);

