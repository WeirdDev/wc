#ifndef LL_H
#define LL_H

struct ll_entry_s {
	//predošlá a nasledujúca položka
	struct ll_entry_s * prev;
	struct ll_entry_s * next;
	
	//dáta ktoré budeme ukladať
	void * data;
};
typedef struct ll_entry_s ll_entry, *pll_entry;

struct linkedlist_s {
	pll_entry first, last;
};
typedef struct linkedlist_s linkedlist, *plinkedlist;

extern plinkedlist ll_new();
extern void ll_destroy(plinkedlist ll);

extern int ll_count(plinkedlist ll);
extern void * ll_get(plinkedlist ll, int index);
extern int ll_set(plinkedlist ll, int index, void * data);

extern void ll_add(plinkedlist ll, void * data);
extern int ll_remove(plinkedlist ll, int index);

extern void ll_push(plinkedlist ll, void * data);
extern void * ll_pop(plinkedlist ll);

#endif /* LL_H */
