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
#include "sizes.h"

u32 tenth_seconds = 0;
u32 float_precision = 100000;


int main(int argc, char **argv) 
{
	// UI related things
	ui_init();

	NF_WriteText(0, 0, 1, 2, "\n==== CALculator ====\n\n");
	NF_WriteText(0, 0, 2, 3, "hey there *winky face*\n\n");

	// Legacy
	int keys = keysDown();
	iprintf("press A to do some sick math\n");
	
	// initialize touch screen, the expression string, 
	// and a few otehr variables
	touchPosition touch_screen = {0, 0, 0, 0};

	char expression[64];
	u8 expression_len = 0;

	int current_scroll = 0;
	char tap_toggle = 0;

	//struct tokenized mathstring;

	char mathstring[MATHSTR_LEN][MAX_NUM_LEN];
	char parsedstring[MATHSTR_LEN][MAX_NUM_LEN];

	u8 whereprint = 15;
	long result = 0;


	// main loop
	while(1)
   	{	
		// get input from buttons and touch screen
		scanKeys();
		touchRead(&touch_screen);
		keys = keysDown();
		int held = keysHeld();

		if (!held) { tap_toggle = 1; }

		// if there was a touch input
		if ((held & KEY_TOUCH) && tap_toggle == 1)
		{
			tap_toggle = 0;

			iprintf("X: %d Y: %d\n", touch_screen.px, touch_screen.py);

			// figure out which virtual button was touched
			int button = check_touch(&touch_screen);
			iprintf("button: %d\n\n", button);

			// do the thing with the button
			if (button == 14) // equal sign
			{ 

				//char bexpression[] = "2^4^3";
				
				result = 0;

				memset(mathstring, '\0', sizeof(mathstring));
				memset(parsedstring, '\0', sizeof(parsedstring));

				tokenize(expression, mathstring);
				parse(mathstring, parsedstring);
				result = evaluate_parsedstring(parsedstring);

				char weenres[200];
				whereprint = 16;
				//NF_ClearTextLayer(0, 0);
				//calc_main_print("behold", &whereprint, 1);
				
				sprintf(weenres, "parsed size: %s", mathstring[0]);

				//calc_main_print(weenres, &whereprint, 1);

				// for (int x = 1; x <= atoi(mathstring[0]); x++)
				// {
				// 	sprintf(weenres, ": %s", mathstring[x]);
				// 	calc_main_print(weenres, &whereprint, 1);
				// }

				// for (int x = 1; x < atoi(parsedstring[0]); x++)
				// {
				// 	sprintf(weenres, ": %s", parsedstring[x]);
				// 	calc_main_print(weenres, &whereprint, 1);
				// }

				whereprint = 20;

				calc_main_print("                                                ", &whereprint, 0);
				calc_main_print("BEHOLD: ", &whereprint, 1);
				sprintf(weenres, "%s = %ld", expression, result);
				calc_main_print(weenres, &whereprint, 0);

				expression[0] = '\0';
				expression_len = 0;

				whereprint = 15;

				calc_main_print("                     ", &whereprint, 0);
			}

			else if (button >= 0 && button <= 9)
			{
				expression[expression_len] = button + '0';
				expression_len += 1;
				if (expression_len >= 64) { expression_len = 0; expression[0] = '\0'; }
				expression[expression_len] = '\0';
				calc_main_print(expression, &whereprint, 0);

			}

			else if (button == 10)
			{
				expression[expression_len] = '+';
				expression_len += 1;
				if (expression_len >= 64) { expression_len = 0; expression[0] = '\0'; }
				expression[expression_len] = '\0';
				calc_main_print(expression, &whereprint, 0);

			}

			else if (button == 11)
			{
				expression[expression_len] = '-';
				expression_len += 1;
				if (expression_len >= 64) { expression_len = 0; expression[0] = '\0'; }
				expression[expression_len] = '\0';
				calc_main_print(expression, &whereprint, 0);

			}

			else if (button == 12)
			{
				expression[expression_len] = '*';
				expression_len += 1;
				if (expression_len >= 64) { expression_len = 0; expression[0] = '\0'; }
				expression[expression_len] = '\0';
				calc_main_print(expression, &whereprint, 0);

			}

			else if (button == 13)
			{
				expression[expression_len] = '/';
				expression_len += 1;
				if (expression_len >= 64) { expression_len = 0; expression[0] = '\0'; }
				expression[expression_len] = '\0';
				calc_main_print(expression, &whereprint, 0);

			}

			else if (button == 15) // clear button
			{
				memset(mathstring, '\0', sizeof(mathstring));
				memset(parsedstring, '\0', sizeof(parsedstring));

				NF_ClearTextLayer(0, 0);

				expression[0] = '\0';
				expression_len = 0;


			}
			
			else if (button == 16) // (
			{
				expression[expression_len] = '(';
				expression_len += 1;
				if (expression_len >= 64) { expression_len = 0; expression[0] = '\0'; }
				expression[expression_len] = '\0';
				calc_main_print(expression, &whereprint, 0);

			}

			else if (button == 17) // )
			{
				expression[expression_len] = ')';
				expression_len += 1;
				if (expression_len >= 64) { expression_len = 0; expression[0] = '\0'; }
				expression[expression_len] = '\0';
				calc_main_print(expression, &whereprint, 0);

			}

			else if (button == 18) // ^
			{
				expression[expression_len] = '^';
				expression_len += 1;
				if (expression_len >= 64) { expression_len = 0; expression[0] = '\0'; }
				expression[expression_len] = '\0';
				calc_main_print(expression, &whereprint, 0);

			}




			NF_ScrollBg(0, 0, 0, current_scroll);
			
			
		}


		if (keys & KEY_START) break;
		// else if (keys & KEY_A) math_routine();

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


				// struct bigreal result = basic_do_math(mathstring); 
				// if (result.sign == -1) { snprintf(expression, sizeof(expression), "-%lld", result.numerator); }
				// else { snprintf(expression, sizeof(expression), "%lld", result.numerator); }



// void penis_print()
// {
// 	tenth_seconds += 1;
// }

// float32 big_c_divide(int x, int y)
// {
// 	return divf32(inttof32(x), inttof32(y));
// }

// void print_float(float32 x)
// {
// 	int32 divided = f32toint(x * float_precision);
// 	iprintf("%ld.%05ld", div32(divided, float_precision), mod32(divided, float_precision));
// }


// void math_routine()
// {

// 	timerStart(3, ClockDivider_1024, TIMER_FREQ_1024(10), penis_print);
// 	iprintf("Start!\n");

// 	// int32 divided = 0;

// 	// for (int x = 0; x < 1; x++)
// 	// {
// 	// 	for (int y = 0; y < 1; y++)
// 	// 	{
// 	// 		divided = divf32(420.69, 49.420);
// 	// 	}
// 	// }

// 	// iprintf("result: %ld\nfloated: ", divided);
// 	// print_float(divided);


// 	struct bigreal a, b;
// 	struct bigreal result;
// 	a.numerator = 95566;
// 	a.denominator = 741;
// 	a.sign = 1;

// 	b.numerator = 2465;
// 	b.denominator = 36;
// 	b.sign = -1;

// 	for (int x = 0; x < 9999; x++)
// 	{
// 		result = bigreal_add(a, b);
// 	}

// 	iprintf("result is: %d %lld / %lld\n", result.sign, result.numerator, result.denominator);

// 	iprintf("\nTime: ");
// 	print_float(divf32(tenth_seconds, 10));
// 	iprintf("\n");
	
// 	tenth_seconds = 0;
// 	timerStop(3);

// }

// void clear_array(char** array, int size)
// {
// 	for (int x = 0; x < size-2; x++)
// 	{
// 		array[x] = '0';
// 	}

// 	array[size-2] = "\0";
// }