#ifndef TREE_H
#define TREE_H

struct expr {
	int type;
	struct expr *arg1, *arg2;
	struct node *id;
	double value;
};

struct stmt {
	struct stmt *next;
	int type;
	struct stmt *s1, *s2;
	struct expr *e1, *e2, *e3;
};

struct expr *mkexpr(int type, ...);
struct stmt *mkstmt(int type, ...);

#endif
