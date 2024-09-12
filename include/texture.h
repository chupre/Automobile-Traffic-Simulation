#define NUMBER_OF_TEXTURES 1

typedef unsigned int Texture;

extern Texture textures[NUMBER_OF_TEXTURES];

void initTextures();
void loadTexture(char * image_name);

