/*
** Mortem's HGE Addon 1.3
** Copyright (C) 2006-2007, Osokin 'elmortem' Makar
** http://elmortem.mirgames.ru
**
** List.
*/

#ifndef _MHALIST_H_
#define _MHALIST_H_

struct mhaListItem {
	void *obj;
	mhaListItem *prev;
	mhaListItem *next;

	mhaListItem(void) {
		obj = 0;
		prev = 0;
		next = 0;
	}
};

class mhaList {
public:
	mhaList(bool _memory = false);
	~mhaList(void);

	void Clear(void);
	void AddFirst(void *obj);
	void AddLast(void *obj);
	void Remove(void *obj);
	void Begin(void);
	void End(void);
	void *Next(void);
	void *Prev(void);
	void *First(void);
	void *Last(void);
	void SetCurr(void *obj);
	int Count(void) { return count; };
	void *ByNum(int num);

protected:
	mhaListItem *first, *last;
	mhaListItem *curr;
	bool memory;
	int count;
};

#endif
