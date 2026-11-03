#include <stdio.h>
#include <wctype.h>
#include <locale.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
	int mode = argc > 1 ? atoi(argv[1]) : 0;
	if (mode == 1) setlocale(LC_ALL, "");
	printf("start mode %d\n", mode); fflush(stdout);
	printf("iswalpha(a)=%d\n", iswalpha(L'a')); fflush(stdout);
	printf("iswalpha(0x4e00)=%d\n", iswalpha(0x4e00)); fflush(stdout);
	printf("iswdigit(5)=%d\n", iswdigit(L'5')); fflush(stdout);
	printf("iswalpha(WEOF)=%d\n", iswalpha(WEOF)); fflush(stdout);
	return 0;
}
