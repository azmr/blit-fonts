#ifndef blit32_H
#ifndef blit_IndexFromASCII
#define blit_IndexFromASCII(ascii) (ascii - 32)
#define blit_ASCIIFromIndex(index) (index + 32)
#endif

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

#define blit32_WIDTH  5
#define blit32_HEIGHT 6
#define blit32_ADVANCE (blit32_WIDTH + 1)
#define blit32_DESCENDER 2
#define blit32_BASELINE_OFFSET (blit32_DESCENDER + 1)
#define blit32_ROW_ADVANCE (blit32_HEIGHT + blit32_BASELINE_OFFSET)
#define blit32_EXTRA_BITS(x) (((x) >> 30) & 3)

typedef unsigned long blit32_glyph;

#ifndef blit32_ARRAY_ONLY
#define blit32_Glyphs Blit32.Glyphs

typedef struct blit32_font
{
	const blit32_glyph Glyphs[blit_NUM_GLYPHS];
	const unsigned int Width;
	const unsigned int Height;
	const unsigned int Descender;
	const unsigned int Advance;
	const unsigned int RowAdvance;
	        blit_props Props;
} blit32_font;

inline void blit32_Scale(blit32_font *Font, int Scale)
{
	                  Font->Props.Scale = Scale;
	*(unsigned int *)&Font->Width       = blit32_WIDTH       * Scale;
	*(unsigned int *)&Font->Height      = blit32_HEIGHT      * Scale;
	*(unsigned int *)&Font->Descender   = blit32_DESCENDER   * Scale;
	*(unsigned int *)&Font->Advance     = blit32_ADVANCE     * Scale;
	*(unsigned int *)&Font->RowAdvance  = blit32_ROW_ADVANCE * Scale;
}

blit32_font Blit32 = {
#else //blit32_ARRAY_ONLY
blit32_glyph blit32_Glyphs[95] =
#endif//blit32_ARRAY_ONLY
	{
		// all chars up to 32 are non-printable
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
#ifndef blit32_ARRAY_ONLY
	},
	blit32_WIDTH,
	blit32_HEIGHT,
	blit32_DESCENDER,
	blit32_ADVANCE,
	blit32_ROW_ADVANCE,
#endif//blit32_ARRAY_ONLY
};

// StartX/Y refers to the top left corner of the glyph's bounding box
int blit32_StringNExplicit(unsigned int *Buffer, int BufWidth, int BufHeight, int Scale, unsigned int Value, int StartX, int StartY, char *String, int StrLen)
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
		int BufOverflow  = BufHeight && y + (blit32_HEIGHT + blit32_DESCENDER) * Scale >= BufHeight;
		if(BufOverflow || (IsNegative && BufUnderflow)) { break; }
		if(x >= 0 && x + blit32_WIDTH * Scale < BufWidth && !(BufUnderflow || (IsNegative && BufOverflow)))
		switch(c)
		{
			default:                                                 // normal character
				{
					c = blit_IndexFromASCII(c);
					blit32_glyph Glyph = blit32_Glyphs[c];
					unsigned int yoffset = y + blit32_EXTRA_BITS(Glyph) * Scale * DrawDir;
					unsigned int *Row = Buffer + yoffset * AbsBufWidth + x;
					for(unsigned int glY = 0; glY < blit32_HEIGHT; ++glY)
					for(unsigned int pxY = Scale; pxY--; Row += BufWidth)
					for(unsigned int glX = 0, *Pixel = Row; glX < blit32_WIDTH; ++glX)
					{
						unsigned int Shift = glY * blit32_WIDTH + glX;
						unsigned int PixelDrawn = (Glyph >> Shift) & 1;
						if(PixelDrawn) for(unsigned int pxX = Scale, pxVal = *Pixel; pxX--; pxVal = *Pixel)
						{ *Pixel++ = !PixelDrawn * pxVal + PixelDrawn * Value; } /* 0: original val, 1: new val */
						else { Pixel += Scale; }
					}
				} // fallthrough
			case  ' ': x +=           Scale * blit32_ADVANCE; break; // space: no need to touch pixels
			case '\t': x +=       4 * Scale * blit32_ADVANCE; break; // tab: add 4 spaces
			case '\b': x -=           Scale * blit32_ADVANCE; break; // non-destructive backspace
			case '\n': y += DrawDir * Scale * blit32_ROW_ADVANCE;    // newline
			           ++LinesPrinted;                     // fallthrough
			case '\r': x  = StartX;                           break; // carriage return
		}
	}
	return LinesPrinted;
}

#ifndef blit32_NO_HELPERS
inline int blit32_StringExplicit(unsigned int *Buffer, int BufWidth, int BufHeight, int Scale, unsigned int Value, int StartX, int StartY, char *String)
{ return blit32_StringNExplicit(Buffer, BufWidth, BufHeight, Scale, Value, StartX, StartY, String, -1); }

inline int blit32_StringNProps(blit_props Props, int StartX, int StartY, char *String, int StrLen)
{ return blit32_StringNExplicit(Props.Buffer, Props.BufWidth, Props.BufHeight, Props.Scale, Props.Value, StartX, StartY, String, StrLen); }

inline int blit32_StringProps(blit_props Props, int StartX, int StartY, char *String)
{ return blit32_StringNExplicit(Props.Buffer, Props.BufWidth, Props.BufHeight, Props.Scale, Props.Value, StartX, StartY, String, -1); }

inline int blit32_StringN(int StartX, int StartY, char *String, int StrLen)
{ return blit32_StringNExplicit(Blit32.Props.Buffer, Blit32.Props.BufWidth, Blit32.Props.BufHeight, Blit32.Props.Scale, Blit32.Props.Value, StartX, StartY, String, StrLen); }

inline int blit32_String(int StartX, int StartY, char *String)
{ return blit32_StringNExplicit(Blit32.Props.Buffer, Blit32.Props.BufWidth, Blit32.Props.BufHeight, Blit32.Props.Scale, Blit32.Props.Value, StartX, StartY, String, -1); }
#endif//blit32_NO_HELPERS

#define blit32_H
#endif//blit32_H
