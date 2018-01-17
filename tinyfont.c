#include <stdio.h>
#include "tinyfont.h"
#include "generatedGlyphs.h"
#define STR_(x) #x
#define STR(x) STR_(x)
#define DRAWWIDTH 200
#define DRAWHEIGHT 70

void
DrawChar(unsigned char *Buffer, int Stride, char c, int xoffset, int yoffset)
{
	unsigned short Glyph = Glyphs16[c];
	if(Glyph >> 15 & 1)
	{ ++yoffset; }

	unsigned char *Row = Buffer + yoffset * Stride + xoffset;
	for(int y = 0; y < GLYPHHEIGHT; ++y)
	{
		unsigned char *Pixel = Row;
		for(int x = 0; x < GLYPHWIDTH; ++x)
		{
			int Shift = y * GLYPHWIDTH + x;
			*Pixel = ((Glyph >> Shift) & 1) ?
				255 :  // foreground
				0; // background
			++Pixel;
		}
		Row += DRAWWIDTH;
	}
}

int
DrawLineNum(unsigned char *Buffer, int Stride, int Num, int StartX, int StartY)
{
	DrawChar(Buffer, Stride, Num+16, StartX, StartY);
	int XOffset = GLYPHWIDTH + 1;
	DrawChar(Buffer, Stride, IndexFromASCII(')'), StartX+XOffset, StartY);
	XOffset += GLYPHWIDTH + 1;
	DrawChar(Buffer, Stride, IndexFromASCII(' '), StartX+XOffset, StartY);
	XOffset += GLYPHWIDTH + 1;
	return XOffset;
}

void DrawString(unsigned char *Buffer, int Stride, char *String, int StartX, int StartY)
{
	char LineNum  = 1;
	int x = DrawLineNum(Buffer, Stride, LineNum++, StartX, StartY);
	int y = StartY;
	
	for(; *String; ++String)
	{
		char c = *String;
		if(c == '\n')
		{
			y += GLYPHHEIGHT + 2;
			x = StartX + DrawLineNum(Buffer, Stride, LineNum++, StartX, y);
		}
		else if(c == '\t')
		{ x += 4 * (GLYPHWIDTH + 1); }
		else
		{
			DrawChar(Buffer, Stride, IndexFromASCII(c), x, y);
			x += GLYPHWIDTH + 1;
		}
	}
}

int main(int cargs, char **args)
{
#if 1
	FILE *GlyphFile = fopen("generatedGlyphs.h", "w");
	GenerateBinaryFromGlyphs((char *)GlyphArray16, 16, GlyphFile);
	fclose(GlyphFile);
#else
	unsigned char image[DRAWWIDTH * DRAWHEIGHT] = {0};

	FILE *PGM = fopen("test.pgm", "wb");
	DrawString(image, DRAWWIDTH,
			"for(int i = 0; i < 7 && bool == 1; ++i)\n"
			"{\n"
			"\tDoStuff(i);\n"
			"}\n"
			,
			1, 1);

	fputs("P5 "STR(DRAWWIDTH)" "STR(DRAWHEIGHT)" 255\n", PGM);
	fwrite(image, 1, sizeof(image), PGM);
	fclose(PGM);

#endif
	return 0;
}
