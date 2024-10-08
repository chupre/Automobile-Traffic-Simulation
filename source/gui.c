// Standard
#include <string.h>

// Custom modules
#include <gui.h>
#include <render.h>
#include <cars.h>
#include <road.h>
#include "GLFW/glfw3.h"
#include "algorithms.h"
#include "camera.h"
#include "map.h"
#include "traffic_density.h"
#include <rlc.h>
#include <texture.h>
#include <shader.h>
#include <dbg.h>

// External 
#if defined(_WIN32) || defined(WIN32)
    #include <dirent/dirent.h>
#else
    #include <dirent.h>
#endif

#define NK_ASSERT(a)
#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include <nuklear/nuklear.h>
#include <nuklear/nuklear_glfw_gl3.h>
#include <style.c>

char userSaveName[MAX_BUFFER_SIZE];
int activeFileIndex = 0;
int curr_step = 0;

config initConfig = {
    50,
    3,
    100,
    SEVERAL_CROSSES,
};

void initGUI()
{
    if (isSaveMenuActive) {
        showSaveMenu();
        return;
    }

    if (isLoadMenuActive) {
        showLoadMenu();
        return;
    }

    if (isInitMenuActive) {
        showInitMenu();
        return;
    }

    if (isHelpMenuActive) {
        showHelpMenu();
        return;
    }

    if (nk_begin(context, "PauseMenu", nk_rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT), 0))
    {
        nk_layout_row_dynamic(context, 50, 1);
        nk_label(context, "Main Menu", NK_TEXT_CENTERED);

        nk_layout_row_begin(context, NK_STATIC, 30, 2);
        nk_layout_row_push(context, (float)WINDOW_WIDTH / 2 - 85);
        nk_spacer(context);
        nk_layout_row_push(context, 150);

        if (nk_button_label(context, "New Model"))
        {
            isInitMenuActive = true;
        }

        nk_layout_row_end(context);

        nk_layout_row_dynamic(context, 5, 1);

        if (isInit) {
            nk_layout_row_begin(context, NK_STATIC, 30, 2);
            nk_layout_row_push(context, (float)WINDOW_WIDTH / 2 - 85);
            nk_spacer(context);
            nk_layout_row_push(context, 150);

            if (nk_button_label(context, "Save Model"))
            {
                strcpy(userSaveName, "");
                isSaveMenuActive = true;
            }

            nk_layout_row_end(context);

            nk_layout_row_dynamic(context, 5, 1);
        }

        nk_layout_row_begin(context, NK_STATIC, 30, 2);
        nk_layout_row_push(context, (float)WINDOW_WIDTH / 2 - 85);
        nk_spacer(context);
        nk_layout_row_push(context, 150);

        if (nk_button_label(context, "Load Model"))
        {
            isLoadMenuActive = true;
        }

        nk_layout_row_end(context);

        nk_layout_row_dynamic(context, 5, 1);

        nk_layout_row_begin(context, NK_STATIC, 30, 2);
        nk_layout_row_push(context, (float)WINDOW_WIDTH / 2 - 85);
        nk_spacer(context);
        nk_layout_row_push(context, 150);

        if (nk_button_label(context, "Help"))
            isHelpMenuActive = true;
       

        nk_layout_row_end(context);

        nk_layout_row_dynamic(context, 5, 1);

        nk_layout_row_begin(context, NK_STATIC, 30, 2);
        nk_layout_row_push(context, (float)WINDOW_WIDTH / 2 - 85);
        nk_spacer(context);
        nk_layout_row_push(context, 150);

        if (nk_button_label(context, "Quit"))
        {
            exit(0);
        }

        nk_layout_row_end(context);

        nk_layout_row_dynamic(context, 10, 1);

        nk_layout_row_dynamic(context, 30, 1);
        nk_label(context, "Theme", NK_TEXT_CENTERED);

        static int op = THEME_DARK;

        nk_layout_row_begin(context, NK_STATIC, 30, 2);
        nk_layout_row_push(context, (float)WINDOW_WIDTH / 2 - 53);
        nk_spacer(context);
        nk_layout_row_push(context, 150);

        if (nk_option_label(context, "Black", op == THEME_BLACK))
        {
            op = THEME_BLACK;
            set_style(context, op);
        }

        nk_layout_row_end(context);

        nk_layout_row_begin(context, NK_STATIC, 30, 2);
        nk_layout_row_push(context, (float)WINDOW_WIDTH / 2 - 53);
        nk_spacer(context);
        nk_layout_row_push(context, 150);

        if (nk_option_label(context, "Blue", op == THEME_BLUE))
        {
            op = THEME_BLUE;
            set_style(context, op);
        }

        nk_layout_row_end(context);

        nk_layout_row_begin(context, NK_STATIC, 30, 2);
        nk_layout_row_push(context, (float)WINDOW_WIDTH / 2 - 53);
        nk_spacer(context);
        nk_layout_row_push(context, 150);
        
        if (nk_option_label(context, "Dark", op == THEME_DARK))
        {
            op = THEME_DARK;
            set_style(context, op);
        }
        
        nk_layout_row_end(context);

        nk_layout_row_begin(context, NK_STATIC, 30, 2);
        nk_layout_row_push(context, (float)WINDOW_WIDTH / 2 - 53);
        nk_spacer(context);
        nk_layout_row_push(context, 150);
        
        if (nk_option_label(context, "Red", op == THEME_RED))
        {
            op = THEME_RED;
            set_style(context, op);
        }
        
        nk_layout_row_end(context);

        nk_layout_row_begin(context, NK_STATIC, 30, 2);
        nk_layout_row_push(context, (float)WINDOW_WIDTH / 2 - 53);
        nk_spacer(context);
        nk_layout_row_push(context, 150);
        
        if (nk_option_label(context, "White", op == THEME_WHITE))
        {
            op = THEME_WHITE;
            set_style(context, op);
        }
        
        nk_layout_row_end(context);

        nk_layout_row_dynamic(context, 10, 1);

        nk_layout_row_dynamic(context, 30, 1);
        nk_label(context, "Credits", NK_TEXT_CENTERED);

        float parent_ratio[] = {0.285f, 0.43f, 0.285f};
        float child_ratio[] = {0.01f, 0.98f, 0.01f};
        nk_layout_row(context, NK_DYNAMIC, 244, 3, parent_ratio);
        nk_spacer(context);
        if (nk_group_begin(context, "HelpInfo", NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_dynamic(context, 10, 1);
            nk_layout_row(context, NK_DYNAMIC, 20, 3, child_ratio);
            nk_spacer(context);
            nk_label(context, "Подготовили:", NK_TEXT_LEFT);
            nk_spacer(context);
            nk_spacer(context);
            nk_label(context, "  Чуприцкий Денис Владмирович", NK_TEXT_LEFT);
            nk_spacer(context);
            nk_spacer(context);
            nk_label(context, "  Гопта Владимир Дмитриевич", NK_TEXT_LEFT);
            nk_spacer(context);
            nk_spacer(context);
            nk_label(context, "  5151003/30002", NK_TEXT_LEFT);
            nk_spacer(context);
            nk_spacer(context);
            nk_label(context, "", NK_TEXT_LEFT);
            nk_spacer(context);
            nk_spacer(context);
            nk_label(context, "Институт компьютерных наук и кибербезопасности", NK_TEXT_CENTERED);
            nk_spacer(context);
            nk_spacer(context);
            nk_label(context, "Высшая школа кибербезопасности", NK_TEXT_CENTERED);
            nk_spacer(context);
            nk_spacer(context);
            nk_label(context, "СПбПУ им. Петра Великого", NK_TEXT_CENTERED);
            nk_spacer(context);
            nk_spacer(context);
            nk_label(context, "2024", NK_TEXT_CENTERED);
            nk_spacer(context);

            nk_group_end(context);
        }
        nk_spacer(context);

    }

    nk_end(context);
    nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}


