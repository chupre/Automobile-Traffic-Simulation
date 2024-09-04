#ifndef GUI_H
#define GUI_H

#include <stdio.h>
#define MAX_BUFFER_SIZE 256
#define MAX_SAVES 64

typedef struct config config;

struct config {
    int max_cars;
    int lines;
    int spawn_frequency;
    int map_type;
};

void initGUI();
void initFont();
void showSaveMenu();
void showLoadMenu();
void save();
void load(char* fileName);
void init(FILE* saveFile);

extern char userSaveName[MAX_BUFFER_SIZE];
extern int activeFileIndex;
extern config initConfig;

#endif
