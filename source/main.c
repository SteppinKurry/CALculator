// 1-26-24
// Trying to make an "advanced" (TI-84 tier, at best) calculator
// software for DS

#include <nds.h>
#include <stdio.h>
#include <time.h>

#include "calmath.h"
#include "ui.h"
#include "graphing.h"
#include "parse.h"
#include "sizes.h"
#include "node.h"

int8 button_num_into_str(u8 b, char* expression, u8* expression_len)
{
	// 10-12-24
	// Takes a button id, the current, and the length of the current 
	// expression and adds the correct character(s) based upon the 
	// button id

	// if no button is pressed, do nothing
	if (b == 255) { return -1; }

	// each of the possible strings a button press could result in
	char buttons[][6] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", 
				   		 "+", "-", "*", "/", "=", "c", "(", ")", "^", 
						 "pi", "sin(", "sin(", "e", "cos(", "cos(", "E", "tan(", "tan(", 
						 "sqrt(", 
						 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
						 "(A)", "(B)", "=", "(C)", "x", "(ans)", "(ans)"};

	// if the final expression will be too long, reset it
	if (*expression_len + strlen(buttons[b])>= MAX_EXP_CHARS) { *expression_len = 0; expression[0] = '\0'; }

	// add the new part to the expression
	strcat(expression, buttons[b]);

	// increment expression len
	*expression_len += strlen(buttons[b]);

	// worked fine
	return 0;	

}

void shift_graph(struct graph* g, int8 delta_x, int8 delta_y, struct fraction a)
{
	g->cam_x += delta_x;
	g->cam_y += delta_y;
	g->px_conversion = fraction_mul(g->px_conversion, a);
	
	NF_ClearTextLayer(0, 1);
	for (int x = 0; x < 256; x++)
	{
		NF_Show3dSprite(x, false);
	}
}

