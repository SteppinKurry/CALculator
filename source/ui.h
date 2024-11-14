// 3-9-24
// been working on the UI for a while; finally time 
// to hide away as much of this dumpster-fire of a UI 
// codebase as I can

#ifndef BADUI
#define BADUI

#include <nf_lib.h>
#include <filesystem.h>

#include "sizes.h"

struct calc_console
{
    char current_line[255];
};

struct ui_screen
{

    u8 button_layout[MAX_UI_BUTTONS][2];
    u8 num_buttons;
    u8 button_size;
    u8 butt_offset;

    char bg_name[MAX_BG_NAME_CHARS];

};

void ui_init();
u8 check_touch(touchPosition* tp, struct ui_screen* ui);
void calc_main_print(char* print_this, u8* line, char newline);
void calc_console_init();
void calc_dummy_print();
void fill_ui_layout(struct ui_screen* ui, u8 layout[][2]);
void set_bottom_ui(struct ui_screen* new_bg);
void nice_fraction_print(u64 num, u64 den, int8 sign, int64 sci, char* expression, char* str);
void annoyed_print(char* str);

void plot_point(u16 x, u16 y);

struct ui_screen ui_init_main_screen();
struct ui_screen ui_init_irr_screen();
struct ui_screen ui_init_func_screen();

#endif