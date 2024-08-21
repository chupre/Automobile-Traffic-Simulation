#ifndef GUI_H
#define GUI_H

#define MAX_BUFFER_SIZE 256
#define MAX_SAVES 64

void initGUI();
void initFont();
void showSaveMenu();
void showLoadMenu();
void save();
void load(char* fileName);

extern char userSaveName[MAX_BUFFER_SIZE];
extern int activeFileIndex;

#endif