void initFont()
{
    struct nk_font_atlas* atlas;
    struct nk_font* jb;
    struct nk_font_config jb_cfg;

    jb_cfg = nk_font_config(24);
    jb_cfg.oversample_h = 1;
    jb_cfg.oversample_v = 1;
    jb_cfg.range = nk_font_cyrillic_glyph_ranges();

    nk_glfw3_font_stash_begin(&glfw, &atlas);
    jb = nk_font_atlas_add_from_file(atlas, "../resources/fonts/JetBrainsMono-Bold.ttf", 24, &jb_cfg);
    nk_glfw3_font_stash_end(&glfw);

    nk_style_set_font(context, &jb->handle);
}


void showSaveMenu()
{
    if (nk_begin(context, "SaveMenu", nk_rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT), 0))
    {
        nk_layout_row_dynamic(context, 50, 1);
        nk_label(context, "Save Model", NK_TEXT_CENTERED);
        
        nk_layout_row_dynamic(context, 25, 1);
        nk_label(context, "Enter save name.", NK_TEXT_CENTERED);

        nk_layout_row_dynamic(context, 10, 1);
        nk_spacer(context);

        nk_layout_row_dynamic(context, 40, 3);
        nk_spacer(context);
        nk_flags event = nk_edit_string_zero_terminated(context, NK_EDIT_BOX | NK_EDIT_AUTO_SELECT, userSaveName, sizeof(userSaveName), nk_filter_ascii);
        nk_spacer(context);

        nk_layout_row_dynamic(context, 10, 1);
        nk_spacer(context);

        nk_layout_row_dynamic(context, 40, 5);
        nk_spacer(context);
        nk_spacer(context);

        if (nk_button_label(context, "Save"))
        {
            save();
        }
    }

    nk_end(context);
    nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}


