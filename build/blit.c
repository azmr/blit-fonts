#include <stdio.h>
#define blit_NO_INLINE
#include "blit16test.h"
#include "blit32test.h"

#define SCALE 5
#define DRAWWIDTH  (SCALE * 300)
#define DRAWHEIGHT (SCALE * 200)

#define ArrayLen(arr) (sizeof(arr)/sizeof(*arr))

typedef unsigned int uint;


void BlitPGM(int FontID, char *FilePrefix, blit_props BlitProps)
{
	char Filename[128] = {0};
	sprintf(Filename, "%s%d.pgm", FilePrefix, FontID);
	FILE *PGM = fopen(Filename, "wb");

	fprintf(PGM, "P2 %u %u 255\n", DRAWWIDTH, DRAWHEIGHT);
	for(int i = 0; i < BlitProps.BufWidth*BlitProps.BufHeight; ++i)
	{ fprintf(PGM, "%u ", BlitProps.Buffer[i]); }

	fclose(PGM);
}

void PrintFontDemo(int FontID, blit_props BlitProps)
{
	char TestString[] = "for(int i = 0; i < 7 && bool == 1; ++i)\n"
					   "{\n"
					   "\tDo_Stuff(Things[a ? i : j]);\n"
					   "}\n"
					   "Not too bad!\n"
					   "abcdefghijklmnopqrstuvwxyz\n"
					   "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
					   "!@#$%^&*()-=~`\\\"'\n"
					   "1234567890_+|\n"
					   "email me at `????????@gmail.com`\n";
	switch(FontID)
	{
		case 16: blit16_TextProps(BlitProps, 30, 30, TestString); break;
		case 32: blit32_TextProps(BlitProps, 30, 30, TestString); break;
	}
	BlitPGM(FontID, "string", BlitProps);
}


void PrintChar(int FontID, blit_props BlitProps, int Col, int Row, char c)
{
	switch(FontID)
	{
		case 16: blit16_TextNProps(BlitProps, 260 + Col * BlitProps.Scale * blit16_ADVANCE,
						                      220 + Row * BlitProps.Scale * blit16_ROW_ADVANCE, 1, &c); break;
		case 32: blit32_TextNProps(BlitProps, 200 + Col * BlitProps.Scale * blit32_ADVANCE,
						                      200 + Row * BlitProps.Scale * blit32_ROW_ADVANCE, 1, &c); break;
	}
}

int PrintRange(int FontID, blit_props BlitProps, int *Col, int *Row, char FirstC, char LastC)
{
	int row = *Row; int col = *Col - 1;
	int rowLen = 13;
	int CharsPrinted = 0;
	for(char c = FirstC; c <= LastC; ++c)
	{
		if(++col >= rowLen)
		{
			++row;
			col = 0;
		}
		/* row+=(col==rowLen), col = (col!=rowLen) *(col + (col!=rowLen)); */
		PrintChar(FontID, BlitProps, col, row, c);
		CharsPrinted++;
	}
	*Row = row; *Col = col;
	return CharsPrinted;
}

void PrintFont(int FontID, blit_props BlitProps)
{
	int Row = 0; int Col = 0;
	PrintRange(FontID, BlitProps, &Col, &Row, '!', '@');
	PrintRange(FontID, BlitProps, &Col, &Row, '[', '`');
	PrintRange(FontID, BlitProps, &Col, &Row, '{', '~');
	PrintRange(FontID, BlitProps, &Col, &Row, 'A', 'Z');
	++Row; Col = 0;
	PrintRange(FontID, BlitProps, &Col, &Row, 'a', 'z');
	BlitPGM(FontID, "test", BlitProps);
}

int main(int cargs, char **args)
{
	blit_props BlitProps = { 0, 255, SCALE, DRAWWIDTH, DRAWHEIGHT };

	static unsigned int image1_16[DRAWWIDTH * DRAWHEIGHT] = {0};
	static unsigned int image1_32[DRAWWIDTH * DRAWHEIGHT] = {0};
	BlitProps.Buffer = image1_16;
	PrintFontDemo(16, BlitProps);
	BlitProps.Buffer = image1_32;
	PrintFontDemo(32, BlitProps);
	
	BlitProps.Scale *= 2;
	static unsigned int image2_16[DRAWWIDTH * DRAWHEIGHT] = {0};
	static unsigned int image2_32[DRAWWIDTH * DRAWHEIGHT] = {0};
	BlitProps.Buffer = image2_16;
	PrintFont(16, BlitProps);
	BlitProps.Buffer = image2_32;
	PrintFont(32, BlitProps);

	return 0;
}
