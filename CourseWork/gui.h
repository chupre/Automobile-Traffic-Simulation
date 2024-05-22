#pragma once

void initGUI();
void initFont();

void initGUI()
{
    if (nk_begin(context, "PauseMenu", nk_rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT), 0))
    {
        nk_layout_row_dynamic(context, 80, 1);
        nk_label(context, "Main Menu", NK_TEXT_CENTERED);

        nk_layout_row_begin(context, NK_STATIC, 30, 2);
        nk_layout_row_push(context, WINDOW_WIDTH / 2 - 85);
        nk_spacer(context);
        nk_layout_row_push(context, 150);
        nk_button_label(context, "Save Model");
        nk_layout_row_end(context);

        nk_layout_row_dynamic(context, 10, 1);

        nk_layout_row_begin(context, NK_STATIC, 30, 2);
        nk_layout_row_push(context, WINDOW_WIDTH / 2 - 85);
        nk_spacer(context);
        nk_layout_row_push(context, 150);
        nk_button_label(context, "Load Model");
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
    droid = nk_font_atlas_add_from_file(atlas, "DroidSans.ttf", 24, &droidCfg);
    nk_glfw3_font_stash_end(&glfw);

    nk_style_load_all_cursors(context, atlas->cursors);
    nk_style_set_font(context, &droid->handle);
}