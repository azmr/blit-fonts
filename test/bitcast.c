#include <stdio.h>
#include "../bitcast16.h"
#include "../bitcast32.h"

#define DRAWWIDTH 300
#define DRAWHEIGHT 100
#define SCALE 5
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
#define DrawFont(num) PGM = fopen("string"#num".pgm", "wb"); \
	static unsigned int image## num[SCALE * SCALE * DRAWWIDTH * DRAWHEIGHT] = {0}; \
	bitcast##num##_String (image## num, SCALE * DRAWWIDTH,  \
		"for(int i = 0; i < 7 && bool == 1; ++i)\n" \
		"{\n" \
		"\tDo_Stuff(Things[a ? i : j]);\n" \
		"}\n" \
		"Not too bad!\n" \
		"abcdefghijklmnopqrstuvwxyz\n" \
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ\n" \
		"!@#$%^&*()-=~`\\\"'\n" \
		"1234567890_+|\n" \
		"email me at `????????@gmail.com`\n" \
		, \
		SCALE, SCALE, SCALE, 255); \
	fprintf(PGM, "P2 %u %u 255\n", SCALE * DRAWWIDTH, SCALE * DRAWHEIGHT); \
	for(int i = 0; i < sizeof(image## num)/sizeof(*image## num); ++i) \
	{ fprintf(PGM, "%u ", image## num[i]); } \
	fclose(PGM);

	DrawFont(16)
	DrawFont(32)
	
	int rowLen = 13;
	int row = 0;
	int col = 0;
#undef DrawFont
#define PrintChar(num, c) bitcast##num##_Char(image2## num, SCALE * DRAWWIDTH, 1, c, 255, \
			30 + (SCALE * col * (BITCAST##num##_WIDTH+1)), \
			30 + (SCALE * row * (BITCAST##num##_HEIGHT+BITCAST##num##_BASELINE_OFFSET+1)), \
			SCALE, SCALE )

#define PrintRange(num, start, finish) \
	for(unsigned int i = 0, c = (start - 32); \
			c <= (finish - 32); \
			++i, ++c, row+=(col==rowLen), col = (col!=rowLen) *(col + (col!=rowLen))) \
	{ PrintChar(num, c); \
		/* printf("c: %u(%c), col: %u, row: %u\n", c,c, col, row); \ */\
	}
#define DrawFont(num) PGM = fopen("test"#num".pgm", "wb"); \
	row = 0; col = 0;\
	static unsigned int image2## num[SCALE * SCALE * DRAWWIDTH * DRAWHEIGHT] = {0}; \
	PrintRange(num, '!', '@') \
	PrintRange(num, '[', '`') \
	PrintRange(num, '{', '~') \
	PrintRange(num, 'A', 'Z') \
	++row; col = 0;\
	PrintRange(num, 'a', 'z') \
	fprintf(PGM, "P2 %u %u 255\n", SCALE * DRAWWIDTH, SCALE * DRAWHEIGHT); \
	for(int i = 0; i < sizeof(image2## num)/sizeof(*image2## num); ++i) \
	{ fprintf(PGM, "%u ", image2## num[i]); } \
	fclose(PGM);

	DrawFont(16)
	DrawFont(32)

	return 0;
}
