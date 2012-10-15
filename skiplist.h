
#ifndef _SKIPLIST_H
#define _SKIPLIST_H

#define SKIPLIST_MAXLEVEL 8

// 具体数据的类型
typedef double value_type;

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
	int level; // 本来是想定义为unsigned int类型，但后面有很多for (int i = (int)level - 1; i >= 0; --i)这样的for循环。太罗嗦，所以直接定义为int类型
} skiplist;

// 创建一个节点的函数，在创建跳表的函数slCreate和插入元素的函数_slInsert中要使用
static skiplistNode *_slCreateNode(int level, value_type score);
// 创建跳表
skiplist *slCreate(void);
// 删除一个节点，在删除跳表中某个元素时要用，是具体进行释放内存操作的函数
static void _slFreeNode(skiplistNode *);
// 释放跳表占用的内存空间
void slFree(skiplist *);
static int _slRandomLevel(void);
// 插入一个元素到跳表中
skiplistNode *slInsert(skiplist *, value_type score);
// 删除跳表中某个元素的内部操作函数
static void _slDeleteNode(skiplist *, skiplistNode *, skiplistNode **);
// 删除跳表中==某值的元素
int slDelete(skiplist *, value_type);
// 查找元素，找到返回1，否则返回0
int slSearch(skiplist *, value_type);
// 打印
void slPrint(skiplist *);

#endif

#ifndef _INCLUDE_STDLIB_H
#define _INCLUDE_STDLIB_H
#include <stdlib.h>
#endif
#ifndef _INCLUDE_STDIO_H
#define _INCLUDE_STDIO_H
#include <stdio.h>
#endif

// 只负责分配内存，和设置score。
static skiplistNode *
_slCreateNode(int level, value_type score)
{
	skiplistNode *sn = malloc(sizeof(*sn) + level * sizeof(struct _skiplistLevel));
	sn->score = score;
	return sn;
}

skiplist *
slCreate(void)
{
//	int j;
	skiplist *sl;
	sl = malloc(sizeof(*sl));
	sl->header = _slCreateNode(SKIPLIST_MAXLEVEL, 0);
	int j;
	for (j = 0; j < SKIPLIST_MAXLEVEL; ++j) {
		sl->header->level[j].forward = NULL;
	}
	sl->header->backward = NULL;
	sl->tail = NULL;
	sl->level = 1;
	sl->length = 0;
	return sl;
}

static void
_slFreeNode(skiplistNode *sn)
{
	free(sn);
}

void
slFree(skiplist *sl)
{
	skiplistNode *node = sl->header->level[0].forward, *next;
	free(sl->header);
	while (node) {
		next = node->level[0].forward;
		_slFreeNode(node);
		node = next;
	}
	free(sl);
}

static int
_slRandomLevel(void)
{
	int level = 1;
	while ((rand() & 0xFFFF) < (0.5 * 0xFFFF)) {
		level += 1;
	}
	return (level < SKIPLIST_MAXLEVEL) ? level : SKIPLIST_MAXLEVEL;
}

skiplistNode *
slInsert(skiplist *sl, value_type score)
{
	skiplistNode *update[SKIPLIST_MAXLEVEL];
	skiplistNode *node;

	node = sl->header;
	int i;
	for (i = sl->level - 1; i >= 0; --i) {
		while (node->level[i].forward && node->level[i].forward->score < score) {
			node = node->level[i].forward;
		}
		update[i] = node;
	}
	int level = _slRandomLevel();
	if (level > sl->level) {
		for (i = sl->level; i < level; ++i) {
			update[i] = sl->header;
		}
		sl->level = level;
	}
	node = _slCreateNode(level, score);
	for (i = 0; i < level; ++i) {
		node->level[i].forward = update[i]->level[i].forward;
		update[i]->level[i].forward = node;
	}

	node->backward = (update[0] == sl->header ? NULL : update[0]);
	if (node->level[0].forward) {
		node->level[0].forward->backward = node;
	} else {
		sl->tail = node;
	}
	++ sl->length;
	return node;
}

static void
_slDeleteNode(skiplist *sl, skiplistNode *x, skiplistNode **update)
{
	int i;
	for (i = 0; i < sl->level; ++i) {
		if (update[i]->level[i].forward == x) {
			update[i]->level[i].forward = x->level[i].forward;
		}
	}
	if (x->level[0].forward) {
		x->level[0].forward->backward = x->backward;
	} else {
		sl->tail = x->backward;
	}
	while (sl->level > 1 && sl->header->level[sl->level-1].forward == NULL) {
		-- sl->level;
	}
	-- sl->length;
}

int
slDelete(skiplist *sl, value_type score)
{
	skiplistNode *update[SKIPLIST_MAXLEVEL], *node;
	node = sl->header;

	int i;
	for (i = sl->level-1; i >= 0; --i) {
		while (node->level[i].forward && node->level[i].forward->score < score) {
			node = node->level[i].forward;
		}
		update[i] = node;
	}
	node = node->level[0].forward;
	if (node && score == node->score) {
		_slDeleteNode(sl, node, update);
		_slFreeNode(node);
		return 1;
	} else {
		return 0;
	}
	return 0;
}

int
slSearch(skiplist *sl, value_type score)
{
	skiplistNode *node;
	node = sl->header;
	int i;
	for (i = sl->level-1; i >= 0; --i) {
		while (node->level[i].forward && node->level[i].forward->score < score) {
			node = node->level[i].forward;
		}
	}
	node = node->level[0].forward;
	// 这里比较运算是否正确？？score是double类型。
	if (node && score == node->score) {
		printf("Found %d\n", (int)node->score);
		return 1;
	} else {
		printf("NOt found %d\n", (int)score);
		return 0;
	}
}

void
slPrint(skiplist *sl)
{
	skiplistNode *node;
	
	int i;
	for (i = 0; i < SKIPLIST_MAXLEVEL; ++i) {
		printf("LEVEL[%d]:", i);
		node = sl->header->level[i].forward;
		while (node) {
			printf("%d -> ", (int)(node->score));
			node = node->level[i].forward;
		}
		printf("NULL\n");
	}
}
