#ifndef blit16_H
#ifndef blit_H
#define blit_NUM_GLYPHS 95
#define blit_IndexFromASCII(ascii) (ascii - 32)
#define blit_ASCIIFromIndex(index) (index + 32)
typedef struct blit_props
{
	unsigned int *Buffer;
	unsigned int  Value;
	         int  Scale;
	         int  BufWidth;
	         int  BufHeight;
} blit_props;
#define blit_H
#endif//blit_H

#define blit16_WIDTH 3
#define blit16_HEIGHT 5
#define blit16_ADVANCE (blit16_WIDTH + 1)
#define blit16_DESCENDER 1
#define blit16_BASELINE_OFFSET (blit16_DESCENDER + 1)
#define blit16_ROW_ADVANCE (blit16_HEIGHT + blit16_BASELINE_OFFSET)
#define blit16_EXTRA_BITS(x) (((x) >> 15) & 1)

typedef unsigned short blit16_glyph;

#ifndef blit16_ARRAY_ONLY
#define blit16_Glyphs Blit16.Glyphs
typedef struct blit16_font
{
	const blit16_glyph Glyphs[blit_NUM_GLYPHS];
	const unsigned int Width;
	const unsigned int Height;
	const unsigned int Descender;
	const unsigned int Advance;
	const unsigned int RowAdvance;
	        blit_props Props;
} blit16_font;

inline void blit16_Scale(blit16_font *Font, int Scale)
{
	                  Font->Props.Scale = Scale;
	*(unsigned int *)&Font->Width       = blit16_WIDTH       * Scale;
	*(unsigned int *)&Font->Height      = blit16_HEIGHT      * Scale;
	*(unsigned int *)&Font->Descender   = blit16_DESCENDER   * Scale;
	*(unsigned int *)&Font->Advance     = blit16_ADVANCE     * Scale;
	*(unsigned int *)&Font->RowAdvance  = blit16_ROW_ADVANCE * Scale;
}

blit16_font Blit16 = {
#else//blit16_ARRAY_ONLY
blit16_glyph blit16_Glyphs[blit_NUM_GLYPHS] =
#endif//blit16_ARRAY_ONLY
	{
		// all chars up to 32 are non-printable
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
#ifndef blit16_ARRAY_ONLY
	},
	blit16_WIDTH,
	blit16_HEIGHT,
	blit16_DESCENDER,
	blit16_ADVANCE,
	blit16_ROW_ADVANCE,
#endif//blit16_ARRAY_ONLY
};

// StartX/Y refers to the top left corner of the glyph's bounding box
int blit16_StringNExplicit(unsigned int *Buffer, int BufWidth, int BufHeight, int Scale, unsigned int Value, int StartX, int StartY, char *String, int StrLen)
{
	int IsNegative = BufWidth < 0;
	int DrawDir = IsNegative ? -1 : 1;
	int AbsBufWidth = DrawDir * BufWidth;
	StrLen = StrLen >= 0 ? StrLen : 0x7FFFFFFF; // if negative, wrap to max int
	unsigned int LinesPrinted = 1;
	for(int i = 0, x = StartX, y = StartY; *String && i < StrLen; ++String, ++i)
	{
		char c = *String;
		int BufUnderflow = y < 0;
		int BufOverflow  = BufHeight && y + (blit16_HEIGHT + blit16_DESCENDER) * Scale >= BufHeight;
		if(BufOverflow || (IsNegative && BufUnderflow)) { break; }
		if(x >= 0 && x + blit16_WIDTH * Scale < BufWidth && !(BufUnderflow || (IsNegative && BufOverflow)))
		switch(c)
		{
			default:                                                 // normal character
				{
					c = blit_IndexFromASCII(c);
					blit16_glyph Glyph = blit16_Glyphs[c];
					unsigned int yoffset = y + blit16_EXTRA_BITS(Glyph) * Scale * DrawDir;
					unsigned int *Row = Buffer + yoffset * AbsBufWidth + x;
					for(unsigned int glY = 0; glY < blit16_HEIGHT; ++glY)
					for(unsigned int pxY = Scale; pxY--; Row += BufWidth)
					for(unsigned int glX = 0, *Pixel = Row; glX < blit16_WIDTH; ++glX)
					{
						unsigned int Shift = glY * blit16_WIDTH + glX;
						unsigned int PixelDrawn = (Glyph >> Shift) & 1;
						if(PixelDrawn) for(unsigned int pxX = Scale, pxVal = *Pixel; pxX--; pxVal = *Pixel)
						{ *Pixel++ = !PixelDrawn * pxVal + PixelDrawn * Value; } /* 0: original val, 1: new val */
						else { Pixel += Scale; }
					}
				} // fallthrough
			case  ' ': x +=           Scale * blit16_ADVANCE; break; // space: no need to touch pixels
			case '\t': x +=       4 * Scale * blit16_ADVANCE; break; // tab: add 4 spaces
			case '\b': x -=           Scale * blit16_ADVANCE; break; // non-destructive backspace
			case '\n': y += DrawDir * Scale * blit16_ROW_ADVANCE;    // newline
			           ++LinesPrinted;                     // fallthrough
			case '\r': x  = StartX;                           break; // carriage return
		}
	}
	return LinesPrinted;
}

#ifndef blit16_NO_HELPERS
inline int blit16_StringExplicit(unsigned int *Buffer, int BufWidth, int BufHeight, int Scale, unsigned int Value, int StartX, int StartY, char *String)
{ return blit16_StringNExplicit(Buffer, BufWidth, BufHeight, Scale, Value, StartX, StartY, String, -1); }

inline int blit16_StringNProps(blit_props Props, int StartX, int StartY, char *String, int StrLen)
{ return blit16_StringNExplicit(Props.Buffer, Props.BufWidth, Props.BufHeight, Props.Scale, Props.Value, StartX, StartY, String, StrLen); }

inline int blit16_StringProps(blit_props Props, int StartX, int StartY, char *String)
{ return blit16_StringNExplicit(Props.Buffer, Props.BufWidth, Props.BufHeight, Props.Scale, Props.Value, StartX, StartY, String, -1); }

inline int blit16_StringN(int StartX, int StartY, char *String, int StrLen)
{ return blit16_StringNExplicit(Blit16.Props.Buffer, Blit16.Props.BufWidth, Blit16.Props.BufHeight, Blit16.Props.Scale, Blit16.Props.Value, StartX, StartY, String, StrLen); }

inline int blit16_String(int StartX, int StartY, char *String)
{ return blit16_StringNExplicit(Blit16.Props.Buffer, Blit16.Props.BufWidth, Blit16.Props.BufHeight, Blit16.Props.Scale, Blit16.Props.Value, StartX, StartY, String, -1); }
#endif//blit16_NO_HELPERS

#define blit16_H
#endif//blit16_H
