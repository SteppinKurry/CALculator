// 3-9-24

#include "ui.h"

// the x and y coordinates of the top left corner of each button on the bottom screen
u8 button_positions[][2] = {{5, 149}, {5, 102}, {53, 102}, {100, 102}, {5, 53}, {53, 53}, {102, 53},
							{5, 5}, {53, 5}, {102, 5}, {211, 3}, {211, 51}, {211, 99}, {211, 147}, {102, 149}, 
							{147, 3}, {147, 51}, {147, 99}, {147, 147}};

// indices 0-9 are buttons 0-9, followed by plus, minus, multiply, divide, and equal
// now followed up by left and right parentheses, clear, and exponent 
u8 total_nums = 19;

void ui_init()
{
    // sets up all of the modes and textures and backgrounds and 
    // everything else that this old-ass console needs to render 
    // things correctly

	// initialize 2d modes for screens
	NF_Set2D(0, 0);  // screen 0, mode 0
	NF_Set2D(1, 0);  // screen 1, mode 0

    // initialize background tiling for both screens
	NF_InitTiledBgBuffers();
	NF_InitTiledBgSys(0);  // starts the bg tile engine on screen 0
	NF_InitTiledBgSys(1);  // starts the bg tile engine on screen 1

	// set the root folder for the filesystem
	nitroFSInit(NULL);
	NF_SetRootFolder("NITROFS");

    // initialize text for the top screen
	NF_InitTextSys(0);
	NF_LoadTextFont("fnt/default", "wang", 256, 256, 0);
	NF_CreateTextLayer(0, 0, 0, "wang");
	NF_SetTextColor(0, 0, 3);

	// pretty sure this just loads the files into memory to use as backgrounds
	NF_LoadTiledBg("bg/buttons", "nfl", 256, 256);   
	NF_LoadTiledBg("bg/top_console", "capa_3", 256, 256);

	// creates a background on screen based upon whatever is in ram
	NF_CreateTiledBg(1, 2, "nfl");  // screen 0, layer 3, name (was set when loading into memory)
	NF_CreateTiledBg(0, 2, "capa_3");

    NF_CreateTextLayer(0, 0, 0, "wang");

}

void calc_main_print(char* print_this, u8* line, char newline)
{
    //NF_ClearTextLayer(0, 0);
    NF_WriteText(0, 0, 2, *line, print_this);

	if (newline == 1) 
	{ 	
		if (*line >= 255) { line = 0; }
		else { *line += 1; }
	}
}

u8 check_touch(touchPosition* tp)
{
    // 3-9-24
	// determines whether or not coordinates on the screen are 
	// inside a button, and if they are, return the id of that 
	// button

	// size of each side of a button in pixels (used for hitbox size)
	u8 button_size = 40;

    // take coordinates from tp pointer for clarity
	u8 x = tp->px;
	u8 y = tp->py;

    // loop through each button and check whether or not the given 
    // coordinates land inside one
	for (int i = 0; i < total_nums; i++)
	{
		u8 bx = button_positions[i][0];
		u8 by = button_positions[i][1];

		char x_collision = (bx <= x) && (x <= bx + button_size);
		char y_collision = (by <= y) && (y <= by + button_size);

		if (x_collision && y_collision) { return i; }
	}

	return 255; // this means nothing was found
}
