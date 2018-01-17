#ifndef TINYFONT_H

#define REVERSE_BIT_ORDER 0

// IDEA: could have 2 bits per point -> 2 levels between full on and off
// TODO: decide on distance between letters and lines? or leave to implementation?

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

//00100011000111111100010101000100
unsigned int
GlyphToBinary(char *Glyph, int size)
{
#if REVERSE_BIT_ORDER
	unsigned int MaxShift = size - 1;
#endif
	unsigned int Result = 0;
	for(unsigned int i = 0; i < size; ++i)
	{
		int filled = Glyph[i] == '#' || Glyph[i] == 1;
		/* putc(filled ? '1' : '0', stdout); */
		if(filled)
		{
#if REVERSE_BIT_ORDER
			Result |= 1 << (MaxShift-i);
#else
			Result |= 1 << i;
#endif
		}
	}
	/* putc('\n', stdout); */
	return Result;
}

// why don't we do for loops like: for(int i = 0; ++i < end;)
// also allows for (for-while)   : for(int i = 0; i++ < end;)

void
GenerateBinaryFromGlyphs(char *Glyphs, int GlyphSize, FILE *File)
{
#define cGLYPHS 95
#define ROW_SIZE 8
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
PrintBinaryChar16(unsigned int Glyph)
{
#define GLYPHWIDTH 3
#define GLYPHHEIGHT 5
	for(int y = 0; y < 5; ++y)
	{
		for(int x = 0; x < GLYPHWIDTH; ++x)
		{
			int Shift = GLYPHWIDTH * y + x;
			putc((Glyph >> Shift) & 1 ? '#' : ' ', stdout);
		}
		putc('\n', stdout);
	}
}

char
IndexFromASCII(char letter)
{
	// all chars up to 32 are non-printable
	// space is at 32, but just space - can be special cased
	// ... but is the code to do this more expensive than just adding space?
	// probably...
	return letter - 32;
}


// 1 bit left over - set descender
char GlyphArray16[96][16] = {
//   32		 	Space
{/*      012 */
/* 0 */ "   "
/* 1 */ "   "
/* 2 */ "   "
/* 3 */ "   "
/* 4 */ "   "},

//   33		!	Exclamation mark
{/*      012 */
/* 0 */ " # "
/* 1 */ " # "
/* 2 */ " # "
/* 3 */ "   "
/* 4 */ " # "},

//   34		"	Double quotes (or speech marks)
{/*      012 */
/* 0 */ "# #"
/* 1 */ "# #"
/* 2 */ "   "
/* 3 */ "   "
/* 4 */ "   "},

//   35		#	Number
{/*      012 */
/* 0 */ "# #"
/* 1 */ "###"
/* 2 */ "# #"
/* 3 */ "###"
/* 4 */ "# #"},

//   36		$	Dollar
{/*      012 */
/* 0 */ " ##"
/* 1 */ "## "
/* 2 */ " ##"
/* 3 */ "## "
/* 4 */ " # "
},

//   37		%	Percent
{/*      012 */
/* 0 */ "# #"
/* 1 */ "  #"
/* 2 */ " # "
/* 3 */ "#  "
/* 4 */ "# #"},

//   38		&	Ampersand
{/*      012 */
/* 0 */ "## "
/* 1 */ " # "
/* 2 */ "###"
/* 3 */ "# #"
/* 4 */ " ##"},

//   39		'	Single quote
{/*      012 */
/* 0 */ " # "
/* 1 */ " # "
/* 2 */ "   "
/* 3 */ "   "
/* 4 */ "   "},

//   40		(	Open parenthesis (or open bracket)
{/*      012 */
/* 0 */ "  #"
/* 1 */ " # "
/* 2 */ " # "
/* 3 */ " # "
/* 4 */ "  #"},

//   41		)	Close parenthesis (or close bracket)
{/*      012 */
/* 0 */ "#  "
/* 1 */ " # "
/* 2 */ " # "
/* 3 */ " # "
/* 4 */ "#  "},

//   42		*	Asterisk
{/*      012 */
/* 0 */ " # "
/* 1 */ "###"
/* 2 */ "# #"
/* 3 */ "   "
/* 4 */ "   "},

//   43		+	Plus
{/*      012 */
/* 0 */ "   "
/* 1 */ " # "
/* 2 */ "###"
/* 3 */ " # "
/* 4 */ "   "},

//   44		,	Comma
{/*      012 */
/* 0 */ "   "
/* 1 */ "   "
/* 2 */ "   "
/* 3 */ " # "
/* 4 */ "#  "},

//   45		-	Hyphen
{/*      012 */
/* 0 */ "   "
/* 1 */ "   "
/* 2 */ "###"
/* 3 */ "   "
/* 4 */ "   "},

//   46		.	Period, dot or full stop
{/*      012 */
/* 0 */ "   "
/* 1 */ "   "
/* 2 */ "   "
/* 3 */ " # "
/* 4 */ "   "},

//   47		/	Slash or divide
{/*      012 */
/* 0 */ "  #"
/* 1 */ "  #"
/* 2 */ " # "
/* 3 */ "#  "
/* 4 */ "#  "},

//   48		0	Zero
{/*      012 */
/* 0 */ " # "
/* 1 */ "# #"
/* 2 */ "# #"
/* 3 */ "# #"
/* 4 */ " # "},

//   49		1	One
{/*      012 */
/* 0 */ " # "
/* 1 */ "## "
/* 2 */ " # "
/* 3 */ " # "
/* 4 */ "###"},

//   50		2	Two
{/*      012 */
/* 0 */ " # "
/* 1 */ "# #"
/* 2 */ "  #"
/* 3 */ " # "
/* 4 */ "###"},

//   51		3	Three
{/*      012 */
/* 0 */ "## "
/* 1 */ "  #"
/* 2 */ " # "
/* 3 */ "  #"
/* 4 */ "## "},

//   52		4	Four
{/*      012 */
/* 0 */ " # "
/* 1 */ "#  "
/* 2 */ "# #"
/* 3 */ "###"
/* 4 */ "  #"},

//   53		5	Five
{/*      012 */
/* 0 */ "###"
/* 1 */ "#  "
/* 2 */ "## "
/* 3 */ "  #"
/* 4 */ "## "},

//   54		6	Six
{/*      012 */
/* 0 */ " ##"
/* 1 */ "#  "
/* 2 */ "###"
/* 3 */ "# #"
/* 4 */ "## "},

//   55		7	Seven
{/*      012 */
/* 0 */ "###"
/* 1 */ "  #"
/* 2 */ " # "
/* 3 */ "#  "
/* 4 */ "#  "},

//   56		8	Eight
{/*      012 */
/* 0 */ " ##"
/* 1 */ "# #"
/* 2 */ " # "
/* 3 */ "# #"
/* 4 */ "## "},

//   57		9	Nine
{/*      012 */
/* 0 */ " ##"
/* 1 */ "# #"
/* 2 */ " ##"
/* 3 */ "  #"
/* 4 */ "  #"},

//   58		:	Colon
{/*      012 */
/* 0 */ "   "
/* 1 */ " # "
/* 2 */ "   "
/* 3 */ " # "
/* 4 */ "   "},

//   59		;	Semicolon
{/*      012 */
/* 0 */ "   "
/* 1 */ " # "
/* 2 */ "   "
/* 3 */ " # "
/* 4 */ "#  "},

//   60		<	Less than (or open angled bracket)
{/*      012 */
/* 0 */ "  #"
/* 1 */ " # "
/* 2 */ "#  "
/* 3 */ " # "
/* 4 */ "  #"},

//   61		=	Equals
{/*      012 */
/* 0 */ "   "
/* 1 */ "###"
/* 2 */ "   "
/* 3 */ "###"
/* 4 */ "   "},

//   62		>	Greater than (or close angled bracket)
{/*      012 */
/* 0 */ "#  "
/* 1 */ " # "
/* 2 */ "  #"
/* 3 */ " # "
/* 4 */ "#  "},

//   63		?	Question mark
{/*      012 */
/* 0 */ "## "
/* 1 */ "  #"
/* 2 */ "## "
/* 3 */ "   "
/* 4 */ "#  "},

//   64		@	At symbol
{/*      012 */
/* 0 */ " ##"
/* 1 */ "###"
/* 2 */ "###"
/* 3 */ "#  "
/* 4 */ "###"},

//   65		A	Uppercase A
{/*      012 */
/* 0 */ " # "
/* 1 */ "###"
/* 2 */ "# #"
/* 3 */ "###"
/* 4 */ "# #"},

//   66		B	Uppercase B
{/*      012 */
/* 0 */ "## "
/* 1 */ "# #"
/* 2 */ "###"
/* 3 */ "# #"
/* 4 */ "## "},

//   67		C	Uppercase C
{/*      012 */
/* 0 */ " ##"
/* 1 */ "#  "
/* 2 */ "#  "
/* 3 */ "#  "
/* 4 */ " ##"},

//   68		D	Uppercase D
{/*      012 */
/* 0 */ "## "
/* 1 */ "# #"
/* 2 */ "# #"
/* 3 */ "# #"
/* 4 */ "## "},

//   69		E	Uppercase E
{/*      012 */
/* 0 */ "###"
/* 1 */ "#  "
/* 2 */ "###"
/* 3 */ "#  "
/* 4 */ "###"},

//   70		F	Uppercase F
{/*      012 */
/* 0 */ "###"
/* 1 */ "#  "
/* 2 */ "###"
/* 3 */ "#  "
/* 4 */ "#  "},

//   71		G	Uppercase G
{/*      012 */
/* 0 */ " ##"
/* 1 */ "#  "
/* 2 */ "# #"
/* 3 */ "# #"
/* 4 */ " ##"},

//   72		H	Uppercase H
{/*      012 */
/* 0 */ "# #"
/* 1 */ "# #"
/* 2 */ "###"
/* 3 */ "# #"
/* 4 */ "# #"},

//   73		I	Uppercase I
{/*      012 */
/* 0 */ "###"
/* 1 */ " # "
/* 2 */ " # "
/* 3 */ " # "
/* 4 */ "###"},

//   74		J	Uppercase J
{/*      012 */
/* 0 */ "###"
/* 1 */ "  #"
/* 2 */ "  #"
/* 3 */ "# #"
/* 4 */ " # "},

//   75		K	Uppercase K
{/*      012 */
/* 0 */ "# #"
/* 1 */ "## "
/* 2 */ "## "
/* 3 */ "# #"
/* 4 */ "# #"},

//   76		L	Uppercase L
{/*      012 */
/* 0 */ "#  "
/* 1 */ "#  "
/* 2 */ "#  "
/* 3 */ "#  "
/* 4 */ "###"},

//   77		M	Uppercase M
{/*      012 */
/* 0 */ "# #"
/* 1 */ "###"
/* 2 */ "# #"
/* 3 */ "# #"
/* 4 */ "# #"},

//   78		N	Uppercase N
{/*      012 */
/* 0 */ "## "
/* 1 */ "# #"
/* 2 */ "# #"
/* 3 */ "# #"
/* 4 */ "# #"},

//   79		O	Uppercase O
{/*      012 */
/* 0 */ " ##"
/* 1 */ "# #"
/* 2 */ "# #"
/* 3 */ "# #"
/* 4 */ "## "},

//   80		P	Uppercase P
{/*      012 */
/* 0 */ "## "
/* 1 */ "# #"
/* 2 */ "## "
/* 3 */ "#  "
/* 4 */ "#  "},

//   81		Q	Uppercase Q
{/*      012 */
/* 0 */ "###"
/* 1 */ "# #"
/* 2 */ "# #"
/* 3 */ "###"
/* 4 */ "  #"},

//   82		R	Uppercase R
{/*      012 */
/* 0 */ "## "
/* 1 */ "# #"
/* 2 */ "## "
/* 3 */ "# #"
/* 4 */ "# #"},

//   83		S	Uppercase S
{/*      012 */
/* 0 */ " ##"
/* 1 */ "#  "
/* 2 */ " # "
/* 3 */ "  #"
/* 4 */ "## "},

//   84		T	Uppercase T
{/*      012 */
/* 0 */ "###"
/* 1 */ " # "
/* 2 */ " # "
/* 3 */ " # "
/* 4 */ " # "},

//   85		U	Uppercase U
{/*      012 */
/* 0 */ "# #"
/* 1 */ "# #"
/* 2 */ "# #"
/* 3 */ "# #"
/* 4 */ " ##"},

//   86		V	Uppercase V
{/*      012 */
/* 0 */ "# #"
/* 1 */ "# #"
/* 2 */ "# #"
/* 3 */ " # "
/* 4 */ " # "},

//   87		W	Uppercase W
{/*      012 */
/* 0 */ "# #"
/* 1 */ "# #"
/* 2 */ "# #"
/* 3 */ "###"
/* 4 */ "# #"},

//   88		X	Uppercase X
{/*      012 */
/* 0 */ "# #"
/* 1 */ "# #"
/* 2 */ " # "
/* 3 */ "# #"
/* 4 */ "# #"},

//   89		Y	Uppercase Y
{/*      012 */
/* 0 */ "# #"
/* 1 */ "# #"
/* 2 */ " # "
/* 3 */ " # "
/* 4 */ " # "},

//   90		Z	Uppercase Z
{/*      012 */
/* 0 */ "###"
/* 1 */ "  #"
/* 2 */ " # "
/* 3 */ "#  "
/* 4 */ "###"},

//   91		[	Opening bracket
{/*      012 */
/* 0 */ " ##"
/* 1 */ " # "
/* 2 */ " # "
/* 3 */ " # "
/* 4 */ " ##"},

//   92		\	Backslash
{/*      012 */
/* 0 */ "#  "
/* 1 */ "#  "
/* 2 */ " # "
/* 3 */ "  #"
/* 4 */ "  #"},

//   93		]	Closing bracket
{/*      012 */
/* 0 */ "## "
/* 1 */ " # "
/* 2 */ " # "
/* 3 */ " # "
/* 4 */ "## "},

//   94		^	Caret - circumflex
{/*      012 */
/* 0 */ " # "
/* 1 */ "# #"
/* 2 */ "   "
/* 3 */ "   "
/* 4 */ "   "},

//   95		_	Underscore
{/*      012 */
/* 0 */ "   "
/* 1 */ "   "
/* 2 */ "   "
/* 3 */ "   "
/* 4 */ "###"}, /* could put below baseline... */

//   96		`	Grave accent
{/*      012 */
/* 0 */ "#  "
/* 1 */ " # "
/* 2 */ "   "
/* 3 */ "   "
/* 4 */ "   "},

//   97		a	Lowercase a
{/*      012 */
/* 0 */ " # "
/* 1 */ "  #"
/* 2 */ "###"
/* 3 */ "# #"
/* 4 */ " ##"},

//   98		b	Lowercase b
{/*      012 */
/* 0 */ "#  "
/* 1 */ "#  "
/* 2 */ "###"
/* 3 */ "# #"
/* 4 */ "## "},

//   99		c	Lowercase c
{/*      012 */
/* 0 */ "   "
/* 1 */ "   "
/* 2 */ " ##"
/* 3 */ "#  "
/* 4 */ "###"},

//   100	d	Lowercase d
{/*      012 */
/* 0 */ "  #"
/* 1 */ "  #"
/* 2 */ " ##"
/* 3 */ "# #"
/* 4 */ "###"},

//   101	e	Lowercase e
{/*      012 */
/* 0 */ "   "
/* 1 */ " # "
/* 2 */ "# #"
/* 3 */ "## "
/* 4 */ " ##"},

//   102	f	Lowercase f
{/*      012 */
/* 0 */ "  #"
/* 1 */ " # "
/* 2 */ "###"
/* 3 */ " # " /* <-- baseline */
/* 4 */ "## " "#"},

//   103	g	Lowercase g
{/*      012 */
/* 0 */ " ##"
/* 1 */ "# #"
/* 2 */ "###"
/* 3 */ "  #" /* <-- baseline */
/* 4 */ "## " "#"},

//   104	h	Lowercase h
{/*      012 */
/* 0 */ "#  "
/* 1 */ "#  "
/* 2 */ "## "
/* 3 */ "# #"
/* 4 */ "# #"},

//   105	i	Lowercase i
{/*      012 */
/* 0 */ "   "
/* 1 */ " # "
/* 2 */ "   "
/* 3 */ " # "
/* 4 */ " ##"},

//   106	j	Lowercase j
{/*      012 */
/* 0 */ " # "
/* 1 */ "   "
/* 2 */ " # "
/* 3 */ " # " /* <-- baseline */
/* 4 */ "#  " "#"},

//   107	k	Lowercase k
{/*      012 */
/* 0 */ "   "
/* 1 */ "# #"
/* 2 */ "## "
/* 3 */ "## "
/* 4 */ "# #"},

//   108	l	Lowercase l
{/*      012 */
/* 0 */ " # "
/* 1 */ " # "
/* 2 */ " # "
/* 3 */ " # "
/* 4 */ "  #"},

//   109	m	Lowercase m
{/*      012 */
/* 0 */ "   "
/* 1 */ "   "
/* 2 */ "###"
/* 3 */ "###"
/* 4 */ "# #"},

//   110	n	Lowercase n
{/*      012 */
/* 0 */ "   "
/* 1 */ "   "
/* 2 */ "## "
/* 3 */ "# #"
/* 4 */ "# #"},

//   111	o	Lowercase o
{/*      012 */
/* 0 */ "   "
/* 1 */ "   "
/* 2 */ " ##"
/* 3 */ "# #"
/* 4 */ "## "},

//   112	p	Lowercase p
{/*      012 */
/* 0 */ " # "
/* 1 */ "# #"
/* 2 */ "## "
/* 3 */ "#  " /* <-- baseline */
/* 4 */ "#  " "#"},

//   113	q	Lowercase q
{/*      012 */
/* 0 */ " # "
/* 1 */ "# #"
/* 2 */ " ##"
/* 3 */ "  #" /* <-- baseline */
/* 4 */ "  #" "#"},

//   114	r	Lowercase r
{/*      012 */
/* 0 */ "   "
/* 1 */ "   "
/* 2 */ "## "
/* 3 */ "# #"
/* 4 */ "#  "},

//   115	s	Lowercase s
{/*      012 */
/* 0 */ "   "
/* 1 */ " ##"
/* 2 */ "## "
/* 3 */ "  #"
/* 4 */ "## "},

//   116	t	Lowercase t
{/*      012 */
/* 0 */ " # "
/* 1 */ " # "
/* 2 */ "###"
/* 3 */ " # "
/* 4 */ " ##"},

//   117	u	Lowercase u
{/*      012 */
/* 0 */ "   "
/* 1 */ "   "
/* 2 */ "# #"
/* 3 */ "# #"
/* 4 */ "## "},

//   118	v	Lowercase v
{/*      012 */
/* 0 */ "   "
/* 1 */ "   "
/* 2 */ "# #"
/* 3 */ " # "
/* 4 */ " # "},

//   119	w	Lowercase w
{/*      012 */
/* 0 */ "   "
/* 1 */ "   "
/* 2 */ "# #"
/* 3 */ "###"
/* 4 */ "###"},

//   120	x	Lowercase x
{/*      012 */
/* 0 */ "   "
/* 1 */ "   "
/* 2 */ "# #"
/* 3 */ " # "
/* 4 */ "# #"},

//   121	y	Lowercase y
{/*      012 */
/* 0 */ "# #"
/* 1 */ "# #"
/* 2 */ " ##"
/* 3 */ "  #"
/* 4 */ "## " "#"},

//   122	z	Lowercase z
{/*      012 */
/* 0 */ "   "
/* 1 */ "   "
/* 2 */ "## "
/* 3 */ " # "
/* 4 */ " ##"},

//   123	{	Opening brace
{/*      012 */
/* 0 */ " ##"
/* 1 */ " # "
/* 2 */ "## "
/* 3 */ " # "
/* 4 */ " ##"},
             
//   124	\	Vertical bar
{/*      012 */
/* 0 */ " # "
/* 1 */ " # "
/* 2 */ " # "
/* 3 */ " # "
/* 4 */ " # "},
             
//   125	]	Closing brace
{/*      012 */
/* 0 */ "## "
/* 1 */ " # "
/* 2 */ " ##"
/* 3 */ " # "
/* 4 */ "## "},

//   126	~	Equivalency sign - tilde
{/*      012 */
/* 0 */ "   "
/* 1 */ "  #"
/* 2 */ "###"
/* 3 */ "#  "
/* 4 */ "   "},
};

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
