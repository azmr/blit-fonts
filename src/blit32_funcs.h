/*
IDEAS:
 - name 'bitcast'?
 	- pun on type casting (also a pun) - software and printing press fonts
	- indicates its low-level/small nature

TODO:
 - drawln -> better cache efficiency
 - finish 32-bit glyphs
 */
#ifndef TINYFONT32_H

#define BASELINE_OFFSET 2
#define GLYPH32_WIDTH  5
#define GLYPH32_HEIGHT 6

// all chars up to 32 are non-printable
// space is at 32, but just space - can be special cased
// ... but is the code to do this more expensive than just adding space?
// probably...
#ifndef IndexFromASCII
#define IndexFromASCII(ascii) (ascii - 32)
#define ASCIIFromIndex(index) (index + 32)
#endif


void
DrawChar32(unsigned int*Buffer, int RowStride, char c, unsigned int Value, int xoffset, int yoffset, unsigned int PixelW, unsigned int PixelH)
{
	unsigned long Glyph = bitcast32_Glyphs[c];
	if((Glyph >> 31) & 1) { yoffset += PixelH; }
	else if((Glyph >> 30) & 1) { yoffset += 2 * PixelH; }

	unsigned int *Row = Buffer + yoffset * RowStride + xoffset;
	for(int y = 0; y < GLYPH32_HEIGHT; ++y)
	{
		for(int pxY = 0; pxY < PixelH; ++pxY)
		{
			/* int y = pxY / PixelH; */
			unsigned int *Pixel = Row;
			for(int x = 0; x < GLYPH32_WIDTH; ++x)
			{
				int Shift = y * GLYPH32_WIDTH + x;
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

/* void */
/* DrawChar(unsigned char *Buffer, int RowStride, int PixelStride, unsigned char c, int xoffset, int yoffset, unsigned int PixelW, unsigned int PixelH) */
/* { */
/* 	if(c > sizeof(bitcast32_Glyphs)/sizeof(*bitcast32_Glyphs) - 1) { c = 0; } // make sure not to try and draw invalid characters (i.e. outside the array) */
/* 	unsigned short Glyph = bitcast32_Glyphs[c]; */
/* 	if(Glyph >> 15 & 1) */
/* 	{ yoffset += BASELINE_OFFSET; } */

/* 	unsigned char *Row = Buffer + yoffset * RowStride + xoffset; */
/* 	for(int y = 0; y < GLYPH32_HEIGHT * PixelH; ++y) */
/* 	{ */
/* 		int GlyphY = y/PixelH; */
/* 		unsigned char *Pixel = Row; */
/* 		for(int x = 0; x < GLYPH32_WIDTH * PixelW; ++x) */
/* 		{ */
/* 			int GlyphX = x/PixelW; */
/* 			int Shift = GlyphY * GLYPH32_WIDTH + GlyphX; */
/* 			unsigned char Value = ((Glyph >> Shift) & 1) ? */
/* 				255 :  // foreground */
/* 				0; // background */
/* 			*Pixel = Value; */
/* 			Pixel += PixelStride; */
/* 		} */
/* 		Row += RowStride; */
/* 	} */
/* } */

void
DrawString32(unsigned int *Buffer, int RowStride, char *String, int StartX, int StartY, int Scale, unsigned int Col)
{
	for(int x = StartX, y = StartY; *String; ++String)
	{
		char c = *String;
		switch(c)
		{
			default: DrawChar32(Buffer, RowStride, IndexFromASCII(c), Col, x, y, Scale, Scale); // fallthrough
			case  ' ': x += Scale * (GLYPH32_WIDTH + 1);     break; // space: no need to touch pixels
			case '\n': y += Scale * (GLYPH32_HEIGHT + BASELINE_OFFSET + 1); // fallthrough
			case '\r': x = StartX;                           break;
			case '\t': x += 4 * Scale * (GLYPH32_WIDTH + 1); break; // tab: add 4 spaces
			case '\b': x -= Scale * (GLYPH32_WIDTH + 1);     break; // non-destructive backspace
		}
	}
}

#define TINYFONT32_H
#endif
