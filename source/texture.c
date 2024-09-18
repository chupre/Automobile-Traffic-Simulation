#include <texture.h>

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>

const char texture_path[128] = "../resources/textures/";
unsigned int carTexture, backgroundTexture;

void initTextures() {
    loadTexture("car.png", &carTexture);
    loadTexture("back.png", &backgroundTexture);
}

void loadTexture(char * image_name, unsigned int * texture) {
    stbi_set_flip_vertically_on_load(true);

    unsigned char * texture_pixels;
    int texture_width, texture_height, texture_channels;

    char full_path[128];
    strcpy(full_path, texture_path);
    strcat(full_path, image_name);

    texture_pixels = stbi_load(full_path, 
                               &texture_width, 
                               &texture_height, 
                               &texture_channels,
                               0);
    GLenum format;
    if (texture_channels == 1)
        format = GL_RED;
    else if (texture_channels == 3)
        format = GL_RGB;
    else if (texture_channels == 4)
        format = GL_RGBA;

    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, texture_width, texture_height, 0, format, GL_UNSIGNED_BYTE, texture_pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Freeing allocated space for image
    stbi_image_free(texture_pixels);
}
