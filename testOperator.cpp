#include <stdio.h>
#include <stdlib.h>


void testOperator(int x, int y)

{

	printf("x : %0.8X  y : %0.8X\n", x, y);
	printf("%0.8X & %0.8X : %0.8X\n", x, y, x & y);
	printf("%0.8X ^ %0.8X : %0.8X\n", x, y, x ^ y);
	printf("%0.8X | %0.8X : %0.8X\n", x, y, x | y);
	printf("~%0.8X,  ~%0.8X : %0.8X, %0.8X\n", x, y, ~x,  ~y);
	printf("%0.8X >> %0.8X : %0.8X\n", x, y, x >> y);
	printf("%0.8X << %0.8X : %0.8X\n", x, y, x << y);


}



void main()

{

	int x, y;

	for (int i = 0; i < 5; i++) {
		x = rand();
		y = rand();
		testOperator(x, y);
		printf("-----------------------------------\n");
	}
	

}



