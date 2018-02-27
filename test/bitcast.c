#include <stdio.h>
/* #include "../src/bitcast16_data.h" */
/* #include "../src/bitcast16funcs.h" */
#include "../src/bitcast32_data.h"
#include "../src/bitcast32funcs.h"

#define DRAWWIDTH 480
#define DRAWHEIGHT 320
#define SCALE 3
#define PADDING sizeof(int)

/* int */
/* DrawLineNum(unsigned char *Buffer, int Stride, int Num, int StartX, int StartY, unsigned int Size) */
/* { */
/* 	DrawChar(Buffer, Stride, Num+16, 1, StartX, StartY, Size, Size); */
/* 	int XOffset = GLYPH16_WIDTH + 1; */
/* 	DrawChar(Buffer, Stride, 1, IndexFromASCII(')'), StartX+XOffset, StartY, Size, Size); */
/* 	XOffset += GLYPH16_WIDTH + 1; */
/* 	DrawChar(Buffer, Stride, 1, IndexFromASCII(' '), StartX+XOffset, StartY, Size, Size); */
/* 	XOffset += GLYPH16_WIDTH + 1; */
/* 	return XOffset; */
/* } */

int main(int cargs, char **args)
{
	FILE *PGM;
#define DrawFont(num) PGM = fopen("test"#num".pgm", "wb"); \
	unsigned int image## num[SCALE * SCALE * DRAWWIDTH * DRAWHEIGHT] = {0}; \
	DrawString## num(image## num, SCALE * DRAWWIDTH,  \
		"for(int i = 0; i < 7 && bool == 1; ++i)\n" \
		"{\n" \
		"\tDo_Stuff(Things[a ? i : j]);\n" \
		"}\n" \
		"Not too bad!\n" \
		"abcdefghijklmnopqrstuvwxyz\n" \
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ\n" \
		"!@#$%^&*()-=~`\\\"'\n" \
		"1234567890_+|\n" \
		"email me at `azmreece@gmail.com`\n" \
		, \
		SCALE, SCALE, SCALE, 255); \
	fprintf(PGM, "P2 %u %u 255\n", SCALE * DRAWWIDTH, SCALE * DRAWHEIGHT); \
	for(int i = 0; i < sizeof(image## num)/sizeof(*image## num); ++i) \
	{ fprintf(PGM, "%u ", image## num[i]); } \
	fclose(PGM);

	/* DrawFont(16) */
	DrawFont(32)

	return 0;
}
