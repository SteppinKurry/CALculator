// 11-13-24
// Functions related to graphing mathematical functions 

#ifndef GRAPHHHHING
#define GRAPHHHHING

#include "calmath.h"
#include "sizes.h"
#include "ui.h"
#include "node.h"

struct graph
{
    // used for sizing; represents the number of in-graph units one 
    // pixel is worth
    struct fraction px_conversion;

    int64 cam_x;
    int64 cam_y;

};

void graph_plot_points(struct graph* g, struct unsimple_exp* function);








#endif