/*
** Mortem's HGE Addon 1.3
** Copyright (C) 2006-2007, Osokin 'elmortem' Makar
** http://elmortem.mirgames.ru
**
** List.
*/

#include <mhalist.h>

mhaList::mhaList(bool _memory) {
	_first = 0;
	_last = 0;
	_curr = 0;
	memory = _memory;

	_count = 0;
}

mhaList::~mhaList(void) {
	clear();
}

void mhaList::clear(void) {
	mhaListItem *item = _first, *next;

	while(item) {
		next = item->next;
		if(memory) delete item->obj;
		delete item;
		item = next;
	}
	_first = 0;
	_last = 0;
	_curr = 0;

	_count = 0;
}

void mhaList::addFirst(void *obj) {
	mhaListItem *item = new mhaListItem();
	item->obj = obj;

	item->next = _first;
	if(_first) _first->prev = item;
	_first = item;
	if(!_last) _last = _first;

	_count++;
}

void mhaList::addLast(void *obj) {
	mhaListItem *item = new mhaListItem();
	item->obj = obj;

	item->prev = _last;
	if(_last) _last->next = item;
	_last = item;
	if(!_first) _first = _last;

	_count++;
}

void mhaList::remove(void *obj) {
	mhaListItem *item = _first;

	while(item) {
		if(item->obj == obj) {
			if(item->prev) item->prev->next = item->next;
			else _first = item->next;
			if(item->next) item->next->prev = item->prev;
			else _last = item->prev;

			if(memory) delete item->obj;
			delete item;

			_count--;
			return;
		}
		item = item->next;
	}
}

void mhaList::begin(void) {
	_curr = _first;
}

void mhaList::end(void) {
	_curr = _last;
}

void *mhaList::next(void) {
	if(!_curr) return 0;
	void *obj = _curr->obj;
	_curr = _curr->next;
	return obj;
}

void *mhaList::prev(void) {
	if(!_curr) return 0;
	void *obj = _curr->obj;
	_curr = _curr->prev;
	return obj;
}

void *mhaList::first(void) {
	if(!_first) return 0;
	return _first->obj;
}

void *mhaList::last(void) {
	if(!_last) return 0;
	return _last->obj;
}

void mhaList::setCurr(void *obj) {
	mhaListItem *item = _first;

	while(item) {
		if(item->obj == obj) {
			_curr = item;
			return;
		}
		item = item->next;
	}
}

void *mhaList::byNum(int num) {
	mhaListItem *item = _first;
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
