#include "fonts.h"
#include "macros.h"

#ifndef M_PC
#include <avr/pgmspace.h>
#endif

static mubyte TheFont14[] M_FLASH = {
	/** Character code: 32; character name: <SPACE> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	/** Character code: 33; character name: <EXCLAMATION MARK> */
	0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x10, 0x10, 0x00, 0x00,
	/** Character code: 34; character name: <QUOTATION MARK> */
	0x00, 0x00, 0x6c, 0x24, 0x24, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	/** Character code: 35; character name: <NUMBER SIGN> */
	0x00, 0x00, 0x00, 0x12, 0x12, 0x7e, 0x24, 0x24, 0x24, 0x7e, 0x48, 0x48, 0x00, 0x00,
	/** Character code: 36; character name: <DOLLAR SIGN> */
	0x00, 0x00, 0x00, 0x08, 0x3c, 0x4a, 0x48, 0x28, 0x1c, 0x4a, 0x4a, 0x3c, 0x08, 0x00,
	/** Character code: 37; character name: <PERCENT SIGN> */
	0x00, 0x00, 0x00, 0x32, 0x4a, 0x4c, 0x38, 0x10, 0x1c, 0x32, 0x52, 0x4c, 0x00, 0x00,
	/** Character code: 38; character name: <AMPERSAND> */
	0x00, 0x00, 0x00, 0x18, 0x24, 0x24, 0x18, 0x32, 0x4a, 0x44, 0x4c, 0x32, 0x00, 0x00,
	/** Character code: 39; character name: <APOSTROPHE> */
	0x00, 0x00, 0x30, 0x10, 0x10, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	/** Character code: 40; character name: <LEFT PARENTHESIS> */
	0x00, 0x00, 0x04, 0x08, 0x08, 0x10, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x04, 0x00,
	/** Character code: 41; character name: <RIGHT PARENTHESIS> */
	0x00, 0x00, 0x40, 0x20, 0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x20, 0x20, 0x40, 0x00,
	/** Character code: 42; character name: <ASTERISK> */
	0x00, 0x00, 0x00, 0x00, 0x10, 0x54, 0x38, 0x10, 0x38, 0x54, 0x10, 0x00, 0x00, 0x00,
	/** Character code: 43; character name: <PLUS SIGN> */
	0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x7c, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00,
	/** Character code: 44; character name: <COMMA> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x08, 0x08, 0x10,
	/** Character code: 45; character name: <HYPHEN, MINUS SIGN> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	/** Character code: 46; character name: <FULL STOP> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00,
	/** Character code: 47; character name: <SOLIDUS> */
	0x00, 0x00, 0x00, 0x04, 0x04, 0x08, 0x08, 0x10, 0x20, 0x20, 0x40, 0x40, 0x00, 0x00,
	/** Character code: 48; character name: <DIGIT ZERO> */
	0x00, 0x00, 0x00, 0x18, 0x24, 0x42, 0x42, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00, 0x00,
	/** Character code: 49; character name: <DIGIT ONE> */
	0x00, 0x00, 0x00, 0x10, 0x30, 0x50, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7c, 0x00, 0x00,
	/** Character code: 50; character name: <DIGIT TWO> */
	0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x02, 0x0c, 0x10, 0x20, 0x40, 0x7e, 0x00, 0x00,
	/** Character code: 51; character name: <DIGIT THREE> */
	0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x02, 0x1c, 0x02, 0x42, 0x42, 0x3c, 0x00, 0x00,
	/** Character code: 52; character name: <DIGIT FOUR> */
	0x00, 0x00, 0x00, 0x04, 0x0c, 0x14, 0x14, 0x24, 0x44, 0x7e, 0x04, 0x04, 0x00, 0x00,
	/** Character code: 53; character name: <DIGIT FIVE> */
	0x00, 0x00, 0x00, 0x7e, 0x40, 0x40, 0x7c, 0x02, 0x02, 0x02, 0x42, 0x3c, 0x00, 0x00,
	/** Character code: 54; character name: <DIGIT SIX> */
	0x00, 0x00, 0x00, 0x3c, 0x42, 0x40, 0x40, 0x7c, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00,
	/** Character code: 55; character name: <DIGIT SEVEN> */
	0x00, 0x00, 0x00, 0x7e, 0x02, 0x04, 0x04, 0x08, 0x08, 0x10, 0x10, 0x10, 0x00, 0x00,
	/** Character code: 56; character name: <DIGIT EIGHT> */
	0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x3c, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00,
	/** Character code: 57; character name: <DIGIT NINE> */
	0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x3e, 0x02, 0x42, 0x42, 0x3c, 0x00, 0x00,
	/** Character code: 58; character name: <COLON> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00,
	/** Character code: 59; character name: <SEMICOLON> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x08, 0x08, 0x10,
	/** Character code: 60; character name: <LESS-THAN SIGN> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x08, 0x10, 0x20, 0x10, 0x08, 0x04, 0x00, 0x00,
	/** Character code: 61; character name: <EQUALS SIGN> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00,
	/** Character code: 62; character name: <GREATER-THAN SIGN> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x08, 0x04, 0x08, 0x10, 0x20, 0x00, 0x00,
	/** Character code: 63; character name: <QUESTION MARK> */
	0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x04, 0x08, 0x08, 0x00, 0x08, 0x08, 0x00, 0x00,
	/** Character code: 64; character name: <COMMERCIAL AT> */
	0x00, 0x00, 0x00, 0x1c, 0x22, 0x4e, 0x52, 0x52, 0x52, 0x4e, 0x20, 0x1e, 0x00, 0x00,
	/** Character code: 65; character name: <CAPITAL LETTER A> */
	0x00, 0x00, 0x00, 0x18, 0x24, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00,
	/** Character code: 66; character name: <CAPITAL LETTER B> */
	0x00, 0x00, 0x00, 0x7c, 0x22, 0x22, 0x22, 0x3c, 0x22, 0x22, 0x22, 0x7c, 0x00, 0x00,
	/** Character code: 67; character name: <CAPITAL LETTER C> */
	0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x40, 0x40, 0x40, 0x42, 0x42, 0x3c, 0x00, 0x00,
	/** Character code: 68; character name: <CAPITAL LETTER D> */
	0x00, 0x00, 0x00, 0x7c, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x7c, 0x00, 0x00,
	/** Character code: 69; character name: <CAPITAL LETTER E> */
	0x00, 0x00, 0x00, 0x7e, 0x40, 0x40, 0x40, 0x7c, 0x40, 0x40, 0x40, 0x7e, 0x00, 0x00,
	/** Character code: 70; character name: <CAPITAL LETTER F> */
	0x00, 0x00, 0x00, 0x7e, 0x40, 0x40, 0x40, 0x7c, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00,
	/** Character code: 71; character name: <CAPITAL LETTER G> */
	0x00, 0x00, 0x00, 0x3c, 0x42, 0x40, 0x40, 0x4e, 0x42, 0x42, 0x46, 0x3a, 0x00, 0x00,
	/** Character code: 72; character name: <CAPITAL LETTER H> */
	0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00,
	/** Character code: 73; character name: <CAPITAL LETTER I> */
	0x00, 0x00, 0x00, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7c, 0x00, 0x00,
	/** Character code: 74; character name: <CAPITAL LETTER J> */
	0x00, 0x00, 0x00, 0x0e, 0x04, 0x04, 0x04, 0x04, 0x04, 0x44, 0x44, 0x38, 0x00, 0x00,
	/** Character code: 75; character name: <CAPITAL LETTER K> */
	0x00, 0x00, 0x00, 0x42, 0x44, 0x48, 0x50, 0x60, 0x50, 0x48, 0x44, 0x42, 0x00, 0x00,
	/** Character code: 76; character name: <CAPITAL LETTER L> */
	0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x7e, 0x00, 0x00,
	/** Character code: 77; character name: <CAPITAL LETTER M> */
	0x00, 0x00, 0x00, 0x42, 0x66, 0x66, 0x5a, 0x5a, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00,
	/** Character code: 78; character name: <CAPITAL LETTER N> */
	0x00, 0x00, 0x00, 0x62, 0x62, 0x52, 0x52, 0x4a, 0x4a, 0x46, 0x46, 0x42, 0x00, 0x00,
	/** Character code: 79; character name: <CAPITAL LETTER O> */
	0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00,
	/** Character code: 80; character name: <CAPITAL LETTER P> */
	0x00, 0x00, 0x00, 0x7c, 0x42, 0x42, 0x42, 0x7c, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00,
	/** Character code: 81; character name: <CAPITAL LETTER Q> */
	0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x72, 0x4a, 0x46, 0x3c, 0x04, 0x02,
	/** Character code: 82; character name: <CAPITAL LETTER R> */
	0x00, 0x00, 0x00, 0x7c, 0x42, 0x42, 0x42, 0x7c, 0x48, 0x44, 0x42, 0x42, 0x00, 0x00,
	/** Character code: 83; character name: <CAPITAL LETTER S> */
	0x00, 0x00, 0x00, 0x3c, 0x42, 0x40, 0x30, 0x0c, 0x02, 0x42, 0x42, 0x3c, 0x00, 0x00,
	/** Character code: 84; character name: <CAPITAL LETTER T> */
	0x00, 0x00, 0x00, 0xfe, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00,
	/** Character code: 85; character name: <CAPITAL LETTER U> */
	0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00,
	/** Character code: 86; character name: <CAPITAL LETTER V> */
	0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x24, 0x24, 0x24, 0x18, 0x18, 0x18, 0x00, 0x00,
	/** Character code: 87; character name: <CAPITAL LETTER W> */
	0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x5a, 0x5a, 0x66, 0x66, 0x42, 0x00, 0x00,
	/** Character code: 88; character name: <CAPITAL LETTER X> */
	0x00, 0x00, 0x00, 0x42, 0x42, 0x24, 0x24, 0x18, 0x24, 0x24, 0x42, 0x42, 0x00, 0x00,
	/** Character code: 89; character name: <CAPITAL LETTER Y> */
	0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x44, 0x28, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00,
	/** Character code: 90; character name: <CAPITAL LETTER Z> */
	0x00, 0x00, 0x00, 0x7e, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x40, 0x7e, 0x00, 0x00,
	/** Character code: 91; character name: <LEFT SQUARE BRACKET> */
	0x00, 0x00, 0x1c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1c, 0x00,
	/** Character code: 92; character name: <REVERSE SOLIDUS> */
	0x00, 0x00, 0x00, 0x40, 0x40, 0x20, 0x20, 0x10, 0x08, 0x08, 0x04, 0x04, 0x00, 0x00,
	/** Character code: 93; character name: <RIGHT SQUARE BRACKET> */
	0x00, 0x00, 0x70, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x70, 0x00,
	/** Character code: 94; character name: <CIRCUMFLEX ACCENT> */
	0x00, 0x18, 0x24, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	/** Character code: 95; character name: <LOW LINE> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00,
	/** Character code: 96; character name: <GRAVE ACCENT> */
	0x00, 0x00, 0x18, 0x10, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	/** Character code: 97; character name: <SMALL LETTER a> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x0e, 0x32, 0x42, 0x46, 0x3a, 0x00, 0x00,
	/** Character code: 98; character name: <SMALL LETTER b> */
	0x00, 0x00, 0x00, 0x40, 0x40, 0x5c, 0x62, 0x42, 0x42, 0x42, 0x62, 0x5c, 0x00, 0x00,
	/** Character code: 99; character name: <SMALL LETTER c> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x40, 0x40, 0x40, 0x42, 0x3c, 0x00, 0x00,
	/** Character code: 100; character name: <SMALL LETTER d> */
	0x00, 0x00, 0x00, 0x02, 0x02, 0x3a, 0x46, 0x42, 0x42, 0x42, 0x46, 0x3a, 0x00, 0x00,
	/** Character code: 101; character name: <SMALL LETTER e> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x7e, 0x40, 0x42, 0x3c, 0x00, 0x00,
	/** Character code: 102; character name: <SMALL LETTER f> */
	0x00, 0x00, 0x00, 0x0c, 0x10, 0x10, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00,
	/** Character code: 103; character name: <SMALL LETTER g> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3a, 0x44, 0x44, 0x44, 0x38, 0x20, 0x5c, 0x42, 0x3c,
	/** Character code: 104; character name: <SMALL LETTER h> */
	0x00, 0x00, 0x00, 0x40, 0x40, 0x5c, 0x62, 0x42, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00,
	/** Character code: 105; character name: <SMALL LETTER i> */
	0x00, 0x00, 0x10, 0x10, 0x00, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7c, 0x00, 0x00,
	/** Character code: 106; character name: <SMALL LETTER j> */
	0x00, 0x00, 0x04, 0x04, 0x00, 0x0c, 0x04, 0x04, 0x04, 0x04, 0x04, 0x44, 0x44, 0x38,
	/** Character code: 107; character name: <SMALL LETTER k> */
	0x00, 0x00, 0x00, 0x40, 0x40, 0x44, 0x48, 0x50, 0x60, 0x50, 0x48, 0x44, 0x00, 0x00,
	/** Character code: 108; character name: <SMALL LETTER l> */
	0x00, 0x00, 0x00, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7c, 0x00, 0x00,
	/** Character code: 109; character name: <SMALL LETTER m> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x54, 0x54, 0x54, 0x54, 0x54, 0x44, 0x00, 0x00,
	/** Character code: 110; character name: <SMALL LETTER n> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x5c, 0x62, 0x42, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00,
	/** Character code: 111; character name: <SMALL LETTER o> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00,
	/** Character code: 112; character name: <SMALL LETTER p> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x5c, 0x62, 0x42, 0x42, 0x42, 0x62, 0x5c, 0x40, 0x40,
	/** Character code: 113; character name: <SMALL LETTER q> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3a, 0x46, 0x42, 0x42, 0x42, 0x46, 0x3a, 0x02, 0x02,
	/** Character code: 114; character name: <SMALL LETTER r> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x5c, 0x62, 0x42, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00,
	/** Character code: 115; character name: <SMALL LETTER s> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x40, 0x3c, 0x02, 0x42, 0x3c, 0x00, 0x00,
	/** Character code: 116; character name: <SMALL LETTER t> */
	0x00, 0x00, 0x00, 0x10, 0x10, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x12, 0x0c, 0x00, 0x00,
	/** Character code: 117; character name: <SMALL LETTER u> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x46, 0x3a, 0x00, 0x00,
	/** Character code: 118; character name: <SMALL LETTER v> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x28, 0x28, 0x10, 0x10, 0x00, 0x00,
	/** Character code: 119; character name: <SMALL LETTER w> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x54, 0x54, 0x54, 0x54, 0x54, 0x28, 0x00, 0x00,
	/** Character code: 120; character name: <SMALL LETTER x> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x28, 0x10, 0x28, 0x44, 0x44, 0x00, 0x00,
	/** Character code: 121; character name: <SMALL LETTER y> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x46, 0x3a, 0x02, 0x02, 0x3c,
	/** Character code: 122; character name: <SMALL LETTER z> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x04, 0x08, 0x10, 0x20, 0x40, 0x7e, 0x00, 0x00,
	/** Character code: 123; character name: <LEFT CURLY BRACKET> */
	0x00, 0x00, 0x0c, 0x10, 0x10, 0x08, 0x08, 0x10, 0x08, 0x08, 0x10, 0x10, 0x0c, 0x00,
	/** Character code: 124; character name: <VERTICAL LINE> */
	0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
	/** Character code: 125; character name: <RIGHT CURLY BRACKET> */
	0x00, 0x00, 0x60, 0x10, 0x10, 0x20, 0x20, 0x10, 0x20, 0x20, 0x10, 0x10, 0x60, 0x00,
	/** Character code: 126; character name: <TILDE> */
	0x00, 0x00, 0x00, 0x32, 0x4c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	/** Character code: 160; character name: <NO-BREAK SPACE> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	/** Character code: 161; character name: <CYRILLIC CAPITAL LETTER IO> */
	0x24, 0x24, 0x00, 0x7e, 0x40, 0x40, 0x40, 0x7e, 0x40, 0x40, 0x40, 0x7e, 0x00, 0x00,
	/** Character code: 162; character name: <SERBOCROATIAN CYRILLIC CAPITAL LETTER DJE> */
	0x00, 0x00, 0x00, 0xfe, 0x10, 0x10, 0x1c, 0x12, 0x12, 0x12, 0x12, 0x1c, 0x00, 0x00,
	/** Character code: 163; character name: <MACEDONIAN CYRILLIC CAPITAL LETTER GJE> */
	0x0c, 0x30, 0x00, 0x7e, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00,
	/** Character code: 164; character name: <UKRANIAN CYRILLIC CAPITAL LETTER IE> */
	0x00, 0x00, 0x00, 0x1c, 0x22, 0x40, 0x40, 0x7c, 0x40, 0x40, 0x22, 0x1c, 0x00, 0x00,
	/** Character code: 165; character name: <MACEDONIAN CYRILLIC CAPITAL LETTER DZE> */
	0x00, 0x00, 0x00, 0x3c, 0x42, 0x40, 0x30, 0x0c, 0x02, 0x42, 0x42, 0x3c, 0x00, 0x00,
	/** Character code: 166; character name: <CYRILLIC CAPITAL LETTER I> */
	0x00, 0x00, 0x00, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7c, 0x00, 0x00,
	/** Character code: 167; character name: <UKRANIAN CYRILLIC CAPITAL LETTER YI> */
	0x28, 0x28, 0x00, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7c, 0x00, 0x00,
	/** Character code: 168; character name: <CYRILLIC CAPITAL LETTER JE> */
	0x00, 0x00, 0x00, 0x0e, 0x04, 0x04, 0x04, 0x04, 0x04, 0x44, 0x44, 0x38, 0x00, 0x00,
	/** Character code: 169; character name: <CYRILLIC CAPITAL LETTER LJE> */
	0x00, 0x00, 0x00, 0x78, 0x48, 0x48, 0x4c, 0x4a, 0x4a, 0x4a, 0x4a, 0x8c, 0x00, 0x00,
	/** Character code: 170; character name: <CYRILLIC CAPITAL LETTER NJE> */
	0x00, 0x00, 0x00, 0x48, 0x48, 0x48, 0x4c, 0x7a, 0x4a, 0x4a, 0x4a, 0x4c, 0x00, 0x00,
	/** Character code: 171; character name: <SERBOCROATIAN CYRILLIC CAPITAL LETTER CHJE> */
	0x00, 0x00, 0x00, 0x7c, 0x10, 0x10, 0x1c, 0x12, 0x12, 0x12, 0x12, 0x12, 0x00, 0x00,
	/** Character code: 172; character name: <MACEDONIAN CYRILLIC CAPITAL LETTER KJE> */
	0x0c, 0x30, 0x00, 0x42, 0x44, 0x48, 0x50, 0x60, 0x50, 0x48, 0x44, 0x42, 0x00, 0x00,
	/** Character code: 173; character name: <SOFT HYPHEN> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	/** Character code: 174; character name: <BIELORUSSIAN CYRILLIC CAPITAL LETTER SHORT U> */
	0x24, 0x18, 0x00, 0x42, 0x42, 0x24, 0x24, 0x18, 0x08, 0x10, 0x10, 0x60, 0x00, 0x00,
	/** Character code: 175; character name: <CYRILLIC CAPITAL LETTER DZE> */
	0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x44, 0x44, 0x7c, 0x10, 0x10, 0x10, 0x00, 0x00,
	/** Character code: 176; character name: <CYRILLIC CAPITAL LETTER A> */
	0x00, 0x00, 0x00, 0x18, 0x18, 0x24, 0x24, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00,
	/** Character code: 177; character name: <CYRILLIC CAPITAL LETTER BE> */
	0x00, 0x00, 0x00, 0x7c, 0x40, 0x40, 0x40, 0x7c, 0x42, 0x42, 0x42, 0x7c, 0x00, 0x00,
	/** Character code: 178; character name: <CYRILLIC CAPITAL LETTER VE> */
	0x00, 0x00, 0x00, 0x7c, 0x42, 0x42, 0x42, 0x7c, 0x42, 0x42, 0x42, 0x7c, 0x00, 0x00,
	/** Character code: 179; character name: <CYRILLIC CAPITAL LETTER GHE> */
	0x00, 0x00, 0x00, 0x7e, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00,
	/** Character code: 180; character name: <CYRILLIC CAPITAL LETTER DE> */
	0x00, 0x00, 0x00, 0x1c, 0x14, 0x14, 0x24, 0x24, 0x24, 0x44, 0x44, 0x7c, 0x82, 0x00,
	/** Character code: 181; character name: <CYRILLIC CAPITAL LETTER IE> */
	0x00, 0x00, 0x00, 0x7e, 0x40, 0x40, 0x40, 0x7c, 0x40, 0x40, 0x40, 0x7e, 0x00, 0x00,
	/** Character code: 182; character name: <CYRILLIC CAPITAL LETTER ZHE> */
	0x00, 0x00, 0x00, 0x92, 0x92, 0x54, 0x54, 0x38, 0x54, 0x54, 0x92, 0x92, 0x00, 0x00,
	/** Character code: 183; character name: <CYRILLIC CAPITAL LETTER ZE> */
	0x00, 0x00, 0x00, 0x3c, 0x42, 0x02, 0x02, 0x1c, 0x02, 0x02, 0x42, 0x3c, 0x00, 0x00,
	/** Character code: 184; character name: <CYRILLIC CAPITAL LETTER I> */
	0x00, 0x00, 0x00, 0x46, 0x46, 0x4a, 0x4a, 0x52, 0x52, 0x62, 0x62, 0x42, 0x00, 0x00,
	/** Character code: 185; character name: <CYRILLIC CAPITAL LETTER SHORT I> */
	0x24, 0x18, 0x00, 0x46, 0x46, 0x4a, 0x4a, 0x52, 0x52, 0x62, 0x62, 0x42, 0x00, 0x00,
	/** Character code: 186; character name: <CYRILLIC CAPITAL LETTER KA> */
	0x00, 0x00, 0x00, 0x42, 0x44, 0x48, 0x50, 0x60, 0x50, 0x48, 0x44, 0x42, 0x00, 0x00,
	/** Character code: 187; character name: <CYRILLIC CAPITAL LETTER EL> */
	0x00, 0x00, 0x00, 0x1e, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x62, 0x00, 0x00,
	/** Character code: 188; character name: <CYRILLIC CAPITAL LETTER EM> */
	0x00, 0x00, 0x00, 0x42, 0x66, 0x66, 0x5a, 0x5a, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00,
	/** Character code: 189; character name: <CYRILLIC CAPITAL LETTER EN> */
	0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00,
	/** Character code: 190; character name: <CYRILLIC CAPITAL LETTER O> */
	0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00,
	/** Character code: 191; character name: <CYRILLIC CAPITAL LETTER PE> */
	0x00, 0x00, 0x00, 0x7e, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00,
	/** Character code: 192; character name: <CYRILLIC CAPITAL LETTER ER> */
	0x00, 0x00, 0x00, 0x7c, 0x42, 0x42, 0x42, 0x7c, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00,
	/** Character code: 193; character name: <CYRILLIC CAPITAL LETTER ES> */
	0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x40, 0x40, 0x40, 0x42, 0x42, 0x3c, 0x00, 0x00,
	/** Character code: 194; character name: <CYRILLIC CAPITAL LETTER TE> */
	0x00, 0x00, 0x00, 0xfe, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00,
	/** Character code: 195; character name: <CYRILLIC CAPITAL LETTER U> */
	0x00, 0x00, 0x00, 0x42, 0x42, 0x24, 0x24, 0x18, 0x08, 0x10, 0x10, 0x60, 0x00, 0x00,
	/** Character code: 196; character name: <CYRILLIC CAPITAL LETTER EF> */
	0x00, 0x00, 0x00, 0x10, 0x7c, 0x92, 0x92, 0x92, 0x92, 0x7c, 0x10, 0x10, 0x00, 0x00,
	/** Character code: 197; character name: <CYRILLIC CAPITAL LETTER HA> */
	0x00, 0x00, 0x00, 0x42, 0x42, 0x24, 0x24, 0x18, 0x24, 0x24, 0x42, 0x42, 0x00, 0x00,
	/** Character code: 198; character name: <CYRILLIC CAPITAL LETTER TSE> */
	0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x7e, 0x02, 0x04,
	/** Character code: 199; character name: <CYRILLIC CAPITAL LETTER CHE> */
	0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x3e, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00,
	/** Character code: 200; character name: <CYRILLIC CAPITAL LETTER SHA> */
	0x00, 0x00, 0x00, 0x4a, 0x4a, 0x4a, 0x4a, 0x4a, 0x4a, 0x4a, 0x4a, 0x7e, 0x00, 0x00,
	/** Character code: 201; character name: <CYRILLIC CAPITAL LETTER SHCHA> */
	0x00, 0x00, 0x00, 0x4a, 0x4a, 0x4a, 0x4a, 0x4a, 0x4a, 0x4a, 0x4a, 0x7e, 0x02, 0x04,
	/** Character code: 202; character name: <CYRILLIC CAPITAL HARD SIGN> */
	0x00, 0x00, 0x00, 0xe0, 0x20, 0x20, 0x20, 0x3c, 0x22, 0x22, 0x22, 0x3c, 0x00, 0x00,
	/** Character code: 203; character name: <CYRILLIC CAPITAL LETTER YERU> */
	0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x72, 0x4a, 0x4a, 0x4a, 0x72, 0x00, 0x00,
	/** Character code: 204; character name: <CYRILLIC CAPITAL SOFT SIGN> */
	0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x7c, 0x42, 0x42, 0x42, 0x7c, 0x00, 0x00,
	/** Character code: 205; character name: <CYRILLIC CAPITAL LETTER E> */
	0x00, 0x00, 0x00, 0x38, 0x44, 0x02, 0x02, 0x3e, 0x02, 0x02, 0x44, 0x38, 0x00, 0x00,
	/** Character code: 206; character name: <CYRILLIC CAPITAL LETTER YU> */
	0x00, 0x00, 0x00, 0x4c, 0x52, 0x52, 0x52, 0x72, 0x52, 0x52, 0x52, 0x4c, 0x00, 0x00,
	/** Character code: 207; character name: <CYRILLIC CAPITAL LETTER YA> */
	0x00, 0x00, 0x00, 0x3e, 0x42, 0x42, 0x42, 0x3e, 0x12, 0x22, 0x42, 0x42, 0x00, 0x00,
	/** Character code: 208; character name: <CYRILLIC SMALL LETTER A> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x0e, 0x32, 0x42, 0x46, 0x3a, 0x00, 0x00,
	/** Character code: 209; character name: <CYRILLIC SMALL LETTER BE> */
	0x00, 0x00, 0x00, 0x3e, 0x40, 0x40, 0x7c, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00,
	/** Character code: 210; character name: <CYRILLIC SMALL LETTER VE> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x42, 0x42, 0x7c, 0x42, 0x42, 0x7c, 0x00, 0x00,
	/** Character code: 211; character name: <CYRILLIC SMALL LETTER GHE> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00,
	/** Character code: 212; character name: <CYRILLIC SMALL LETTER DE> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x14, 0x24, 0x24, 0x44, 0x44, 0x7c, 0x82, 0x00,
	/** Character code: 213; character name: <CYRILLIC SMALL LETTER IE> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x7e, 0x40, 0x42, 0x3c, 0x00, 0x00,
	/** Character code: 214; character name: <CYRILLIC SMALL LETTER ZHE> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x54, 0x54, 0x54, 0x38, 0x54, 0x54, 0x54, 0x00, 0x00,
	/** Character code: 215; character name: <CYRILLIC SMALL LETTER ZE> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x02, 0x0c, 0x02, 0x42, 0x3c, 0x00, 0x00,
	/** Character code: 216; character name: <CYRILLIC SMALL LETTER I> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x46, 0x4a, 0x4a, 0x52, 0x52, 0x62, 0x00, 0x00,
	/** Character code: 217; character name: <CYRILLIC SMALL LETTER SHORT I> */
	0x00, 0x00, 0x24, 0x18, 0x00, 0x46, 0x46, 0x4a, 0x4a, 0x52, 0x52, 0x62, 0x00, 0x00,
	/** Character code: 218; character name: <CYRILLIC SMALL LETTER KA> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x48, 0x50, 0x70, 0x48, 0x44, 0x42, 0x00, 0x00,
	/** Character code: 219; character name: <CYRILLIC SMALL LETTER EL> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x12, 0x12, 0x12, 0x12, 0x12, 0x62, 0x00, 0x00,
	/** Character code: 220; character name: <CYRILLIC SMALL LETTER EM> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x66, 0x66, 0x5a, 0x5a, 0x42, 0x42, 0x00, 0x00,
	/** Character code: 221; character name: <CYRILLIC SMALL LETTER EN> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x42, 0x00, 0x00,
	/** Character code: 222; character name: <CYRILLIC SMALL LETTER O> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00,
	/** Character code: 223; character name: <CYRILLIC SMALL LETTER PE> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00,
	/** Character code: 224; character name: <CYRILLIC SMALL LETTER ER> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x5c, 0x62, 0x42, 0x42, 0x42, 0x62, 0x5c, 0x40, 0x40,
	/** Character code: 225; character name: <CYRILLIC SMALL LETTER ES> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x40, 0x40, 0x40, 0x42, 0x3c, 0x00, 0x00,
	/** Character code: 226; character name: <CYRILLIC SMALL LETTER TE> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00,
	/** Character code: 227; character name: <CYRILLIC SMALL LETTER U> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x24, 0x24, 0x18, 0x08, 0x10, 0x10, 0x60,
	/** Character code: 228; character name: <CYRILLIC SMALL LETTER EF> */
	0x00, 0x00, 0x00, 0x10, 0x10, 0x38, 0x54, 0x54, 0x54, 0x54, 0x54, 0x38, 0x10, 0x10,
	/** Character code: 229; character name: <CYRILLIC SMALL LETTER HA> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x24, 0x18, 0x24, 0x42, 0x42, 0x00, 0x00,
	/** Character code: 230; character name: <CYRILLIC SMALL LETTER TSE> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x7e, 0x04, 0x08,
	/** Character code: 231; character name: <CYRILLIC SMALL LETTER CHE> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x3e, 0x02, 0x02, 0x02, 0x00, 0x00,
	/** Character code: 232; character name: <CYRILLIC SMALL LETTER SHA> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x7c, 0x00, 0x00,
	/** Character code: 233; character name: <CYRILLIC SMALL LETTER SHCHA> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x7e, 0x04, 0x08,
	/** Character code: 234; character name: <CYRILLIC SMALL HARD SIGN> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x20, 0x20, 0x3c, 0x22, 0x22, 0x3c, 0x00, 0x00,
	/** Character code: 235; character name: <CYRILLIC SMALL LETTER YERU> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x64, 0x54, 0x54, 0x64, 0x00, 0x00,
	/** Character code: 236; character name: <CYRILLIC SMALL SOFT SIGN> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x7c, 0x42, 0x42, 0x7c, 0x00, 0x00,
	/** Character code: 237; character name: <CYRILLIC SMALL LETTER E> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x02, 0x1e, 0x02, 0x44, 0x38, 0x00, 0x00,
	/** Character code: 238; character name: <CYRILLIC SMALL LETTER YU> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x4c, 0x52, 0x52, 0x72, 0x52, 0x52, 0x4c, 0x00, 0x00,
	/** Character code: 239; character name: <CYRILLIC SMALL LETTER YA> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x42, 0x42, 0x3e, 0x12, 0x22, 0x42, 0x00, 0x00,
	/** Character code: 240; character name: <NUMBER ACRONYM> */
	0x0c, 0x12, 0x0c, 0x62, 0x62, 0x52, 0x52, 0x4a, 0x4a, 0x46, 0x46, 0x42, 0x00, 0x00,
	/** Character code: 241; character name: <CYRILLIC SMALL IO> */
	0x00, 0x00, 0x24, 0x24, 0x00, 0x3c, 0x42, 0x42, 0x7e, 0x40, 0x42, 0x3c, 0x00, 0x00,
	/** Character code: 242; character name: <SERBOCROATIAN CYRILLIC SMALL LETTER DJE> */
	0x00, 0x00, 0x00, 0x20, 0x20, 0x78, 0x20, 0x3c, 0x22, 0x22, 0x22, 0x3c, 0x00, 0x00,
	/** Character code: 243; character name: <MACEDONIAN CYRILLIC SMALL LETTER GJE> */
	0x00, 0x00, 0x0c, 0x30, 0x00, 0x7e, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00,
	/** Character code: 244; character name: <UKRANIAN CYRILLIC SMALL LETTER IE> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x22, 0x40, 0x78, 0x40, 0x22, 0x1c, 0x00, 0x00,
	/** Character code: 245; character name: <MACEDONIAN CYRILLIC SMALL LETTER DZE> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x40, 0x3c, 0x02, 0x42, 0x3c, 0x00, 0x00,
	/** Character code: 246; character name: <CYRILLIC SMALL LETTER I> */
	0x00, 0x00, 0x10, 0x10, 0x00, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7c, 0x00, 0x00,
	/** Character code: 247; character name: <UKRANIAN CYRILLIC SMALL LETTER YI> */
	0x00, 0x00, 0x48, 0x48, 0x00, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7c, 0x00, 0x00,
	/** Character code: 248; character name: <CYRILLIC SMALL LETTER JE> */
	0x00, 0x00, 0x04, 0x04, 0x00, 0x0c, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x44, 0x38,
	/** Character code: 249; character name: <CYRILLIC SMALL LETTER LJE> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x28, 0x28, 0x2c, 0x2a, 0x2a, 0x4c, 0x00, 0x00,
	/** Character code: 250; character name: <CYRILLIC SMALL LETTER NJE> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x50, 0x50, 0x7c, 0x52, 0x52, 0x5c, 0x00, 0x00,
	/** Character code: 251; character name: <SERBOCROATIAN CYRILLIC SMALL LETTER CHJE> */
	0x00, 0x00, 0x00, 0x20, 0x20, 0x78, 0x20, 0x2c, 0x32, 0x22, 0x22, 0x22, 0x00, 0x00,
	/** Character code: 252; character name: <MACEDONIAN CYRILLIC SMALL LETTER KJE> */
	0x00, 0x00, 0x0c, 0x30, 0x00, 0x46, 0x48, 0x50, 0x70, 0x48, 0x44, 0x42, 0x00, 0x00,
	/** Character code: 253; character name: <PARAGRAPH SIGN> */
	0x00, 0x00, 0x38, 0x44, 0x40, 0x38, 0x44, 0x44, 0x38, 0x04, 0x44, 0x38, 0x00, 0x00,
	/** Character code: 254; character name: <BIELORUSSIAN CYRILLIC SMALL LETTER SHORT U> */
	0x00, 0x00, 0x24, 0x18, 0x00, 0x42, 0x42, 0x24, 0x24, 0x18, 0x08, 0x10, 0x10, 0x60,
	/** Character code: 255; character name: <CYRILLIC SMALL LETTER DZE> */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x44, 0x44, 0x7c, 0x10, 0x10, 0x10,
};

mubyte get_font14_byte (mubyte aChar, mubyte aLine) {
    muword index;

    m_return_val_if_fail (aChar >= 32, 0);
    m_return_val_if_fail (aChar <= 126, 0 || (aChar >= 160 && aChar <= 255));
    m_return_val_if_fail (aLine < 14, 0);

    if (aChar < 128) {
        aChar -= 32;
    }
    else {
        aChar -= (160 - (127 - 32));
    }

    index = ((muword)aChar)*14 + aLine;

#ifdef M_PC
    return TheFont14[index];
#else
    return pgm_read_byte (&TheFont14[index]);
#endif
}
