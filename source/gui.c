// Standard
#include <string.h>

// Custom modules
#include <gui.h>
#include <render.h>
#include <cars.h>
#include <road.h>
#include "algorithms.h"
#include "camera.h"
#include "map.h"
#include "traffic_density.h"

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

config initConfig = {
    100,
    3,
    65,
    ONE_ROAD_N
};

void initGUI()
{
    if (isSaveMenuActive)
    {
        showSaveMenu();
        return;
    }

    if (isLoadMenuActive)
    {
        showLoadMenu();
        return;
    }

    if (isInitMenuActive) {
        showInitMenu();
        return;
    }

    if (nk_begin(context, "PauseMenu", nk_rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT), 0))
    {
        nk_layout_row_dynamic(context, 50, 1);
        nk_label(context, "Main Menu", NK_TEXT_CENTERED);

        nk_layout_row_begin(context, NK_STATIC, 30, 2);
        nk_layout_row_push(context, WINDOW_WIDTH / 2 - 85);
        nk_spacer(context);
        nk_layout_row_push(context, 150);

        if (nk_button_label(context, "New Model"))
        {
            isInitMenuActive = true;
        }

        nk_layout_row_end(context);

        nk_layout_row_dynamic(context, 10, 1);

        if (isInit) {
            nk_layout_row_begin(context, NK_STATIC, 30, 2);
            nk_layout_row_push(context, WINDOW_WIDTH / 2 - 85);
            nk_spacer(context);
            nk_layout_row_push(context, 150);

            if (nk_button_label(context, "Save Model"))
            {
                strcpy(userSaveName, "");
                isSaveMenuActive = true;
            }

            nk_layout_row_end(context);

            nk_layout_row_dynamic(context, 10, 1);
        }

        nk_layout_row_begin(context, NK_STATIC, 30, 2);
        nk_layout_row_push(context, WINDOW_WIDTH / 2 - 85);
        nk_spacer(context);
        nk_layout_row_push(context, 150);

        if (nk_button_label(context, "Load Model"))
        {
            isLoadMenuActive = true;
        }

        nk_layout_row_end(context);

        nk_layout_row_dynamic(context, 10, 1);

        nk_layout_row_begin(context, NK_STATIC, 30, 2);
        nk_layout_row_push(context, WINDOW_WIDTH / 2 - 85);
        nk_spacer(context);
        nk_layout_row_push(context, 150);

        if (nk_button_label(context, "Quit"))
        {
            exit(0);
        }

        nk_layout_row_end(context);

        nk_layout_row_dynamic(context, 15, 1);

        nk_layout_row_dynamic(context, 30, 1);
        nk_label(context, "Theme", NK_TEXT_CENTERED);

        static int op = THEME_BLACK;

        nk_layout_row_begin(context, NK_STATIC, 30, 2);
        nk_layout_row_push(context, WINDOW_WIDTH / 2 - 53);
        nk_spacer(context);
        nk_layout_row_push(context, 150);

        if (nk_option_label(context, "Black", op == THEME_BLACK))
        {
            op = THEME_BLACK;
            set_style(context, op);
        }

        nk_layout_row_end(context);

        nk_layout_row_begin(context, NK_STATIC, 30, 2);
        nk_layout_row_push(context, WINDOW_WIDTH / 2 - 53);
        nk_spacer(context);
        nk_layout_row_push(context, 150);

        if (nk_option_label(context, "Blue", op == THEME_BLUE))
        {
            op = THEME_BLUE;
            set_style(context, op);
        }

        nk_layout_row_end(context);

        nk_layout_row_begin(context, NK_STATIC, 30, 2);
        nk_layout_row_push(context, WINDOW_WIDTH / 2 - 53);
        nk_spacer(context);
        nk_layout_row_push(context, 150);
        
        if (nk_option_label(context, "Dark", op == THEME_DARK))
        {
            op = THEME_DARK;
            set_style(context, op);
        }
        
        nk_layout_row_end(context);

        nk_layout_row_begin(context, NK_STATIC, 30, 2);
        nk_layout_row_push(context, WINDOW_WIDTH / 2 - 53);
        nk_spacer(context);
        nk_layout_row_push(context, 150);
        
        if (nk_option_label(context, "Red", op == THEME_RED))
        {
            op = THEME_RED;
            set_style(context, op);
        }
        
        nk_layout_row_end(context);

        nk_layout_row_begin(context, NK_STATIC, 30, 2);
        nk_layout_row_push(context, WINDOW_WIDTH / 2 - 53);
        nk_spacer(context);
        nk_layout_row_push(context, 150);
        
        if (nk_option_label(context, "White", op == THEME_WHITE))
        {
            op = THEME_WHITE;
            set_style(context, op);
        }
        
        nk_layout_row_end(context);

    }

    nk_end(context);
    nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}


