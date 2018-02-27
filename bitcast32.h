#ifndef BITCAST32_H

typedef unsigned long bitcast32_glyph;

bitcast32_glyph bitcast32_Glyphs[95] = {
0x00000000,0x08021084,0x0000294a,0x15f52bea,0x08fa38be,0x33a22e60,0x2e94d8a6,0x00001084,
0x10421088,0x04421082,0x00a23880,0x00471000,0x04420000,0x00070000,0x0c600000,0x02222200,
0x1d3ad72e,0x3e4214c4,0x3e22222e,0x1d18320f,0x210fc888,0x1d183c3f,0x1d17844c,0x0222221f,
0x1d18ba2e,0x210f463e,0x0c6018c0,0x04401000,0x10411100,0x00e03800,0x04441040,0x0802322e,
0x3c1ef62e,0x231fc544,0x1f18be2f,0x3c10862e,0x1f18c62f,0x3e10bc3f,0x0210bc3f,0x3d1e843e,
0x2318fe31,0x3e42109f,0x0c94211f,0x23149d31,0x3e108421,0x231ad6bb,0x239cd671,0x1d18c62e,
0x0217c62f,0x30eac62e,0x2297c62f,0x1d141a2e,0x0842109f,0x1d18c631,0x08454631,0x375ad631,
0x22a21151,0x08421151,0x3e22221f,0x1842108c,0x20820820,0x0c421086,0x00004544,0xbe000000,
0x00000082,0x1c97b000,0x0e949c21,0x1c10b800,0x1c94b908,0x1c329800,0x42211c4c,0x4e87252e,
0x12949c21,0x18420080,0x98842008,0x12519521,0x0c210842,0x235aac00,0x12949c00,0x0c949800,
0x4213a526,0x7087252e,0x02149800,0x0e837000,0x0c213c42,0x0e94a400,0x0464a400,0x155ac400,
0x36426c00,0x4e872529,0x1e223c00,0x1843188c,0x08421084,0x0c463086,0x0006d800,
};

#define BITCAST32_BASELINE_OFFSET 2
#define BITCAST32_WIDTH  5
#define BITCAST32_HEIGHT 6
#define BITCAST32_STRIDE (BITCAST32_WIDTH + 1)

// all chars up to 32 are non-printable
// space is at 32, but just space - can be special cased
// ... but is the code to do this more expensive than just adding space?
// probably...
#ifndef bitcast_IndexFromASCII
#define bitcast_IndexFromASCII(ascii) (ascii - 32)
#define bitcast_ASCIIFromIndex(index) (index + 32)
#endif

void
bitcast32_Char(unsigned int *Buffer, int RowStride, int Dir, char c, unsigned int Value, int xoffset, int yoffset, unsigned int PixelW, unsigned int PixelH)
{
	bitcast32_glyph Glyph = bitcast32_Glyphs[c];
	yoffset += (2 * (Glyph >> 30 & 1) + (Glyph >> 31 & 1)) * Dir * PixelH;
	unsigned int *Row = Buffer + yoffset * Dir * RowStride + xoffset;
	for(unsigned int y = 0; y < BITCAST32_HEIGHT; ++y)
		for(unsigned int pxY = PixelH; pxY--; Row += RowStride)
			for(unsigned int x = 0, *Pixel = Row; x < BITCAST32_WIDTH; ++x)
			{
				unsigned int Shift = y * BITCAST32_WIDTH + x;
				unsigned int PixelDrawn = (Glyph >> Shift) & 1;
				if(PixelDrawn)
					for(unsigned int pxX = PixelW;
						pxX--;
						*Pixel++ = !PixelDrawn * *Pixel + PixelDrawn * Value);
				else { Pixel += PixelW; }
			}
}

void
bitcast32_String(unsigned int *Buffer, int RowStride, char *String, int StartX, int StartY, int Scale, unsigned int Col)
{
	BEGIN_TIMED_BLOCK;
	int Dir = RowStride < 0 ? -1 : 1;
	for(int x = StartX, y = StartY; *String; ++String)
	{
		char c = *String;
		switch(c)
		{
			default: bitcast32_Char(Buffer, RowStride, Dir, bitcast_IndexFromASCII(c), Col, x, y, Scale, Scale); // fallthrough
			case  ' ': x +=       Scale * BITCAST32_STRIDE; break; // space: no need to touch pixels
			case '\t': x +=   4 * Scale * BITCAST32_STRIDE; break; // tab: add 4 spaces
			case '\b': x -=       Scale * BITCAST32_STRIDE; break; // non-destructive backspace
			case '\n': y += Dir * Scale * (BITCAST32_HEIGHT + BITCAST32_BASELINE_OFFSET + 1);                              // fallthrough
			case '\r': x = StartX;                             break;
		}
	}
	END_TIMED_BLOCK;
}

#define BITCAST32_H
#endif//BITCAST32_H
