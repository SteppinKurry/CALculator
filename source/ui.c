// 3-9-24

#include "ui.h"
#include <stdio.h>



struct ui_screen ui_init_main_screen()
{
	// 10-11-24
	// returns a ui_screen with everything setup for the 
	// main button display

	struct ui_screen ui;

	ui.num_buttons = 19;
	ui.button_size = 40;
	ui.butt_offset = 0;
	sprintf(ui.bg_name, "%s", "./bg/buttons-main");
	NF_LoadTiledBg(ui.bg_name, ui.bg_name, 256, 256);

	// the x and y coordinates of the top left corner of each button on the bottom screen
	u8 button_layout[][2] = {
		
		{5, 149}, {5, 102}, {53, 102}, {100, 102}, {5, 53}, {53, 53}, {102, 53},
		{5, 5}, {53, 5}, {102, 5}, {211, 3}, {211, 51}, {211, 99}, {211, 147}, {102, 149}, 
		{147, 3}, {147, 51}, {147, 99}, {147, 147}};

	
	fill_ui_layout(&ui, button_layout);

	// indices 0-9 are buttons 0-9, followed by plus, minus, multiply, divide, and equal
	// now followed up by left and right parentheses, clear, and exponent 

	return ui;
}

struct ui_screen ui_init_irr_screen()
{
	// returns a ui_screen with everything setup for the 
	// irrational button screen
	struct ui_screen ui;

	ui.num_buttons = 10;
	ui.button_size = 40;
	ui.butt_offset = 19;
	sprintf(ui.bg_name, "%s", "./bg/buttons-irrational");
	NF_LoadTiledBg(ui.bg_name, ui.bg_name, 256, 256);

	// the x and y coordinates of the top left corner of each button on the bottom screen
	u8 button_layout[][2] = 
	{

		{118, 2},  {166, 2},  {206, 2},
		{118, 50}, {166, 50}, {206, 50},
		{118, 98}, {166, 98}, {206, 98},
							  {206, 146}

	};

	fill_ui_layout(&ui, button_layout);

	// indices 0-9 are buttons 0-9, followed by plus, minus, multiply, divide, and equal
	// now followed up by left and right parentheses, clear, and exponent 

	return ui;
}

struct ui_screen ui_init_func_screen()
{
	// returns a ui_screen with everything setup for the 
	// irrational button screen
	struct ui_screen ui;

	ui.num_buttons = 7;
	ui.button_size = 40;
	ui.butt_offset = 45;
	sprintf(ui.bg_name, "%s", "./bg/buttons-functions");
	NF_LoadTiledBg(ui.bg_name, ui.bg_name, 256, 256);

	// the x and y coordinates of the top left corner of each button on the bottom screen
	u8 button_layout[][2] = 
	{

		{2, 51},  {51, 51},		{210, 2},
		{2, 98},  {51, 98},
		{2, 145}, {42, 145}

	};

	fill_ui_layout(&ui, button_layout);
	return ui;
}

void fill_ui_layout(struct ui_screen* ui, u8 layout[][2])
{
	for (int x = 0; x < ui->num_buttons; x++)
	{
		for (int y = 0; y < 2; y++) { ui->button_layout[x][y] = layout[x][y]; }
	}
}

void ui_init()
{
    // sets up all of the modes and textures and backgrounds and 
    // everything else that this old-ass console needs to render 
    // things correctly

	// initialize 2d modes for screens
	NF_Set2D(0, 0);  // screen 0, mode 0
	NF_Set2D(1, 0);  // screen 1, mode 0
	swiWaitForVBlank();

	// set the root folder for the filesystem
	nitroFSInit(NULL);
	NF_SetRootFolder("NITROFS");

	NF_Set3D(0, 0);
	NF_Set2D(1, 0);

    // initialize background tiling for both screens
	NF_InitTiledBgBuffers();
	NF_InitTiledBgSys(0);  // starts the bg tile engine on screen 0
	NF_InitTiledBgSys(1);  // starts the bg tile engine on screen 1

	calc_console_init();

}