void showLoadMenu()
{
    DIR* directory;
    struct dirent* entry;
    directory = opendir("../saves");
    
    char saves[MAX_SAVES][MAX_BUFFER_SIZE];
    int save_counter = 0;

    if (directory == NULL)
    {
        printf("Error opening directory\n");
        isLoadMenuActive = false;
        return;
    }

    while ((entry = readdir(directory)) != NULL)
    {
        memcpy(saves[save_counter], entry->d_name, MAX_BUFFER_SIZE);
        save_counter++;
    }

    if (nk_begin(context, "LoadMenu", nk_rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT), 0))
    {
        nk_layout_row_dynamic(context, 50, 1);
        nk_label(context, "Load Model", NK_TEXT_CENTERED);

        nk_layout_row_dynamic(context, 300, 3);
        nk_spacer(context);

        if (nk_group_begin(context, "Save files", NK_WINDOW_BORDER)) 
        {
            if (save_counter == 2)
            {
                nk_layout_row_dynamic(context, 30, 1);
                nk_label(context, "No saves found", NK_TEXT_CENTERED);
            }
            else
            {
                for (int i = 0; i < save_counter; i++)
                {
                    if (strcmp("..", saves[i]) == 0 || strcmp(".", saves[i]) == 0)
                        continue;

                    nk_layout_row_dynamic(context, 30, 1);

                    if (nk_option_label(context, saves[i], i == activeFileIndex))
                    {
                        activeFileIndex = i;
                    }
                }
            }

            nk_group_end(context);
        }

        nk_spacer(context);

        nk_layout_row_dynamic(context, 10, 1);
        nk_spacer(context);

        nk_layout_row_dynamic(context, 35, 5);
        nk_spacer(context);
        nk_spacer(context);

        if (nk_button_label(context, "Load"))
        {
            if (strcmp("..", saves[activeFileIndex]) == 0 || strcmp(".", saves[activeFileIndex]) == 0)
                return;

            load(saves[activeFileIndex]);
        }

        nk_layout_row_dynamic(context, 10, 1);
        nk_spacer(context);

        nk_layout_row_dynamic(context, 35, 5);
        nk_spacer(context);
        nk_spacer(context);

        if (save_counter > 2)
        {
            if (nk_button_label(context, "Delete"))
            {
                char fullname[MAX_BUFFER_SIZE + 10] = "../saves/";
                strcat(fullname, saves[activeFileIndex]);

                remove(fullname);
            }
        }
    }

    nk_end(context);
    nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}


