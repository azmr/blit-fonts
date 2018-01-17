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
	int XOffset = GLYPH16_WIDTH + 1;
	DrawChar(Buffer, Stride, 1, IndexFromASCII(')'), StartX+XOffset, StartY);
	XOffset += GLYPH16_WIDTH + 1;
	DrawChar(Buffer, Stride, 1, IndexFromASCII(' '), StartX+XOffset, StartY);
	XOffset += GLYPH16_WIDTH + 1;
	return XOffset;
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
	DrawString16(image, DRAWWIDTH, 1,
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
