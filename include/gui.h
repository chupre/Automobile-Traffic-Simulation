#ifndef GUI_H
#define GUI_H

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
void init();

extern char userSaveName[MAX_BUFFER_SIZE];
extern int activeFileIndex;
extern config initConfig;

#endif