void initFont()
{
    struct nk_font_atlas* atlas;
    struct nk_font* droid;
    struct nk_font_config droidCfg;

    droidCfg = nk_font_config(0);

    nk_glfw3_font_stash_begin(&glfw, &atlas);
    droid = nk_font_atlas_add_from_file(atlas, "../resources/fonts/DroidSans.ttf", 24, &droidCfg);
    nk_glfw3_font_stash_end(&glfw);

    nk_style_load_all_cursors(context, atlas->cursors);
    nk_style_set_font(context, &droid->handle);
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
        printf("Error: unable to save the model\n\n");
        return;
    }

    fwrite(&initConfig, sizeof(initConfig), 1, saveFile);
    fwrite(cars, sizeof(car) * MAX_CARS, 1, saveFile);
    fwrite(carTransformMatrixes, sizeof(mat3) * 1000, 1, saveFile);
    fwrite(roads, sizeof(road) * NUMBER_OF_ROADS, 1, saveFile);
    fwrite(&freeCars, sizeof(freeCars), 1, saveFile);

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
    
    setCarsToDefault();
    setRoadsToDefault();
    glm_mat3_identity_array(carTransformMatrixes, 1000);

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
        nk_slider_int(context, 0, &initConfig.max_cars, 1000, 10);

        nk_layout_row_dynamic(context, 25, 1);
        char lines_label[MAX_BUFFER_SIZE];
        sprintf(lines_label, "Number of lines: %d", initConfig.lines);
        nk_label(context, lines_label, NK_TEXT_LEFT);
        nk_slider_int(context, 0, &initConfig.lines, 64, 1);

        nk_layout_row_dynamic(context, 25, 1);
        char spawn_label[MAX_BUFFER_SIZE];
        sprintf(spawn_label, "Cars spawn frequency: %d%%", initConfig.spawn_frequency);
        nk_label(context, spawn_label, NK_TEXT_LEFT);
        nk_slider_int(context, 0, &initConfig.spawn_frequency, 100, 1);

        nk_layout_row_dynamic(context, 25, 1);
        nk_label(context, "Choose map type:", NK_TEXT_LEFT);
        if (nk_option_label(context, "N", initConfig.map_type == ONE_ROAD_N)) initConfig.map_type = ONE_ROAD_N;
        if (nk_option_label(context, "S", initConfig.map_type == ONE_ROAD_S)) initConfig.map_type = ONE_ROAD_S;
        if (nk_option_label(context, "W", initConfig.map_type == ONE_ROAD_W)) initConfig.map_type = ONE_ROAD_W;
        if (nk_option_label(context, "E", initConfig.map_type == ONE_ROAD_E)) initConfig.map_type = ONE_ROAD_E;
        if (nk_option_label(context, "NS", initConfig.map_type == TWO_ROADS_NS)) initConfig.map_type = TWO_ROADS_NS;
        if (nk_option_label(context, "WE", initConfig.map_type == TWO_ROADS_WE)) initConfig.map_type = TWO_ROADS_WE;
        if (nk_option_label(context, "CROSS", initConfig.map_type == CROSS)) initConfig.map_type = CROSS;

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
    if (isInit) {
        glm_mat3_identity_array(carTransformMatrixes, 1000);
        for (int i = 0; i < 1000; i++) {
            glm_translate2d(carTransformMatrixes[i], (vec2){100.0, 100.0});
        }
    } 

#ifdef DEBUG
    dbgIsCellsInit = false;
    dbgVerticesInit = false;
#endif

    setMap(initConfig.map_type, initConfig.lines, initConfig.max_cars, initConfig.spawn_frequency);

    densityDataInit = false;
    alghorithmsInit = false;
    cameraInit = false;

    setCarsToDefault();
    initRoads();
    initLines();
    initCars();

    if (saveFile) {
        fread(cars, sizeof(car) * MAX_CARS, 1, saveFile);
        fread(carTransformMatrixes, sizeof(mat3) * MAX_CARS, 1, saveFile);
        fread(&freeCars, sizeof(freeCars), 1, saveFile);

        fclose(saveFile);
    }

    isInitMenuActive = false;
    isInit = true;
    paused = false;
}
