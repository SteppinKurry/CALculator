// 1-26-24
// Trying to make an "advanced" (TI-84 tier, at best) calculator
// software for DS

#include <nds.h>
#include <stdio.h>
#include <time.h>

#include "calmath.h"
#include "ui.h"
#include "parse.h"
#include "sizes.h"
#include "node.h"

int8 add_char_to_expression(char* expression, char c, u8* expression_len)
{
	// 10-3-24
	// Adds the next number/operator to the expression string

	// '@' means that no button was actually pressed, so return 
	// 1 and do nothing
	if (c == '@') { return 1; }

	// proceed like normal
	expression[*expression_len] = c;
	*expression_len += 1;
	if (*expression_len >= MAX_EXP_CHARS) { *expression_len = 0; expression[0] = '\0'; }
	expression[*expression_len] = '\0';

	return 0;
}

char button_num_to_char(u8 b)
{
	// 10-3-24
	// Turns a number representing a calculator button into the 
	// character that represents it
	u8 buttons[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
				   '+', '-', '*', '/', '=', 'c', '(', ')', '^' };

	// if no button is pressed, return something weird
	if (b == 255) { return '@'; }


	return buttons[b];

}

int main(int argc, char **argv) 
{
	// UI related things
	ui_init();

	// set variables for all of the UI's screens
	struct ui_screen ui_main = ui_init_main_screen();
	struct ui_screen ui_irrational = ui_init_irr_screen();

	// pointer to the currently used screen
	struct ui_screen* current_ui = &ui_main;
	
	NF_WriteText(0, 0, 1, 2, "\n==== CALculator ====\n\n");
	NF_WriteText(0, 0, 2, 3, "hey there *winky face*\n\n");
	
	// initialize touch screen, the expression string, 
	// and a few other variables
	touchPosition touch_screen = {0, 0, 0, 0};
	int keys = keysDown();

	char expression[MAX_EXP_CHARS];
	u8 expression_len = 0;

	char tap_toggle = 0;

	// mathstring is for the tokenized expression
	char mathstring[MATHSTR_LEN][MAX_NUM_LEN];

	// parsed string is the for the parsed (postifx) expression
	char parsedstring[MATHSTR_LEN][MAX_NUM_LEN];

	// initialize result variable
	struct fraction result = fraction_init(0, 1, 1);
	
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
			int button = check_touch(&touch_screen, current_ui);
			iprintf("button: %d\n\n", button);

			// do the thing with the button
			if (button == 14) // equal sign
			{ 

				if (result.denominator > 1 && expression_len == 0)
				{
					// if the last answer was a fraction, pressing equal again 
					// will print the decimal approximation of the fraction

					char weenres[200];
					whereprint = 20;

					double ans = (double) result.numerator / result.denominator;

					if (result.sign == 1)
						sprintf(weenres, "%lld/%lld = %lf", result.numerator, result.denominator, ans);

					else
						sprintf(weenres, "%lld/%lld = -%lf", result.numerator, result.denominator, ans);

					calc_main_print("                                                                ", &whereprint, 0);
					calc_main_print(weenres, &whereprint, 0);

					whereprint = 15;

					expression[0] = '\0';
					expression_len = 0;

					whereprint = 15;
					calc_main_print("                                                ", &whereprint, 0);
					
					continue;
				}

				//char expression[] = "9^7";
				
				memset(mathstring, '\0', sizeof(mathstring));
				memset(parsedstring, '\0', sizeof(parsedstring));

				// tokenize, parse, and evaluate the expression
				tokenize(expression, mathstring);
				parse(mathstring, parsedstring);

				struct unsimple_exp uexp = unsimple_exp_init();
				u8 error = construct_unsimple_from_parsedstring(parsedstring, &uexp);
				
				// ast error (scary)
				if (error == 1)
				{
					whereprint = 10;
					calc_main_print("FUCK", &whereprint, 1);
				}

				unsimple_simplify(&uexp);
				result = unsimple_evaluate(&uexp);

				char weenres[200];
				whereprint = 20;

				calc_main_print("                                                                ", &whereprint, 0);
				calc_main_print("BEHOLD: ", &whereprint, 1);

				// makes the result look nicer when it's printed
				nice_fraction_print(result.numerator, result.denominator, result.sign, expression, weenres);

				// reset the output to have the postorder of the expression
				postorder(&uexp, weenres);

				// print whatever is in weenres
				calc_main_print(weenres, &whereprint, 0);

				// reset the expression
				expression[0] = '\0';
				expression_len = 0;

				whereprint = 15;
				calc_main_print("                                                ", &whereprint, 0);
			}

			// clear button
			else if (button == 15)
			{
				memset(mathstring, '\0', sizeof(mathstring));
				memset(parsedstring, '\0', sizeof(parsedstring));

				result = fraction_init(1, 1, 1);

				NF_ClearTextLayer(0, 0);

				expression[0] = '\0';
				expression_len = 0;
			}
			
			// any button that isn't clear or equal sign
			else
			{
				add_char_to_expression(expression, button_num_to_char(button), &expression_len);
				calc_main_print(expression, &whereprint, 0);
			}

		}

		// change to the irrational button set
		if (keys & KEY_UP)
		{
			set_bottom_ui(&ui_irrational);
			current_ui = &ui_irrational;
		}
		
		// change to the regular button set
		if (keys & KEY_DOWN)
		{
			set_bottom_ui(&ui_main);
			current_ui = &ui_main;
		}

		// quit program
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