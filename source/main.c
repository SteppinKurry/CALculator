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

int main(int argc, char **argv) 
{
	// UI related things
	ui_init();

	NF_WriteText(0, 0, 1, 2, "\n==== CALculator ====\n\n");
	NF_WriteText(0, 0, 2, 3, "hey there *winky face*\n\n");
	
	// initialize touch screen, the expression string, 
	// and a few other variables
	touchPosition touch_screen = {0, 0, 0, 0};
	int keys = keysDown();

	char expression[64];
	u8 expression_len = 0;

	char tap_toggle = 0;

	// mathstring is for the tokenized expression
	char mathstring[MATHSTR_LEN][MAX_NUM_LEN];

	// parsed string is the for the parsed (postifx) expression
	char parsedstring[MATHSTR_LEN][MAX_NUM_LEN];

	// initialize result variable
	struct bigreal result;
	result.numerator = 0;
	result.denominator = 1;
	result.sign = 1;

	
	u8 whereprint = 15;

	// main loop
	while(1)
   	{	
		// get input from buttons and touch screen
		scanKeys();
		touchRead(&touch_screen);
		keys = keysDown();
		int held = keysHeld();

		if (!held) tap_toggle = 1; 

		// if there was a touch input
		if ((held & KEY_TOUCH) && tap_toggle == 1)
		{

			tap_toggle = 0;

			// figure out which virtual button was touched
			int button = check_touch(&touch_screen);
			iprintf("button: %d\n\n", button);

			// do the thing with the button
			if (button == 14) // equal sign
			{ 

				if (result.denominator > 1)
				{
					// if the last answer was a fraction, pressing equal again 
					// will output the decimal approximation of the fraction

					u32 float_precision = 1000;

					u64 a = div64(result.numerator, result.denominator);
					u32 remainder = mod64(result.numerator, result.denominator);

					int32 decimal = divf32(inttof32(remainder), inttof32(result.denominator)) & 0x00000FFF;
					int32 divided = f32toint(decimal * float_precision);
					

					char weenres[200];
					whereprint = 20;

					if (result.sign == 1)
						sprintf(weenres, "%lld/%lld = %lld.%ld", result.numerator, result.denominator, a, 
																					mod32(divided, float_precision));

					else
						sprintf(weenres, "%lld/%lld = -%lld.%ld", result.numerator, result.denominator, a, 
																					mod32(divided, float_precision));


					calc_main_print("                                                                ", &whereprint, 0);
					calc_main_print(weenres, &whereprint, 0);

					whereprint = 15;

					expression[0] = '\0';
					expression_len = 0;

					whereprint = 15;
					calc_main_print("                                                ", &whereprint, 0);
					
					// reset the result variable
					result.numerator = 0;
					result.denominator = 1;
					result.sign = 1;

					continue;
				}

				// char expression[] = "(2/15)-(2/45)";
				
				// reset the result variable
				result.numerator = 0;
				result.denominator = 1;
				result.sign = 1;

				memset(mathstring, '\0', sizeof(mathstring));
				memset(parsedstring, '\0', sizeof(parsedstring));

				tokenize(expression, mathstring);
				parse(mathstring, parsedstring);
				result = evaluate_parsedstring(parsedstring);

				char weenres[200];
				whereprint = 16;

				sprintf(weenres, "parsed size: %s", mathstring[0]);
				whereprint = 20;

				calc_main_print("                                                                ", &whereprint, 0);
				calc_main_print("BEHOLD: ", &whereprint, 1);


				// makes the result look nicer when it's printed
				if (result.denominator == 1) 
				{ 
					if (result.sign == -1) { sprintf(weenres, "%s = -%lld", expression, result.numerator); }
					else { sprintf(weenres, "%s = %lld", expression, result.numerator); }
					
				}
				else
				{
					if (result.sign == -1) 
					{
						sprintf(weenres, "%s = -%lld\\%lld", expression, result.numerator, result.denominator);
					}
					else
					{
						sprintf(weenres, "%s = %llu\\%llu", expression, result.numerator, result.denominator);
					}

				}
				calc_main_print(weenres, &whereprint, 0);

				expression[0] = '\0';
				expression_len = 0;

				whereprint = 15;
				calc_main_print("                                                ", &whereprint, 0);
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

			else if (button == 13) // fraction button
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

		}


		if (keys & KEY_START) break;

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