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

#include "glyphs16.h"
// TODO: glyphs24?
#include "glyphs32.h"
#include "generatedGlyphs.h"

#define REVERSE_BIT_ORDER 0
#define BASELINE_OFFSET 1
#define GLYPH16_WIDTH 3
#define GLYPH16_HEIGHT 5
#define cGLYPHS 95
#define ROW_SIZE 8

// all chars up to 32 are non-printable
// space is at 32, but just space - can be special cased
// ... but is the code to do this more expensive than just adding space?
// probably...
#define IndexFromASCII(ascii) (ascii - 32)
#define ASCIIFromIndex(index) (index + 32)


// IDEA: could have 2 bits per point -> 2 levels between full on and off
char glyph64[64] = 
//       12345678
/* 0 */ "        "
/* 1 */ "        "
/* 2 */ "        "
/* 3 */ "        "
/* 4 */ "        "
/* 5 */ "        "
/* 6 */ "        "
/* 7 */ "        ";

char glyph32[32] = 
//       01234
/* 0 */ "  #  "
/* 1 */ " # # "
/* 2 */ "#   #"
/* 3 */ "#####"
/* 4 */ "#   #"
/* 5 */ "#   #";
// 2 bits left over - set descender/caps/?vert offset (0-3)

#if 1 // GLYPH GENERATION/TESTING /////////////////////////////////////////////
unsigned int
GlyphToBinary(char *Glyph, int size)
{
	unsigned int Result = 0;
	for(unsigned int i = 0; i < size; ++i)
	{
		int filled = Glyph[i] == '#' || Glyph[i] == 1;
		if(filled)
		{ Result |= 1 << i; }
	}
	return Result;
}

void
GenerateBinaryFromGlyphs(char *Glyphs, int GlyphSize, FILE *File)
{
	fprintf(File, "unsigned short Glyphs%u[%u] = {\n", GlyphSize, cGLYPHS);
	int iGlyph = 0;
	for(char *Glyph = Glyphs, *LastGlyph = Glyphs + cGLYPHS * GlyphSize;
		Glyph <= LastGlyph;
		Glyph += ROW_SIZE * GlyphSize)
	{
		for(int i = 0; i < ROW_SIZE && iGlyph < cGLYPHS; ++i, ++iGlyph)
		{
			fprintf(File, "0x%0*x,", GlyphSize/4, GlyphToBinary(Glyph + i * GlyphSize, GlyphSize));
		}
		putc('\n', File);
	}
	fprintf(File, "};");
}

void
TESTPrintBinaryChar16(unsigned int Glyph)
{
	for(int y = 0; y < 5; ++y)
	{
		for(int x = 0; x < GLYPH16_WIDTH; ++x)
		{
			int Shift = GLYPH16_WIDTH * y + x;
			putc((Glyph >> Shift) & 1 ? '#' : ' ', stdout);
		}
		putc('\n', stdout);
	}
}
#endif// GLYPH GENERATION/TESTING /////////////////////////////////////////////

void
DrawChar16(unsigned int*Buffer, int RowStride, char c, unsigned int Value, int xoffset, int yoffset, unsigned int PixelW, unsigned int PixelH)
{
	unsigned short Glyph = Glyphs16[c];
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
#if 1
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
#else
				unsigned char Value = ((Glyph >> Shift) & 1) ?
					255 :  // foreground
					0; // background
				for(int pxX = 0; pxX < PixelW; ++pxX)
				{
					*Pixel = Value;
					Pixel += PixelStride;
				}
#endif
			}
			Row += RowStride;
		}
	}
}

void
DrawChar(unsigned char *Buffer, int RowStride, int PixelStride, char c, int xoffset, int yoffset, unsigned int PixelW, unsigned int PixelH)
{
	unsigned short Glyph = Glyphs16[c];
	if(Glyph >> 15 & 1)
	{ yoffset += BASELINE_OFFSET; }

	unsigned char *Row = Buffer + yoffset * RowStride + xoffset;
	for(int y = 0; y < GLYPH16_HEIGHT * PixelH; ++y)
	{
		int GlyphY = y/PixelH;
		/* printf("y: %d, GlyphY: %d\n", y, GlyphY); */
		unsigned char *Pixel = Row;
		for(int x = 0; x < GLYPH16_WIDTH * PixelW; ++x)
		{
			int GlyphX = x/PixelW;
			/* printf("\tx: %d, GlyphX: %d\n", x, GlyphX); */
			int Shift = GlyphY * GLYPH16_WIDTH + GlyphX;
			unsigned char Value = ((Glyph >> Shift) & 1) ?
				255 :  // foreground
				0; // background
			*Pixel = Value;
			Pixel += PixelStride;
		}
		/* printf("\n"); */
		Row += RowStride;
	}
}