int main(int argc, char **argv) 
{
	// UI related things
	ui_init();

	// uhhhhhhh
	struct graph g = {fraction_init(1, 10, 1), 0, 0};
	bool graph_mode = false;

	// set variables for all of the UI's screens
	struct ui_screen ui_main = ui_init_main_screen();
	struct ui_screen ui_irrational = ui_init_irr_screen();
	struct ui_screen ui_functions = ui_init_func_screen();

	// pointer to the currently used screen
	struct ui_screen* current_ui = &ui_main;
	
	NF_WriteText(0, 1, 1, 2, "\n==== CALculator ====\n\n");
	NF_WriteText(0, 1, 2, 3, "hey there *winky face*\n\n");
	
	// initialize touch screen, the expression string, 
	// and a few other variables
	touchPosition touch_screen = {0, 0, 0, 0};
	int keys = keysDown();

	char expression[MAX_EXP_CHARS];
	u8 expression_len = 0;
	expression[0] = '\0';

	char tap_toggle = 0;

	// initialize math stuff

	// this array stores the values of user-defined variables in 
	// the program
	struct unsimple_exp var_values[28];

	for (int x = 0; x < 28; x++)
	{
		var_values[x] = unsimple_exp_init();
	}

	
	char mathstring1[MATHSTR_LEN][MAX_NUM_LEN];
	char parsedstring1[MATHSTR_LEN][MAX_NUM_LEN];
	char user_var_ans[] = "0";

	tokenize(user_var_ans, mathstring1);
	parse(mathstring1, parsedstring1);
	construct_unsimple_from_parsedstring(parsedstring1, &var_values[27]);

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

			// used for printing
			char weenres[200];

			// do the thing with the button
			if (button == 14) // equal sign
			{ 

				if (result.denominator > 1 && expression_len == 0)
				{
					// if the last answer was a fraction, pressing equal again 
					// will show the decimal approximation of the fraction

					whereprint = 20;

					double ans = (double) result.numerator / result.denominator;

					if (result.sign == 1)
						sprintf(weenres, "%lld/%lld = %lf*10^%lld", result.numerator, 
									result.denominator, ans, result.sci_notation);

					else
						sprintf(weenres, "%lld/%lld = -%lf*10^%lld", result.numerator, 
								result.denominator, ans, result.sci_notation);

					calc_main_print("                                                             ", &whereprint, 0);
					calc_main_print(weenres, &whereprint, 0);

					whereprint = 15;

					expression[0] = '\0';
					expression_len = 0;

					calc_main_print("                                                ", &whereprint, 0);
					
					continue;
				}

				// if the expression is empty for no reason, do nothing
				else if (strlen(expression) == 0) { continue; }

				//char expression[] = "sin(2/3)";

				// reset the strings
				memset(mathstring, '\0', sizeof(mathstring));
				memset(parsedstring, '\0', sizeof(parsedstring));

				// tokenize and parse the expression
				tokenize(expression, mathstring);
				parse(mathstring, parsedstring);

				// generate an unsimplified expression variable
				struct unsimple_exp uexp = unsimple_exp_init();
				int8 error = construct_unsimple_from_parsedstring(parsedstring, &uexp);

				// ast error (scary)
				if (error == 1)
				{
					whereprint = 10;
					calc_main_print("FUCK", &whereprint, 1);
				}

				unsimple_sub_vars(&uexp, var_values, false);
				unsimple_simplify(&uexp);
				result = unsimple_evaluate(&uexp);
				//result = uexp.root->number;
				
				// reset the print and the string variable
				whereprint = 18;
				weenres[0] = '\0';

				calc_main_print("                                                           ", &whereprint, 0);
				calc_main_print("BEHOLD: ", &whereprint, 1);

				// makes the result look nicer when it's printed
				nice_fraction_print(result.numerator, result.denominator, result.sign, 
									result.sci_notation, expression, weenres);

				if (fraction_is_error(result)) { sprintf(weenres, "error :("); }

				// print whatever is in weenres
				calc_main_print(weenres, &whereprint, 0);

				// reset the expression
				expression[0] = '\0';
				expression_len = 0;

				whereprint = 15;
				calc_main_print("                                                ", &whereprint, 0);

				// move the previous answer into its spot in the user variables
				error = unsimple_copy(&uexp, &var_values[27]);

				if (error != 0) 
				{
					sprintf(weenres, "PANIC: %d", error);
					annoyed_print(weenres);
				}
			}

			// clear button
			else if (button == 15)
			{
				// reset each string
				memset(mathstring, '\0', sizeof(mathstring));
				memset(parsedstring, '\0', sizeof(parsedstring));

				// reset result
				result = fraction_init(1, 1, 1);

				// clear the console
				NF_ClearTextLayer(0, 1);
				for (int x = 0; x < 256; x++)
				{
					NF_Show3dSprite(x, false);
				}
				//NF_ClearTextLayer(0, 1);

				// reset expression
				expression[0] = '\0';
				expression_len = 0;

				whereprint = 15;

				graph_mode = false;
			}
			
			// any button that isn't clear or equal sign
			else
			{
				// length of the expression before modification
				u8 old_len = expression_len;
				char new_word[8] = "";	// represents the part being added to the expression

				// add the new part to the expression
				button_num_into_str(button, expression, &expression_len);

				// this is the part that adds "ans" to the beginning of an expression if an 
				// operator is pressed first

				// if the old length was 0
				if (old_len == 0)
				{
					// put the newly added part into new_word
					for (int x = 0; x < expression_len; x++)
					{
						new_word[x] = expression[x];
					}

					new_word[expression_len] = '\0';

					// if the new part was an operator, put an "ans" in front of it (if it's
					// not an open parenthesis)
					if (is_operator(new_word) && strcmp(new_word, "("))
					{
						expression[0] = '\0';
						strcat(expression, "ans");
						expression_len += 3;
						expression[expression_len] = '\0';
						strcat(expression, new_word);
					}
				}

				// update the line displaying the expression
				calc_main_print(expression, &whereprint, 0);

			}

		}

		// change the button set
		if ((keys & KEY_UP) && !graph_mode)
		{
			if (current_ui == &ui_main)
			{
				set_bottom_ui(&ui_irrational);
				current_ui = &ui_irrational;
			}
			else if (current_ui == &ui_functions)
			{
				set_bottom_ui(&ui_main);
				current_ui = &ui_main;
			}
		}
		
		// change the button set
		if ((keys & KEY_DOWN) && !graph_mode)
		{
			if (current_ui == &ui_irrational)
			{
				set_bottom_ui(&ui_main);
				current_ui = &ui_main;
			}
			else if (current_ui == &ui_main)
			{
				set_bottom_ui(&ui_functions);
				current_ui = &ui_functions;
			}
		}

		// delete the last thing from the expression
		if (keys & KEY_B)
		{
			expression_len -= 1;
			expression[expression_len] = '\0';
			calc_main_print("                                                ", &whereprint, 0);
			calc_main_print(expression, &whereprint, 0);

			if (graph_mode) 
			{
				graph_mode = false;
				
				// reset each string
				memset(mathstring, '\0', sizeof(mathstring));
				memset(parsedstring, '\0', sizeof(parsedstring));

				// reset result
				result = fraction_init(1, 1, 1);

				// clear the console
				NF_ClearTextLayer(0, 1);
				for (int x = 0; x < 256; x++)
				{
					NF_Show3dSprite(x, false);
				}
				//NF_ClearTextLayer(0, 1);

				// reset expression
				expression[0] = '\0';
				expression_len = 0;

				whereprint = 15;

				graph_mode = false;
			}
			
		}

		// graph a function
		if (keys & KEY_R)
		{
			// reset graph container before graphing new function
			g.cam_x = 0;
			g.cam_y = 0;
			g.px_conversion = fraction_init(1, 10, 1);

			// reset the sprites representing the graph
			NF_ClearTextLayer(0, 1);
			for (int x = 0; x < 256; x++)
			{
				NF_Show3dSprite(x, false);
			}
			
			// plot the points and enable graphing mode
			graph_plot_points(&g, &var_values[27]);
			graph_mode = true;
		}
		
		// shift the graph's view left or right
		if ((keys & KEY_RIGHT) && graph_mode)
		{
			shift_graph(&g, 10, 0, fraction_init(1, 1, 1));
			graph_plot_points(&g, &var_values[27]);
		}
		
		else if ((keys & KEY_LEFT) && graph_mode)
		{
			shift_graph(&g, -10, 0, fraction_init(1, 1, 1));
			graph_plot_points(&g, &var_values[27]);
		}

		// shift the graph's view up or down
		if ((keys & KEY_UP) && graph_mode)
		{
			shift_graph(&g, 0, 10, fraction_init(1, 1, 1));
			graph_plot_points(&g, &var_values[27]);
		}
		
		else if ((keys & KEY_DOWN) && graph_mode)
		{
			shift_graph(&g, 0, -10, fraction_init(1, 1, 1));
			graph_plot_points(&g, &var_values[27]);
		}

		// zoom in or out
		else if ((keys & KEY_X) && graph_mode)
		{
			// zoom in
			shift_graph(&g, 0, 0, fraction_init(1, 2, 1));
			graph_plot_points(&g, &var_values[27]);
		}
		else if ((keys & KEY_Y) && graph_mode)
		{
			// zoom out
			shift_graph(&g, 0, 0, fraction_init(2, 1, 1));
			graph_plot_points(&g, &var_values[27]);
		}

		// quit program
		if (keys & KEY_START) break;
		
		NF_Draw3dSprites();
		NF_Sort3dSprites();
		glFlush(0);

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