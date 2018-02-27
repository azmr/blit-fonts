#ifndef BITCAST16_H
// TODO:
// - account for Direction, so that you always give top-left/top-right point as (x, y)

typedef unsigned short bitcast16_glyph;

bitcast16_glyph bitcast16_Glyphs[95] = {
0x0000,0x2092,0x002d,0x5f7d,0x279e,0x52a5,0x7ad6,0x0012,
0x4494,0x1491,0x017a,0x05d0,0x1400,0x01c0,0x0400,0x12a4,
0x2b6a,0x749a,0x752a,0x38a3,0x4f4a,0x38cf,0x3bce,0x12a7,
0x3aae,0x49ae,0x0410,0x1410,0x4454,0x0e38,0x1511,0x10e3,
0x73ee,0x5f7a,0x3beb,0x624e,0x3b6b,0x73cf,0x13cf,0x6b4e,
0x5bed,0x7497,0x2b27,0x5add,0x7249,0x5b7d,0x5b6b,0x3b6e,
0x12eb,0x4f6b,0x5aeb,0x388e,0x2497,0x6b6d,0x256d,0x5f6d,
0x5aad,0x24ad,0x72a7,0x6496,0x4889,0x3493,0x002a,0xf000,
0x0011,0x6b98,0x3b79,0x7270,0x7b74,0x6750,0x95d6,0xb9ee,
0x5b59,0x6410,0xb482,0x56e8,0x6492,0x5be8,0x5b58,0x3b70,
0x976a,0xcd6a,0x1370,0x38f0,0x64ba,0x3b68,0x2568,0x5f68,
0x54a8,0xb9ad,0x73b8,0x64d6,0x2492,0x3593,0x03e0,
};

#define BITCAST16_BASELINE_OFFSET 1
#define BITCAST16_WIDTH 3
#define BITCAST16_HEIGHT 5
#define BITCAST16_STRIDE (BITCAST16_WIDTH + 1)

// all chars up to 32 are non-printable
// space is at 32, but just space - can be special cased
// ... but is the code to do this more expensive than just adding space?
// probably...
#ifndef bitcast_IndexFromASCII
#define bitcast_IndexFromASCII(ascii) (ascii - 32)
#define bitcast_ASCIIFromIndex(index) (index + 32)
#endif

void
bitcast16_Char(unsigned int *Buffer, int RowStride, int Dir, char c, unsigned int Value, int xoffset, int yoffset, unsigned int PixelW, unsigned int PixelH)
{
	bitcast16_glyph Glyph = bitcast16_Glyphs[c];
	yoffset += (Glyph >> 15 & 1) * Dir * PixelH;
	unsigned int *Row = Buffer + yoffset * Dir * RowStride + xoffset;
	for(unsigned int y = 0; y < BITCAST16_HEIGHT; ++y)
		for(unsigned int pxY = PixelH; pxY--; Row += RowStride)
			for(unsigned int x = 0, *Pixel = Row; x < BITCAST16_WIDTH; ++x)
			{
				unsigned int Shift = y * BITCAST16_WIDTH + x;
				unsigned int PixelDrawn = (Glyph >> Shift) & 1;
				if(PixelDrawn)
					for(unsigned int pxX = PixelW;
						pxX--;
						*Pixel++ = !PixelDrawn * *Pixel + PixelDrawn * Value); // 0: original val, 1: new val
				else { Pixel += PixelW; }
			}
}

void
bitcast16_String(unsigned int *Buffer, int RowStride, char *String, int StartX, int StartY, int Scale, unsigned int Col)
{
	int Dir = RowStride < 0 ? -1 : 1;
	for(int x = StartX, y = StartY; *String; ++String)
	{
		char c = *String;
		switch(c)
		{
			default: bitcast16_Char(Buffer, RowStride, Dir, bitcast_IndexFromASCII(c), Col, x, y, Scale, Scale); // fallthrough
			case  ' ': x +=       Scale * BITCAST16_STRIDE; break; // space: no need to touch pixels
			case '\t': x +=   4 * Scale * BITCAST16_STRIDE; break; // tab: add 4 spaces
			case '\b': x -=       Scale * BITCAST16_STRIDE; break; // non-destructive backspace
			case '\n': y += Dir * Scale * (BITCAST16_HEIGHT + BITCAST16_BASELINE_OFFSET + 1);                    // fallthrough
			case '\r': x = StartX;                             break;
		}
	}
}

#define BITCAST16_H
#endif//BITCAST16_H
