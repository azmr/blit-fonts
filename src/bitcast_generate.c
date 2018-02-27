#include <stdio.h>
#include "bitcast16_glyphs.h"
#include "bitcast32_glyphs.h"

#define OUTPUT_ROW_SIZE 8
#define cGLYPHS 95

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
	fprintf(File, "bitcast%u_glyph bitcast%u_Glyphs[%u] = {\n", GlyphSize, GlyphSize, cGLYPHS);
	int iGlyph = 0;
	for(char *Glyph = Glyphs, *LastGlyph = Glyphs + cGLYPHS * GlyphSize;
		Glyph <= LastGlyph;
		Glyph += OUTPUT_ROW_SIZE * GlyphSize)
	{
		for(int i = 0; i < OUTPUT_ROW_SIZE && iGlyph < cGLYPHS; ++i, ++iGlyph)
		{
			fprintf(File, "0x%0*x,", GlyphSize/4, GlyphToBinary(Glyph + i * GlyphSize, GlyphSize));
		}
		putc('\n', File);
	}
	fprintf(File, "};");
}

void
TESTPrintBinaryChar16(unsigned long long Glyph, int GlyphWidth, int GlyphHeight)
{
	for(int y = 0; y < GlyphHeight; ++y)
	{
		for(int x = 0; x < GlyphWidth; ++x)
		{
			int Shift = GlyphWidth * y + x;
			putc((Glyph >> Shift) & 1 ? '#' : ' ', stdout);
		}
		putc('\n', stdout);
	}
}

int main()
{
	FILE *AllGlyphs = fopen("generatedGlyphs.h", "w");
	FILE *Glyphs16 = fopen("bitcast16_data.h", "w");
	FILE *Glyphs32 = fopen("bitcast32_data.h", "w");
	fputs("typedef unsigned short bitcast16_glyph;\ntypedef unsigned long bitcast32_glyph;\n\n", AllGlyphs);
	fputs("typedef unsigned short bitcast16_glyph;\n\n", Glyphs16);
	fputs("typedef unsigned long  bitcast32_glyph;\n\n", Glyphs32);
	GenerateBinaryFromGlyphs((char *)GlyphArray16, 16, AllGlyphs);
	GenerateBinaryFromGlyphs((char *)GlyphArray16, 16, Glyphs16);
	fputs("\n\n", AllGlyphs);
	GenerateBinaryFromGlyphs((char *)GlyphArray32, 32, AllGlyphs);
	GenerateBinaryFromGlyphs((char *)GlyphArray32, 32, Glyphs32);
	fclose(AllGlyphs);
	fclose(Glyphs16);
	fclose(Glyphs32);

	return 0;
}

/*  ASCII TABLE
//   32		 	Space
//   33		!	Exclamation mark
//   34		"	Double quotes (or speech marks)
//   35		#	Number
//   36		$	Dollar
//   37		%	Percent token
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
