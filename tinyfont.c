#include <stdio.h>
#include "tinyfont.h"
#define STR_(x) #x
#define STR(x) STR_(x)
#define DRAWWIDTH 260
#define DRAWHEIGHT 70

int
DrawLineNum(unsigned char *Buffer, int Stride, int Num, int StartX, int StartY)
{
	DrawChar(Buffer, Stride, Num+16, 1, StartX, StartY);
	int XOffset = GLYPHWIDTH + 1;
	DrawChar(Buffer, Stride, 1, IndexFromASCII(')'), StartX+XOffset, StartY);
	XOffset += GLYPHWIDTH + 1;
	DrawChar(Buffer, Stride, 1, IndexFromASCII(' '), StartX+XOffset, StartY);
	XOffset += GLYPHWIDTH + 1;
	return XOffset;
}

void DrawString16(unsigned char *Buffer, int Stride, char *String, int StartX, int StartY)
{
	for(int x = StartX, y = StartY; *String; ++String)
	{
		char c = *String;
		if(c == '\n')
		{
			y += GLYPHHEIGHT + BASELINE_OFFSET + 1;
			x = StartX;// + DrawLineNum(Buffer, Stride, LineNum++, StartX, y);
		}
		else if(c == '\t')
		{ x += 4 * (GLYPHWIDTH + 1); }
		else
		{
			DrawChar(Buffer, Stride, 1, IndexFromASCII(c), x, y);
			x += GLYPHWIDTH + 1;
		}
	}
}

int main(int cargs, char **args)
{

#if 0
	FILE *GlyphFile = fopen("generatedGlyphs.h", "w");
	GenerateBinaryFromGlyphs((char *)GlyphArray16, 16, GlyphFile);
	fclose(GlyphFile);
#else
	unsigned char image[DRAWWIDTH * DRAWHEIGHT] = {0};

	FILE *PGM = fopen("test.pgm", "wb");
	DrawString16(image, DRAWWIDTH,
			"for(int i = 0; i < 7 && bool == 1; ++i)\n"
			"{\n"
			"\tDo_Stuff(Things[a ? i : j]);\n"
			"}\n"
			"abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
			,
			1, 1);

	fputs("P5 "STR(DRAWWIDTH)" "STR(DRAWHEIGHT)" 255\n", PGM);
	fwrite(image, 1, sizeof(image), PGM);
	fclose(PGM);

#endif
	return 0;
}
