/* ISC License
 * Copyright (c) 2018 Andrew Reece
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <stdio.h>
#define blit_NO_INLINE
#include "blit16test.h"
#include "blit32test.h"

void BlitPGM(int FontID, int DemoN, blit_props BlitProps)
{
	char Filename[128] = {0};
	sprintf(Filename, "blit%d_%d.pgm", FontID, DemoN);
	FILE *PGM = fopen(Filename, "wb");

	fprintf(PGM, "P2 %u %u 255\n", BlitProps.BufWidth, BlitProps.BufHeight);
	for(int i = 0; i < BlitProps.BufWidth*BlitProps.BufHeight; ++i)
	{ fprintf(PGM, "%u ", BlitProps.Buffer[i]); }

	fclose(PGM);
}

void FontDemo1(int FontID, blit_props BlitProps, int x, int y)
{
	char TestString[] =
		"\n"
		" abcdefghijklm\n"
		" nopqrstuvwxyz\n"
		" ABCDEFGHIJKLM\n"
		" NOPQRSTUVWXYZ\n"
		" 0123456789+-=\n"
		"  {[(</|\\>)]}\n"
		"  !@#$%^&*~_?\n"
		"    ,;`\"':.";
#define W1 15
#define H1 10
	switch(FontID)
	{
		case 16: blit16_TextProps(BlitProps, x, y, TestString); break;
		case 32: blit32_TextProps(BlitProps, x, y, TestString); break;
	}
	BlitPGM(FontID, 1, BlitProps);
}

void FontDemo2(int FontID, blit_props BlitProps, int x, int y)
{
	char TestString[] =
		"\n"
#define L2  " for(int i = 0; i < 7 && *bool == 1; ++i)\n"
			L2
			" {\n"
			" \tDo_Stuff(Things[a ? 2*i : j]);\n"
			" }\n"
			" Not too bad!\n"
			" abcdefghijklmnopqrstuvwxyz\n"
			" ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
			" !@#$%^&*()-=~`\\\"'\n"
			" 1234567890_+|\n"
			" email me at `????????@gmail.com`\n";
#define W2 (sizeof(L2)-1)
#define H2 12
	switch(FontID)
	{
		case 16: blit16_TextProps(BlitProps, x, y, TestString); break;
		case 32: blit32_TextProps(BlitProps, x, y, TestString); break;
	}
	BlitPGM(FontID, 2, BlitProps);
}


void PrintChar(int FontID, blit_props BlitProps, int Col, int Row, char c)
{
	switch(FontID)
	{
		case 16: blit16_TextNProps(BlitProps, BlitProps.Scale * (2 + Col * blit16_ADVANCE),
						                      BlitProps.Scale * (2 + Row * blit16_ROW_ADVANCE), 1, &c); break;
		case 32: blit32_TextNProps(BlitProps, BlitProps.Scale * (2 + Col * blit32_ADVANCE),
						                      BlitProps.Scale * (2 + Row * blit32_ROW_ADVANCE), 1, &c); break;
	}
}

int PrintRange(int FontID, blit_props BlitProps, int *Col, int *Row, char FirstC, char LastC)
{
	int row = *Row; int col = *Col;
	int rowLen = 14;
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

void FontDemo3(int FontID, blit_props BlitProps)
{
	int Row = 0; int Col = -1;
	PrintRange(FontID, BlitProps, &Col, &Row, '!', '@');
	PrintRange(FontID, BlitProps, &Col, &Row, '[', '`');
	PrintRange(FontID, BlitProps, &Col, &Row, '{', '~');
	PrintRange(FontID, BlitProps, &Col, &Row, 'A', 'Z');
	++Row; Col = -1;
	PrintRange(FontID, BlitProps, &Col, &Row, 'a', 'z');
	BlitPGM(FontID, 3, BlitProps);
}

int main(int cargs, char **args)
{
#define SCALE 4
#define WIDTH(d, b)  (SCALE * W##d * blit## b ##_ADVANCE)
#define HEIGHT(d, b) (SCALE * H##d * blit## b ##_ROW_ADVANCE)
#define MAXWIDTH(d)  WIDTH (d, 32)
#define MAXHEIGHT(d) HEIGHT(d, 32)
#define CENTRE_FONT(d, b) (MAXWIDTH(d)/2-WIDTH(d, b)/2), (MAXHEIGHT(d)/2-HEIGHT(d, b)/2)
#define DEMO(d, b) \
	puts("Demo "#d", blit"#b);\
	static unsigned int image## d ##_## b[MAXWIDTH(d) * MAXHEIGHT(d)] = {0};\
	blit_props Props## d ##_## b = { image## d ##_## b, 255, SCALE, MAXWIDTH(d), MAXHEIGHT(d) };\
	FontDemo## d(b, Props## d ##_## b, CENTRE_FONT(d, b));

	DEMO(1, 16);
	DEMO(1, 32);

	DEMO(2, 16);
	DEMO(2, 32);

	return 0;
}