void save()
{
    char fullname[MAX_BUFFER_SIZE + 10] = "../saves/";
    strcat(fullname, userSaveName);
    strcat(fullname, ".bin");

    FILE* saveFile = fopen(fullname, "wb");

    if (saveFile == NULL)
    {
        printf("Error: unable to save the model. Check if you have the save folder\n\n");
        return;
    }

    fwrite(&initConfig, sizeof(initConfig), 1, saveFile);

    RLC* occupiedCells = (RLC *)malloc(sizeof(RLC) * NUMBER_OF_ROADS * (NUMBER_OF_LINES + 1) * NUMBER_OF_CELLS);
    int occupiedCellsCounter = 0;

    for (int i = 0; i < NUMBER_OF_ROADS; i++)
        for (int j = 0; j < NUMBER_OF_LINES + 1; j++)
            for(int k = 0; k < NUMBER_OF_CELLS; k++)
                if (roads[i].lines[j].cells[k] == OCCUPYING_CAR) {
                    occupiedCells[occupiedCellsCounter].road = i;
                    occupiedCells[occupiedCellsCounter].line = j;
                    occupiedCells[occupiedCellsCounter].cell = k;
                        occupiedCellsCounter++;
                }
                else {
                    occupiedCells[occupiedCellsCounter].road = -1;
                    occupiedCells[occupiedCellsCounter].line = -1;
                    occupiedCells[occupiedCellsCounter].cell = -1;
                        occupiedCellsCounter++;
                }


    fwrite(cars, sizeof(car) * MAX_CARS, 1, saveFile);
    fwrite(occupiedCells, sizeof(RLC) * NUMBER_OF_ROADS * (NUMBER_OF_LINES + 1) * NUMBER_OF_CELLS, 1, saveFile);
    fwrite(&freeCars, sizeof(int), 1, saveFile);

    if (MAP_TYPE == CROSS || MAP_TYPE == SEVERAL_CROSSES) {
        for (int i = 0; i < NUMBER_OF_CROSSES; i++) {
            int q_count = crosses[i].carsEndingManeuver.qauntity;
            fwrite(&q_count, sizeof(int), 1, saveFile);

            q_item * item_ptr = crosses[i].carsEndingManeuver.head;
            while (q_count > 0) {
                fwrite(&item_ptr->value->ID, sizeof(int), 1, saveFile);
                item_ptr = item_ptr->next;
                q_count--;
            }

            q_count = crosses[i].carsArriving.qauntity;
            fwrite(&q_count, sizeof(int), 1, saveFile);

            item_ptr = crosses[i].carsArriving.head;
            while (q_count > 0) {
                fwrite(&item_ptr->value->ID, sizeof(int), 1, saveFile);
                item_ptr = item_ptr->next;
                q_count--;
            }

            int * cellsID = (int *)malloc(sizeof(int) * NUMBER_OF_CROSS_CELLS);
            for (int cell = 0; cell < NUMBER_OF_CROSS_CELLS; cell++)
                if (crosses[i].cells[cell]) {
                    if (crosses[i].cells[cell] == OCCUPYING_CAR) {
                        cellsID[cell] = -3;
                        continue;
                    }
                    cellsID[cell] = crosses[i].cells[cell]->ID;
                }
                else {
                    cellsID[cell] = -2;
                }
                //cellsID[cell] = crosses[i].cells[cell] ? crosses[i].cells[cell]->ID : -1;

            fwrite(cellsID, sizeof(int) * NUMBER_OF_CROSS_CELLS, 1, saveFile);
            free(cellsID);
        }

        fwrite(lights, sizeof(traffic_light) * NUMBER_OF_TRAFFIC_LIGHTS, 1, saveFile);
    }

    //fwrite(&densityData, sizeof(int) * NUMBER_OF_ROADS, 1, saveFile);

    free(occupiedCells);
    fclose(saveFile);

    isSaveMenuActive = false;
}


void load(char* fileName)
{
    char fullname[MAX_BUFFER_SIZE + 10] = "../saves/";
    strcat(fullname, fileName);

    FILE* saveFile = fopen(fullname, "rb");

    if (saveFile == NULL)
    {
        printf("Error: failed to open save file while loading\n\n");
        return;
    }
    
    fread(&initConfig, sizeof(initConfig), 1, saveFile);
    init(saveFile);

    isLoadMenuActive = false;
}

