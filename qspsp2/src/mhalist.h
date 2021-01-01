/*
** Mortem's HGE Addon 1.3
** Copyright (C) 2006-2007, Osokin 'elmortem' Makar
** http://elmortem.ru
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

	void clear(void);
	void addFirst(void *obj);
	void addLast(void *obj);
	void remove(void *obj);
	void begin(void);
	void end(void);
	void *next(void);
	void *prev(void);
	void *first(void);
	void *last(void);
	void setCurr(void *obj);
	int count(void) { return _count; };
	void *byNum(int num);

protected:
	mhaListItem *_first, *_last;
	mhaListItem *_curr;
	bool memory;
	int _count;
};

#endif
