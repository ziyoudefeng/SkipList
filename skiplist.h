
#ifndef _SKIPLIST_H
#define _SKIPLIST_H

#define SKIPLIST_MAXLEVEL 8

// 具体数据的类型
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

// 创建一个节点的函数，在创建跳表的函数slCreate和插入元素的函数_slInsert中要使用
skiplistNode *_slCreateNode(unsigned int level, value_type score);
// 创建跳表
skiplist *slCreate(void);
// 删除一个节点，在删除跳表中某个元素时要用，是具体进行释放内存操作的函数
void _slFreeNode(skiplistNode *);
// 释放跳表占用的内存空间
void slFree(skiplist *);
unsigned int _slRandomLevel(void);
// 插入一个元素到跳表中
skiplistNode *slInsert(skiplist *, value_type score);
// 删除跳表中某个元素的内部操作函数
void _slDeleteNode(skiplist *, skipistNode *, skiplistNode **);
// 删除跳表中==某值的元素
int slDelete(skiplist *, value_type);
// 查找元素，找到返回1，否则返回0
int slSearch(skiplist *, value_type);
// 打印
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


