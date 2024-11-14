
#include "graphing.h"
#include "stdio.h"


void graph_plot_points(struct graph* g, struct unsimple_exp* function)
{
	// 11-13-24

    u8 x_max = GRAPH_MAX_POINTS; // number of points on the x axis
	//u8 y_max = 

	struct unsimple_exp vars[2];

    //NF_Create3dSprite(0, 41, 4, 81, 81);

    //NF_Rotate3dSprite(0, 0, 90, 0);

    struct fraction step = g->px_conversion;

    // to figure out where to start, we need to know how many xs (pixels) to plot,
    // where the camera is centered, and we need to know how many in-graph units 
    // one x (pixel) is worth
    
    // number of pixels between the center and left edge
    int64 start_numerator = ((g->cam_x) - (x_max / 2));

    // put the numerator into the numerator
    struct fraction start = fraction_init_int(start_numerator, 1);

    // multiply by the pixel conversion factor to get the actual, in-graph 
    // value for the starting x value
    start = fraction_mul(start, g->px_conversion);

	// this is the number that is treated as the input 
	vars[1] = unsimple_exp_init();

	// this makes the vars[1] root point to a correctly initialized node
	vars[1].root = unsimple_add_node(node_init_num(NULL, NULL, start), &vars[1]);

	for (int x = 0; x < x_max; x++)
	{

		// make the new x value by adding step to the current one
		vars[1].root->number = fraction_add(vars[1].root->number, step);

		// make a new tree since subbing a variable changes the function
		struct unsimple_exp function_sub;

		// copy the original function into the one that will get its 
		// variables subbed
		if (unsimple_copy(function, &function_sub) != 0)
		{
			annoyed_print("FUCKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK");
			return;
		}

		// sub the new variable value
		unsimple_sub_vars(&function_sub, vars, true);
		//unsimple_simplify(&function_sub);

		// calculate the function value
		struct fraction y_value = unsimple_evaluate(&function_sub);

        if (fraction_is_error(y_value)) { continue; }

		// figure out which pixel to put the point at
        y_value = fraction_div(y_value, g->px_conversion);
		int16 y = (int) fraction_mag(y_value);

		if (y > (81 + g->cam_y)) { continue; }
        if (y < - (88 - g->cam_y)) { continue; }


		// plot the point
		plot_point(x, (81-y) + g->cam_y);
	
	}

}