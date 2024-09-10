// 3-9-24
// been working on the UI for a while; finally time 
// to hide away as much of this dumpster-fire of a UI 
// codebase as I can

#ifndef BADUI
#define BADUI

#include <nf_lib.h>
#include <filesystem.h>

struct calc_console
{
    char current_line[255];
};

void ui_init();
u8 check_touch(touchPosition* tp);
void calc_main_print(char* print_this, u8* line, char newline);


#endif