// 1-26-24
// Trying to make an "advanced" (TI-84 tier, at best) calculator
// software for DS

// Includes C
#include <stdio.h>
#include <time.h>

#include <nds.h>

#include <calmath.h>

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

void OnKeyPressed(int key) 
{
   if(key > 0)
      iprintf("%c", key);
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

	b.numerator = 2465;
	b.denominator = 36;

	for (int x = 0; x < 9999; x++)
	{
		result = bigreal_add(a, b);
	}

	iprintf("result is: %lld / %lld\n", result.numerator, result.denominator);

	iprintf("\nTime: ");
	print_float(divf32(tenth_seconds, 10));
	iprintf("\n");
	
	tenth_seconds = 0;
	timerStop(3);

}

int main(int argc, char **argv) 
{
	consoleDemoInit();
	int keys = keysDown();
	iprintf("press A to do some sick math\n");

	while(1)
   	{	
		swiWaitForVBlank();
		scanKeys();
		keys = keysDown();

		if (keys & KEY_START) break;
		else if (keys & KEY_A) math_routine();

		
	}

	return 0;
}