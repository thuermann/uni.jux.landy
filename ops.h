#ifndef OPS_H
#define OPS_H

/*
 *  Define names for a couple of operators.  These values must be
 *  different from all token values of other operators defined in
 *  parse.tab.h to ensure unique values which are used as expr types.
 */
enum {
	EQ = 1024,
	NE,
	LE,
	GE,
	POSTINC,
	POSTDEC,
	PREINC,
	PREDEC,
	ASSIGNPLUS,
	ASSIGNMINUS,
	ASSIGNMULT,
	ASSIGNDIV,
};

#endif