void showInitMenu() {
    if (nk_begin(context, "InitMenu", nk_rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT), 0))
    {
        nk_layout_row_dynamic(context, 50, 1);
        nk_label(context, "New Model", NK_TEXT_CENTERED);
        
        nk_layout_row_dynamic(context, 25, 1);
        char cars_label[MAX_BUFFER_SIZE];
        sprintf(cars_label, "Max amount of cars: %d", initConfig.max_cars);
        nk_label(context, cars_label, NK_TEXT_LEFT);
        nk_slider_int(context, 1, &initConfig.max_cars, 1000, 1);

        nk_layout_row_dynamic(context, 25, 1);
        char lines_label[MAX_BUFFER_SIZE];
        sprintf(lines_label, "Number of lines: %d", initConfig.lines);
        nk_label(context, lines_label, NK_TEXT_LEFT);
        nk_slider_int(context, 1, &initConfig.lines, 64, 1);

        nk_layout_row_dynamic(context, 25, 1);
        char spawn_label[MAX_BUFFER_SIZE];
        sprintf(spawn_label, "Cars spawn frequency: %d%%", initConfig.spawn_frequency);
        nk_label(context, spawn_label, NK_TEXT_LEFT);
        nk_slider_int(context, 0, &initConfig.spawn_frequency, 100, 1);

        nk_layout_row_dynamic(context, 25, 1);
        nk_label(context, "Choose map type:", NK_TEXT_LEFT);
        if (nk_option_label(context, "Vertical Road", initConfig.map_type == VERTICAL)) initConfig.map_type = VERTICAL;
        if (nk_option_label(context, "Horizontal Road", initConfig.map_type == HORIZONTAL)) initConfig.map_type = HORIZONTAL;
        if (nk_option_label(context, "Cross", initConfig.map_type == CROSS)) initConfig.map_type = CROSS;
        if (nk_option_label(context, "Multiple Crosses", initConfig.map_type == SEVERAL_CROSSES)) initConfig.map_type = SEVERAL_CROSSES;

        nk_layout_row_dynamic(context, 40, 5);
        nk_spacer(context);
        nk_spacer(context);

        if (nk_button_label(context, "Create a new model"))
        {
            init(NULL);
        }
    }

    nk_end(context);
    nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}

