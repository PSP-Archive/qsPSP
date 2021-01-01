/*
** Mortem's HGE Addon 1.3
** Copyright (C) 2006-2007, Osokin 'elmortem' Makar
** http://elmortem.mirgames.ru
**
** List.
*/

#include <mhalist.h>

mhaList::mhaList(bool _memory) {
	first = 0;
	last = 0;
	curr = 0;
	memory = _memory;

	count = 0;
}

mhaList::~mhaList(void) {
	Clear();
}

void mhaList::Clear(void) {
	mhaListItem *item = first, *next;

	while(item) {
		next = item->next;
		if(memory) delete item->obj;
		delete item;
		item = next;
	}
	first = 0;
	last = 0;
	curr = 0;

	count = 0;
}

void mhaList::AddFirst(void *obj) {
	mhaListItem *item = new mhaListItem();
	item->obj = obj;

	item->next = first;
	if(first) first->prev = item;
	first = item;
	if(!last) last = first;

	count++;
}

void mhaList::AddLast(void *obj) {
	mhaListItem *item = new mhaListItem();
	item->obj = obj;

	item->prev = last;
	if(last) last->next = item;
	last = item;
	if(!first) first = last;

	count++;
}

void mhaList::Remove(void *obj) {
	mhaListItem *item = first;

	while(item) {
		if(item->obj == obj) {
			if(item->prev) item->prev->next = item->next;
			else first = item->next;
			if(item->next) item->next->prev = item->prev;
			else last = item->prev;

			if(memory) delete item->obj;
			delete item;

			count--;
			return;
		}
		item = item->next;
	}
}

void mhaList::Begin(void) {
	curr = first;
}

void mhaList::End(void) {
	curr = last;
}

void *mhaList::Next(void) {
	if(!curr) return 0;
	void *obj = curr->obj;
	curr = curr->next;
	return obj;
}

void *mhaList::Prev(void) {
	if(!curr) return 0;
	void *obj = curr->obj;
	curr = curr->prev;
	return obj;
}

void *mhaList::First(void) {
	if(!first) return 0;
	return first->obj;
}

void *mhaList::Last(void) {
	if(!last) return 0;
	return last->obj;
}

void mhaList::SetCurr(void *obj) {
	mhaListItem *item = first;

	while(item) {
		if(item->obj == obj) {
			curr = item;
			return;
		}
		item = item->next;
	}
}

void *mhaList::ByNum(int num) {
	mhaListItem *item = first;
	int i = 0;

	while(item) {
		if(i == num) {
			return item->obj;
		}
		item = item->next;
		i++;
	}
	return 0;
}
