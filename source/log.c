#include <log.h>

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

// log_key _LOG_KEY_ = PLAY;
// log_key _LOG_KEY_ = RECORD;
log_key _LOG_KEY_ = NO;
FILE* f = NULL;

// char fileName[100] = "log.txt";
// char fileName[100] = "situation.txt";
char fileName[100] = "a.txt";
GLint fileSize;

GLvoid openFile(){
    if (_LOG_KEY_ == NO){
        return;
    }
    if (_LOG_KEY_ == PLAY){
        f = fopen(fileName, "r");
        fseek(f, 0, SEEK_END);
        fileSize = ftell(f);
        rewind(f);
    }else if (_LOG_KEY_ == RECORD){
        f = fopen(fileName, "w");
    }
    if (f == NULL){
        printf("opening %s failed\n", fileName);
        exit(-1);
    }
}

GLvoid closeFile(){
    if (_LOG_KEY_ == NO)
        return;
    fclose(f);
}

bool readFile(log_data* data){
    char buf[20] = {'\0'};
    if (ftell(f) == fileSize){
        printf("ftell(f) == fileSize\n");
        return false;
    }
    fgets(buf, 20, f); 
    if (buf[strlen(buf) - 1] == '\n'){
        buf[strlen(buf) - 1] = '\0';
    }
    if (strncmp(buf, "=", 1) == 0){
        return false;
    }
    char *istr = strtok(buf, SEP);
    data->road = atoi(istr);
    istr = strtok(NULL, SEP);
    data->line = atoi(istr);
    istr = strtok(NULL, SEP);
    data->cell = atoi(istr);
    istr = strtok(NULL, SEP);
    data->velocity = atoi(istr);
    return true;
}

GLvoid writeFile(log_data* data){
    fprintf(f, "%d %d %d %d\n", data->road, data->line, data->cell, data->velocity);
}

GLvoid printFileEmptyStep(){
    fprintf(f, "\n");
}

GLvoid printFileStepEnd(){
    fprintf(f, "=\n");
    // printf("fileSize: %ld\n", ftell(f));
}
