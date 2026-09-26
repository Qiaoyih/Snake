#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main()
{
	int a = 0, b = 0;
	scanf("%d %d", &a, &b);
	while (b != 0)
	{
		int x = a % b;
		a = b;
		b = x;
	}
	printf("%d\n", a);
	return 0;
}