void init (FILE* saveFile) {
    glm_mat3_identity_array(carTransformMatrixes, 1000);

    for (int i = 0; i < 1000; i++)
        glm_translate2d(carTransformMatrixes[i], (vec2){100.0, 100.0});

#ifdef DEBUG
    dbgIsCellsInit = false;
    dbgVerticesInit = false;
#endif
    
    if (isInit) {
        free(densityData);
        for (int i = 0; i < NUMBER_OF_ROADS; i++) {
            for(int j = 0; j < NUMBER_OF_LINES; j++) {
                free(roads[i].lines[j].cells);
            }
            free(roads[i].lines);
        }
        free(lineVertices);
        free(cars);
        free(roadIndices);
        free(roadVertices);
        free(roads);
        free(lights);
        for (int i = 0; i < NUMBER_OF_CROSSES; i++) {
            free(crosses[i].cells);
            q_delete(&crosses[i].carsArriving);
            q_delete(&crosses[i].carsEndingManeuver);
        }
        free(crosses);
        free(checkedCars);
        free(skipCarsFromCross);
        free(carAddingQueue);
        free(userCarsPtrs);
        glDeleteVertexArrays(1, &roadVAO);
        glDeleteVertexArrays(1, &carVAO);
        glDeleteVertexArrays(1, &lineVAO);
        glDeleteVertexArrays(1, &backgroundVAO);
        glDeleteBuffers(1, &roadVBO);
        glDeleteBuffers(1, &carVBO);
        glDeleteBuffers(1, &carInstanceVBO);
        glDeleteBuffers(1, &lineVBO);
        glDeleteBuffers(1, &backgroundVBO);
        glDeleteBuffers(1, &roadEBO);
        glDeleteBuffers(1, &carEBO);
        glDeleteBuffers(1, &backgroundEBO);
    }

    traffic_light_adding_index = -1;
    midLinesCounter = 0;
    densityDataInit = false;
    alghorithmsInit = false;
    cameraInit = false;

    setMap(initConfig.map_type, initConfig.lines, initConfig.max_cars, initConfig.spawn_frequency);

    setCarsToDefault();
    initBackground();
    initRoads();
    initLines();
    initCars();

    step_count = 0;
    
    if (saveFile) {
        RLC * occupiedCells = (RLC *)malloc(sizeof(RLC) * NUMBER_OF_CELLS * (NUMBER_OF_LINES + 1) * NUMBER_OF_ROADS);

        fread(cars, sizeof(car) * MAX_CARS, 1, saveFile);
        fread(occupiedCells, sizeof(RLC) * NUMBER_OF_ROADS * (NUMBER_OF_LINES + 1) * NUMBER_OF_CELLS, 1, saveFile);
        fread(&freeCars, sizeof(int), 1, saveFile);

        for (int i = 0; i < NUMBER_OF_CELLS * (NUMBER_OF_LINES + 1) * NUMBER_OF_ROADS; i++) {
            if (occupiedCells[i].road != -1)
                roads[occupiedCells[i].road].lines[occupiedCells[i].line].cells[occupiedCells[i].cell] = OCCUPYING_CAR;
        }

        for (int i = 0; i < MAX_CARS; i++) {
            if(cars[i].isActive) {
                if (cars[i].nextCell.road != NEXT_CELL_IS_ON_CROSS) {
                    //setBornCar(&cars[i], i, cars[i].currCell);
                    roads[cars[i].currCell.road].lines[cars[i].currCell.line].cells[cars[i].currCell.cell] = &cars[i];
                } else {
                    if (cars[i].crossCurrCell.crossNum != -1)
                        setCarOnCross(cars[i].crossCurrCell, &cars[i], i);
                }
            }
        }

        free(occupiedCells);

        if (MAP_TYPE == CROSS || MAP_TYPE == SEVERAL_CROSSES) {
            for (int i = 0; i < NUMBER_OF_CROSSES; i++) {
                int q_count;
                fread(&q_count, sizeof(int), 1, saveFile);

                while(q_count > 0) {
                    int carID;
                    fread(&carID, sizeof(int), 1, saveFile);
                    q_append(&cars[carID], &crosses[i].carsEndingManeuver);
                    q_count--;
                }

                fread(&q_count, sizeof(int), 1, saveFile);

                while(q_count > 0) {
                    int carID;
                    fread(&carID, sizeof(int), 1, saveFile);
                    q_append(&cars[carID], &crosses[i].carsArriving);
                    q_count--;
                }

                int * cellsID = (int *)malloc(sizeof(int) * NUMBER_OF_CROSS_CELLS);
                fread(cellsID, sizeof(int) * NUMBER_OF_CROSS_CELLS, 1, saveFile);

                for (int cell = 0; cell < NUMBER_OF_CROSS_CELLS; cell++) {
                    if (cellsID[cell] != -2) {
                        if (cellsID[cell] == -3) {
                            crosses[i].cells[cell] = OCCUPYING_CAR;
                            continue;
                        }
                        crosses[i].cells[cell] = &cars[cellsID[cell]];
                    }
                    else
                        crosses[i].cells[cell] = NULL;
                }
                free(cellsID);
            }
            
            traffic_light * newLights = (traffic_light *)malloc(sizeof(traffic_light) * NUMBER_OF_TRAFFIC_LIGHTS);
            fread(newLights, sizeof(traffic_light) * NUMBER_OF_TRAFFIC_LIGHTS, 1, saveFile);
            for (int i = 0; i < NUMBER_OF_TRAFFIC_LIGHTS; i++) {
                lights[i].changeTimer = newLights[i].changeTimer;
                lights[i].color = newLights[i].color;
            }
            free(newLights);
        }

        // free(densityData);
        // densityData = (int* )malloc(sizeof(int) * NUMBER_OF_ROADS);
        // fread(&densityData, sizeof(int) * NUMBER_OF_ROADS, 1, saveFile);

        fclose(saveFile);

        lastTime = glfwGetTime();
        timer = lastTime;
        deltaTime = 0.0f;

        endPauseTime = 0.0f;
    }

    isInitMenuActive = false;
    isInit = true;
    paused = false;
}

