#include <stdio.h>
#include "bitcast.h"
#define STR_(x) #x
#define STR(x) STR_(x)
#define DRAWWIDTH 198
#define DRAWHEIGHT 72
#define SCALE 8
#define PADDING sizeof(int)

int
DrawLineNum(unsigned char *Buffer, int Stride, int Num, int StartX, int StartY, unsigned int Size)
{
	DrawChar(Buffer, Stride, Num+16, 1, StartX, StartY, Size, Size);
	int XOffset = GLYPH16_WIDTH + 1;
	DrawChar(Buffer, Stride, 1, IndexFromASCII(')'), StartX+XOffset, StartY, Size, Size);
	XOffset += GLYPH16_WIDTH + 1;
	DrawChar(Buffer, Stride, 1, IndexFromASCII(' '), StartX+XOffset, StartY, Size, Size);
	XOffset += GLYPH16_WIDTH + 1;
	return XOffset;
}

int main(int cargs, char **args)
{
#if GENERATE_GLYPHS
	FILE *GlyphFile = fopen("generatedGlyphs.h", "w");
	GenerateBinaryFromGlyphs((char *)GlyphArray16, 16, GlyphFile);
	fclose(GlyphFile);
#else
	unsigned int image[SCALE * SCALE * DRAWWIDTH * DRAWHEIGHT] = {0};

	FILE *PGM = fopen("test.pgm", "wb");
	DrawString16(image, SCALE * DRAWWIDTH, 
			"for(int i = 0; i < 7 && bool == 1; ++i)\n"
			"{\n"
			"\tDo_Stuff(Things[a ? i : j]);\n"
			"}\n"
			"Not too bad!\n"
			"abcdefghijklmnopqrstuvwxyz\n"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
			"!@#$%^&*()-=~`\\\"'\n"
			"1234567890_+|\n"
			"email me at `azmreece@gmail.com`\n"
			,
			SCALE, SCALE, SCALE);

	fprintf(PGM, "P2 %u %u 255\n", SCALE * DRAWWIDTH, SCALE * DRAWHEIGHT);
	for(int i = 0; i < sizeof(image)/sizeof(*image); ++i)
	{
		fprintf(PGM, "%u ", image[i]);
	}
	/* fwrite(image, 1, sizeof(image), PGM); */
	fclose(PGM);

#endif
	return 0;
}
