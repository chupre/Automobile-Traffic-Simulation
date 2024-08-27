#include <spawn_logs.h>
#include <time.h>
#include <string.h>

int fileNumber = 0;
char fileName[100] = {'\0'};
FILE* filePtr = NULL;

GLvoid changeFileName()
{
    memset(fileName, '\0', strlen(fileName));
    int index = 0;
    fileName[index] = fileNumber + '0';
    memcpy(&fileName[1], ".txt", 4);
    // fileNumber++;
}

GLvoid openFile()
{
    changeFileName();
    filePtr = fopen(fileName, "w");
}

GLvoid closeFile()
{
    fclose(filePtr);
}

GLvoid logCar(car* Car)
{ 
    fprintf(filePtr, "currCell (%d, %d, %d)\nnextCell (%d, %d, %d)\nvelocity %d\nisActive %d\nisCrushed %d\n\n",
			Car->currCell.road, Car->currCell.line, Car->currCell.cell,
			Car->nextCell.road, Car->nextCell.line, Car->nextCell.cell,
			Car->velocity,
			Car->isActive,
			Car->isCrushed);
}
