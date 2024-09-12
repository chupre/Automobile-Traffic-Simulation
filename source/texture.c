#include <stdio.h>
#include <texture.h>

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>

Texture textures[NUMBER_OF_TEXTURES];
int textures_loaded = 0;
const char texture_path[128] = "../resources/textures/";

void initTextures() {
    struct dirent *dirEntry;

    DIR *dir = opendir(texture_path);

    if (dir == NULL) {
        return;
    }

    while ((dirEntry = readdir(dir)) != NULL)
        if (strstr(dirEntry->d_name, ".png") != NULL) {
            loadTexture(dirEntry->d_name);
            textures_loaded++;
        }

    closedir(dir);
}

void loadTexture(char * image_name) {
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

    glGenTextures(1, &textures[textures_loaded]);
    glBindTexture(GL_TEXTURE_2D, textures[textures_loaded]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, format, texture_width, texture_height, 0, format, GL_UNSIGNED_BYTE, texture_pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Freeing allocated space for image
    stbi_image_free(texture_pixels);
}