void
DrawString16(unsigned int *Buffer, int RowStride, char *String, int StartX, int StartY, unsigned int Scale)
{
	for(int x = StartX, y = StartY; *String; ++String)
	{
		char c = *String;
		switch(c)
		{
		case '\n':
			y += Scale * (GLYPH16_HEIGHT + BASELINE_OFFSET + 1);
			x = StartX;
			break;

		case '\t':
			x += Scale * 4 * (GLYPH16_WIDTH + 1); 
			break;

		default:
			DrawChar16(Buffer, RowStride, IndexFromASCII(c), 255, x, y, Scale, Scale);
			x += Scale * (GLYPH16_WIDTH + 1);
		}
	}
}


/*  ASCII TABLE
//   32		 	Space
//   33		!	Exclamation mark
//   34		"	Double quotes (or speech marks)
//   35		#	Number
//   36		$	Dollar
//   37		%	Procenttecken
//   38		&	Ampersand
//   39		'	Single quote
//   40		(	Open parenthesis (or open bracket)
//   41		)	Close parenthesis (or close bracket)
//   42		*	Asterisk
//   43		+	Plus
//   44		,	Comma
//   45		-	Hyphen
//   46		.	Period, dot or full stop
//   47		/	Slash or divide
//   48		0	Zero
//   49		1	One
//   50		2	Two
//   51		3	Three
//   52		4	Four
//   53		5	Five
//   54		6	Six
//   55		7	Seven
//   56		8	Eight
//   57		9	Nine
//   58		:	Colon
//   59		;	Semicolon
//   60		<	Less than (or open angled bracket)
//   61		=	Equals
//   62		>	Greater than (or close angled bracket)
//   63		?	Question mark
//   64		@	At symbol
//   65		A	Uppercase A
//   66		B	Uppercase B
//   67		C	Uppercase C
//   68		D	Uppercase D
//   69		E	Uppercase E
//   70		F	Uppercase F
//   71		G	Uppercase G
//   72		H	Uppercase H
//   73		I	Uppercase I
//   74		J	Uppercase J
//   75		K	Uppercase K
//   76		L	Uppercase L
//   77		M	Uppercase M
//   78		N	Uppercase N
//   79		O	Uppercase O
//   80		P	Uppercase P
//   81		Q	Uppercase Q
//   82		R	Uppercase R
//   83		S	Uppercase S
//   84		T	Uppercase T
//   85		U	Uppercase U
//   86		V	Uppercase V
//   87		W	Uppercase W
//   88		X	Uppercase X
//   89		Y	Uppercase Y
//   90		Z	Uppercase Z
//   91		[	Opening bracket
//   92		\	Backslash
//   93		]	Closing bracket
//   94		^	Caret - circumflex
//   95		_	Underscore
//   96		`	Grave accent
//   97		a	Lowercase a
//   98		b	Lowercase b
//   99		c	Lowercase c
//   100	d	Lowercase d
//   101	e	Lowercase e
//   102	f	Lowercase f
//   103	g	Lowercase g
//   104	h	Lowercase h
//   105	i	Lowercase i
//   106	j	Lowercase j
//   107	k	Lowercase k
//   108	l	Lowercase l
//   109	m	Lowercase m
//   110	n	Lowercase n
//   111	o	Lowercase o
//   112	p	Lowercase p
//   113	q	Lowercase q
//   114	r	Lowercase r
//   115	s	Lowercase s
//   116	t	Lowercase t
//   117	u	Lowercase u
//   118	v	Lowercase v
//   119	w	Lowercase w
//   120	x	Lowercase x
//   121	y	Lowercase y
//   122	z	Lowercase z
//   123	{	Opening brace
//   124	|	Vertical bar
//   125	}	Closing brace
//   126	~	Equivalency sign - tilde
//   127		Delete         */
#define TINYFONT_H
#endif
