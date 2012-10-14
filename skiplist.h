
#ifndef _SKIPLIST_H
#define _SKIPLIST_H

#define SKIPLIST_MAXLEVEL 8

typedef value_type double;

typedef struct _skiplistNode {
	value_type score;
	struct _skiplistNode *backward;
	struct _skiplistLevel {
		struct _skiplistNode *forward;
	} level[];
} skiplistNode;

typedef struct _skiplist {
	struct _skiplistNode *header, *tail;
	unsigned long length;
	unsigned int level;
} skiplist;

skiplistNode *slCreateNOde(unsigned int level, value_type score);
skiplist *slCreate(void);
void _slFreeNode(skiplistNode *);
void slFree(skiplist *);
unsigned int slRandomLevel(void);
skiplistNode *_slInsert(skiplist *, value_type score);
void _slDeleteNode(skiplist *, skipistNode *, skiplistNode **);
int slDelete(skiplist *, value_type);
int slSearch(skiplist *, value_type);
void slPrint(skiplist *);

#endif

#ifndef _INCLUDE_STDLIB.H
#define _INCLUDE_STDLIB.H
#include <stdlib.h>
#endif
#ifndef _INCLUDE_STDIO.H
#define _INCLUDE_STDIO.H
#include <stdio.h>
#endif

