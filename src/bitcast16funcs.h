/*
IDEAS:
 - name 'bitcast'?
 	- pun on type casting (also a pun) - software and printing press fonts
	- indicates its low-level/small nature

TODO:
 - drawln -> better cache efficiency
 - finish 32-bit glyphs
 */
#ifndef TINYFONT_H

#define BASELINE_OFFSET 1
#define GLYPH16_WIDTH 3
#define GLYPH16_HEIGHT 5

// all chars up to 32 are non-printable
// space is at 32, but just space - can be special cased
// ... but is the code to do this more expensive than just adding space?
// probably...
#define IndexFromASCII(ascii) (ascii - 32)
#define ASCIIFromIndex(index) (index + 32)


void
DrawChar16(unsigned int*Buffer, int RowStride, char c, unsigned int Value, int xoffset, int yoffset, unsigned int PixelW, unsigned int PixelH)
{
	unsigned short Glyph = bitcast16_Glyphs[c];
	if(Glyph >> 15 & 1)
	{ yoffset += PixelH * BASELINE_OFFSET; }

	unsigned int *Row = Buffer + yoffset * RowStride + xoffset;
	for(int y = 0; y < GLYPH16_HEIGHT; ++y)
	{
		for(int pxY = 0; pxY < PixelH; ++pxY)
		{
			/* int y = pxY / PixelH; */
			unsigned int *Pixel = Row;
			for(int x = 0; x < GLYPH16_WIDTH; ++x)
			{
				int Shift = y * GLYPH16_WIDTH + x;
				int PixelDrawn = ((Glyph >> Shift) & 1);
				if(PixelDrawn) // use lerp-like thing to avoid branch?
				{
					for(int pxX = 0; pxX < PixelW; ++pxX)
					{
						*Pixel = Value;
						++Pixel;// += PixelStride;
					}
				}
				else
				{
					Pixel += PixelW;// * PixelStride;
				}
			}
			Row += RowStride;
		}
	}
}

void
DrawChar(unsigned char *Buffer, int RowStride, int PixelStride, unsigned char c, int xoffset, int yoffset, unsigned int PixelW, unsigned int PixelH)
{
	if(c > sizeof(bitcast16_Glyphs)/sizeof(*bitcast16_Glyphs) - 1) { c = 0; } // make sure not to try and draw invalid characters (i.e. outside the array)
	unsigned short Glyph = bitcast16_Glyphs[c];
	if(Glyph >> 15 & 1)
	{ yoffset += BASELINE_OFFSET; }

	unsigned char *Row = Buffer + yoffset * RowStride + xoffset;
	for(int y = 0; y < GLYPH16_HEIGHT * PixelH; ++y)
	{
		int GlyphY = y/PixelH;
		unsigned char *Pixel = Row;
		for(int x = 0; x < GLYPH16_WIDTH * PixelW; ++x)
		{
			int GlyphX = x/PixelW;
			int Shift = GlyphY * GLYPH16_WIDTH + GlyphX;
			unsigned char Value = ((Glyph >> Shift) & 1) ?
				255 :  // foreground
				0; // background
			*Pixel = Value;
			Pixel += PixelStride;
		}
		Row += RowStride;
	}
}

void
DrawString16(unsigned int *Buffer, int RowStride, char *String, int StartX, int StartY, int Scale, unsigned int Col)
{
	for(int x = StartX, y = StartY; *String; ++String)
	{
		char c = *String;
		switch(c)
		{
			default: DrawChar16(Buffer, RowStride, IndexFromASCII(c), Col, x, y, Scale, Scale); // fallthrough
			case  ' ': x += Scale * (GLYPH16_WIDTH + 1);     break; // space: no need to touch pixels
			case '\n': y += Scale * (GLYPH16_HEIGHT + BASELINE_OFFSET + 1); // fallthrough
			case '\r': x = StartX;                           break;
			case '\t': x += 4 * Scale * (GLYPH16_WIDTH + 1); break; // tab: add 4 spaces
			case '\b': x -= Scale * (GLYPH16_WIDTH + 1);     break; // non-destructive backspace
		}
	}
}

#define TINYFONT_H
#endif
