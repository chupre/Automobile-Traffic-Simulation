#ifndef LOG_H
#define LOG_H

// External
#include <glad/glad.h>
#include <cglm/cglm.h>

#include <stdbool.h>

#define SEP " "

typedef struct log_data  log_data;
struct log_data{
    GLint road, line, cell, velocity;
};

typedef enum log_key log_key;
enum log_key{
    NO, PLAY, RECORD
};

extern FILE* f;
extern log_key _LOG_KEY_;
extern char fileName[100];
extern GLint fileSize;

bool readFile(log_data* data);
GLvoid openFile();
GLvoid closeFile();
GLvoid writeFile(log_data* data);
GLvoid printFileEmptyStep();
GLvoid printFileStepEnd();

#endif