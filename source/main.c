// 1-26-24
// Trying to make an "advanced" (TI-84 tier, at best) calculator
// software for DS

// Includes C
#include <stdio.h>
#include <time.h>

#include <nds.h>

#include "ui.h"
#include "parse.h"
#include "calmath.h"

u32 tenth_seconds = 0;
u32 float_precision = 100000;

void penis_print()
{
	tenth_seconds += 1;
}

float32 big_c_divide(int x, int y)
{
	return divf32(inttof32(x), inttof32(y));
}

void print_float(float32 x)
{
	int32 divided = f32toint(x * float_precision);
	iprintf("%ld.%05ld", div32(divided, float_precision), mod32(divided, float_precision));
}

void math_routine()
{

	timerStart(3, ClockDivider_1024, TIMER_FREQ_1024(10), penis_print);
	iprintf("Start!\n");

	// int32 divided = 0;

	// for (int x = 0; x < 1; x++)
	// {
	// 	for (int y = 0; y < 1; y++)
	// 	{
	// 		divided = divf32(420.69, 49.420);
	// 	}
	// }

	// iprintf("result: %ld\nfloated: ", divided);
	// print_float(divided);


	struct bigreal a, b;
	struct bigreal result;
	a.numerator = 95566;
	a.denominator = 741;
	a.sign = 1;

	b.numerator = 2465;
	b.denominator = 36;
	b.sign = -1;

	for (int x = 0; x < 9999; x++)
	{
		result = bigreal_add(a, b);
	}

	iprintf("result is: %d %lld / %lld\n", result.sign, result.numerator, result.denominator);

	iprintf("\nTime: ");
	print_float(divf32(tenth_seconds, 10));
	iprintf("\n");
	
	tenth_seconds = 0;
	timerStop(3);

}

// void clear_array(char** array, int size)
// {
// 	for (int x = 0; x < size-2; x++)
// 	{
// 		array[x] = '0';
// 	}

// 	array[size-2] = "\0";
// }

int main(int argc, char **argv) 
{
	ui_init();

	NF_WriteText(0, 0, 1, 2, "\n==== CALculator ====\n\n");
	NF_WriteText(0, 0, 2, 3, "hey there *winky face*\n\n");

	int keys = keysDown();
	iprintf("press A to do some sick math\n");
	
	touchPosition touch_screen = {0, 0, 0, 0};

	char buffer[32];
	int myvar = 10;
	snprintf(buffer, sizeof(buffer), "Hello world %d times", myvar);

	// char expression[64] = "999999999999999999+";
	// u8 expression_len = 19;

	char expression[64];
	u8 expression_len = 0;

	//clear_array(&expression, 64);

	int current_scroll = 0;
	char tap_toggle = 0;

	struct tokenized mathstring;

	while(1)
   	{	
		scanKeys();
		touchRead(&touch_screen);
		keys = keysDown();
		int held = keysHeld();

		if (!held) { tap_toggle = 1; }

		if ((held & KEY_TOUCH) && tap_toggle == 1)
		{
			tap_toggle = 0;

			iprintf("X: %d Y: %d\n", touch_screen.px, touch_screen.py);

			int button = check_touch(&touch_screen);
			iprintf("button: %d\n\n", button);

			if (button == 14) 
			{ 
				//current_scroll += 5; 
				parse(expression, &mathstring);
				struct bigreal result = basic_do_math(mathstring); 
				if (result.sign == -1) { snprintf(expression, sizeof(expression), "-%lld", result.numerator); }
				else { snprintf(expression, sizeof(expression), "%lld", result.numerator); }
				calc_main_print(expression);

				expression[0] = '\0';
				expression_len = 0;

			}

			else if (button >= 0 && button <= 9)
			{
				expression[expression_len] = button + '0';
				expression_len += 1;
				if (expression_len >= 64) { expression_len = 0; expression[0] = '\0'; }
				expression[expression_len] = '\0';
				calc_main_print(expression);

			}

			else if (button == 10)
			{
				expression[expression_len] = '+';
				expression_len += 1;
				if (expression_len >= 64) { expression_len = 0; expression[0] = '\0'; }
				expression[expression_len] = '\0';
				calc_main_print(expression);

			}

			else if (button == 11)
			{
				expression[expression_len] = '-';
				expression_len += 1;
				if (expression_len >= 64) { expression_len = 0; expression[0] = '\0'; }
				expression[expression_len] = '\0';
				calc_main_print(expression);

			}

			else if (button == 12)
			{
				expression[expression_len] = '*';
				expression_len += 1;
				if (expression_len >= 64) { expression_len = 0; expression[0] = '\0'; }
				expression[expression_len] = '\0';
				calc_main_print(expression);

			}

			else if (button == 13)
			{
				expression[expression_len] = '/';
				expression_len += 1;
				if (expression_len >= 64) { expression_len = 0; expression[0] = '\0'; }
				expression[expression_len] = '\0';
				calc_main_print(expression);

			}


			NF_ScrollBg(0, 0, 0, current_scroll);
			
			
		}

		if (keys & KEY_START) break;
		else if (keys & KEY_A) math_routine();

		NF_UpdateTextLayers();

		// Actualiza el array de OAM
		NF_SpriteOamSet(0);
		NF_SpriteOamSet(1);

		swiWaitForVBlank();

		// Actualiza el OAM
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);

	}

	return 0;
}