void showInfo() {
    int width = 720;
    int height = 320;

    if (nk_begin(context, 
                "Info", 
                nk_rect((float)WINDOW_WIDTH/2 - 110, 
                        (float)WINDOW_HEIGHT/2 - 110, 
                        width, 
                        height),
                NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE))
    {
        char carsInfo[MAX_BUFFER_SIZE];
        char timeInfo[MAX_BUFFER_SIZE];
        char stepInfo[MAX_BUFFER_SIZE];
        char trafficDensityInfo[MAX_BUFFER_SIZE];
        char carsOnLine[MAX_BUFFER_SIZE];

        sprintf(carsInfo, "Total cars: %d", MAX_CARS - freeCars);
        sprintf(timeInfo, "Time: %.2f", glfwGetTime());
        sprintf(stepInfo, "Updates: %d", step_count);

        nk_layout_row_dynamic(context, 20, 1);
        nk_label(context, carsInfo, NK_TEXT_LEFT);
        nk_label(context, timeInfo, NK_TEXT_LEFT);
        nk_label(context, stepInfo, NK_TEXT_LEFT);

        if (isLinePicked) {
            if (step_count && step_count != curr_step) {
                sprintf(trafficDensityInfo, "Traffic Density: %d", getDensityData(pickedRLC.road));
                sprintf(carsOnLine, "Cars on Line: %d", getCarsNumOnLine(pickedRLC.road, pickedRLC.line));
            }

            nk_layout_row_dynamic(context, 20, 1);
            nk_label(context, trafficDensityInfo, NK_TEXT_LEFT);
            nk_label(context, carsOnLine, NK_TEXT_LEFT);
        } else {
            nk_layout_row_dynamic(context, 50, 1);
            nk_label(context, "Pick a line to get more info.", NK_TEXT_LEFT);
        }
    }

    if (nk_window_is_hidden(context, "Info")) {
        isShowInfo = false;
        return;
    }

    if (nk_window_is_hovered(context) && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        nk_window_set_position(context, "Info", nk_vec2(mousePosX - (float)width / 2, mousePosY));

    nk_end(context); 

    nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}

void showHelpMenu() {
    if (nk_begin(context, "HelpMenu", nk_rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT), 0)) {
        nk_layout_row_dynamic(context, 50, 1);
        nk_label(context, "Help", NK_TEXT_CENTERED);

        float parent_ratio[] = {0.15f, 0.7f, 0.15f};
        float child_ratio[] = {0.01f, 0.98f, 0.01f};
        nk_layout_row(context, NK_DYNAMIC, 330, 3, parent_ratio);
        nk_spacer(context);
        if (nk_group_begin(context, "HelpInfo", NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_dynamic(context, 10, 1);
            nk_layout_row(context, NK_DYNAMIC, 20, 3, child_ratio);
            nk_spacer(context);
            nk_label(context, "Use WASD to move camera.", NK_TEXT_LEFT);
            nk_spacer(context);
            nk_spacer(context);
            nk_label(context, "Use mouse wheel to zoom in and out.", NK_TEXT_LEFT);
            nk_spacer(context);
            nk_spacer(context);
            nk_label(context, "Left click on road to set a car.", NK_TEXT_LEFT);
            nk_spacer(context);
            nk_spacer(context);
            nk_label(context, "Right click on road to set a crashed car.", NK_TEXT_LEFT);
            nk_spacer(context);
            nk_spacer(context);
            nk_label(context, "Press ESC to open or close the pause menu.", NK_TEXT_LEFT);
            nk_spacer(context);
            nk_layout_row_dynamic(context, 20, 1);
            nk_layout_row(context, NK_DYNAMIC, 20, 3, child_ratio);
            nk_spacer(context);
            nk_label(context, "Press '`' or '~' to open or close the info window.", NK_TEXT_LEFT);
            nk_spacer(context);
            nk_spacer(context);
            nk_label(context, "When the info window is opened, left click on the line to pick it.", NK_TEXT_LEFT);
            nk_spacer(context);
            nk_spacer(context);
            nk_label(context, "You can move around the info window by clicking on it's top panel.", NK_TEXT_LEFT);
            nk_spacer(context);
            nk_spacer(context);
            nk_layout_row_dynamic(context, 20, 1);
            nk_layout_row(context, NK_DYNAMIC, 20, 3, child_ratio);
            nk_spacer(context);
            nk_label(context, "Press ESC to exit this menu.", NK_TEXT_LEFT);
            nk_spacer(context);

            nk_group_end(context);
        }
        nk_spacer(context);

    }

    nk_end(context);
    nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}