void calc_console_init()
{

	u8 text_layer = 1;
	u8 bg_layer = 2;

	// initialize sprite stuff
	NF_InitSpriteBuffers();
	NF_Init3dSpriteSys();

	// load graph_point sprite stuff
	NF_LoadSpriteGfx("./sprite/graph_point", 39, 8, 8);
	NF_LoadSpriteGfx("./sprite/graph_axes", 41, 32, 32);
	NF_LoadSpritePal("./sprite/graph_point", 40);

	NF_Vram3dSpriteGfx(39, 39, true);
	NF_Vram3dSpriteGfx(41, 41, true);
	NF_Vram3dSpritePal(40, 4);

	// generate the sprites used for graphing
	for (int x = 0; x < 235; x++)
	{
		NF_Create3dSprite(x, 39, 4, x, 0);
		NF_Show3dSprite(x, false);
	}

	// pretty sure this just loads the files into memory to use as backgrounds
	NF_LoadTiledBg("bg/buttons-main", "bottom_screen", 256, 256);   
	NF_LoadTiledBg("bg/top_console", "top_screen", 256, 256);

	// creates a background on screen based upon whatever is in ram
	NF_CreateTiledBg(1, 2, "bottom_screen");  // screen 1, layer 2, name (was set when loading into memory)
	NF_CreateTiledBg(0, bg_layer, "top_screen");

	// initialize text for the top screen
	NF_InitTextSys(0);
	NF_LoadTextFont("fnt/default", "wang", 256, 256, 0);
	NF_CreateTextLayer(0, text_layer, 0, "wang");
	NF_DefineTextColor(0, text_layer, 2, 0, 0, 0);	// what
	NF_SetTextColor(0, text_layer, 2);


}


void set_bottom_ui(struct ui_screen* ui)
{
	//NF_LoadTiledBg(ui->bg_name, ui->bg_name, 256, 256);
	NF_CreateTiledBg(1, 1, ui->bg_name);
	NF_ShowBg(1, 2);

	return;
}

void calc_main_print(char* print_this, u8* line, char newline)
{
    //NF_ClearTextLayer(0, 0);
    NF_WriteText(0, 1, 2, *line, print_this);

	if (newline == 1) 
	{ 	
		if (*line >= 255) { line = 0; }
		else { *line += 1; }
	}
}

void plot_point(u16 x, u16 y)
{

	// assign the sprite id to the x position of the point
	NF_Move3dSprite(x, x+10, y+10);
	NF_Show3dSprite(x, true);
	//NF_Create3dSprite(x, 39, 4, x+10, y+10);

}


void annoyed_print(char* str)
{
	u8 peen = 10;
	calc_main_print(str, &peen, 0);
}

void nice_fraction_print(u64 num, u64 den, int8 sign, int64 sci, char* expression, char* str)
{
	char temp[MAX_EXP_CHARS];
	str[0] = '\0';

	// original expression
	sprintf(str, "%s = ", expression);

	// sign
	if (sign == -1) { strcat(str, "-"); }

	// numerator
	sprintf(temp, "%llu", num);
	strcat(str, temp);

	// denominator
	if (den != 1)
	{
		sprintf(temp, "/%llu", den);
		strcat(str, temp);
	}

	// scientific notation
	if (sci != 0)
	{
		sprintf(temp, "*10^(%lld)", sci);
		strcat(str, temp);
	}
	

	return;

}

void calc_dummy_print()
{
	u8 ween = 10;

	calc_main_print("You're a dummy!", &ween, 1);
}

u8 check_touch(touchPosition* tp, struct ui_screen* ui)
{
    // 3-9-24
	// determines whether or not coordinates on the screen are 
	// inside a button, and if they are, return the id of that 
	// button

	// size of each side of a button in pixels (used for hitbox size)
	u8 button_size = ui->button_size;

    // take coordinates from tp pointer for clarity
	u8 x = tp->px;
	u8 y = tp->py;

    // loop through each button and check whether or not the given 
    // coordinates land inside one
	for (int i = 0; i < ui->num_buttons; i++)
	{
		u8 bx = ui->button_layout[i][0];
		u8 by = ui->button_layout[i][1];

		char x_collision = (bx <= x) && (x <= bx + button_size);
		char y_collision = (by <= y) && (y <= by + button_size);

		if (x_collision && y_collision) { return i + ui->butt_offset; }
	}

	return 255; // this means nothing was found
}
