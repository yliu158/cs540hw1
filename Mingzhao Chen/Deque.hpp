#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef Deque_DEFINE
#define Deque_DEFINE(t)\
	struct Deque_##t##_Iterator\
	{\
		t * now;\
		int first;\
		int current;\
		int limit;\
		void inc(Deque_##t##_Iterator *it)\
		{\
			it->current = (it->current + 1);\
		}\
		void dec(Deque_##t##_Iterator *it)\
		{\
			it->current = (it->current - 1);\
		}\
		t & deref(Deque_##t##_Iterator * it)\
		{\
			return it->now[(it->current + it->first) % it->limit];\
		}\
	};\
\
	struct  Deque_##t\
	{\
		char type_name[sizeof("Deque_"#t)];\
		t * data;\
		int first;\
		int last;\
		int LIMIT = 10;\
		bool (*f)(const t &, const t &);\
		void Deque_##t##_expand(Deque_##t * dep)\
		{\
			t * nd = (t *) malloc(sizeof(t)*dep->LIMIT*2);\
			for(int i = 0, j = dep->first; i < dep->LIMIT; i++, j++) {nd[i] = dep->data[j % dep->LIMIT];}\
			free(dep->data);\
			dep->data = nd;\
			dep->LIMIT = dep->LIMIT * 2;\
			int temp = dep->size(dep) - 1;\
			dep->first = 0;\
			dep->last = temp;\
		}\
		void push_back(Deque_##t * dep, t d)\
		{\
			dep->last = (dep->last + 1) % dep->LIMIT;\
			dep->data[dep->last] = d;\
			if(dep->size(dep) > dep->LIMIT/2)\
				{dep->Deque_##t##_expand(dep);}\
		}\
		void push_front(Deque_##t * dep, t d)\
		{\
			dep->first = (dep->first + dep->LIMIT - 1) % dep->LIMIT;\
			dep->data[dep->first] = d;\
			if(dep->size(dep) > dep->LIMIT/2)\
				{dep->Deque_##t##_expand(dep);}\
		}\
		void pop_back(Deque_##t * dep)\
		{\
			dep->last = (dep->last + dep->LIMIT - 1) % dep->LIMIT;\
		}\
		void pop_front(Deque_##t * dep)\
		{\
			dep->first = (dep->first + 1) % dep->LIMIT;\
		}\
		t &front(Deque_##t * dep)\
		{\
			return dep->data[first];\
		}\
		t &back(Deque_##t * dep)\
		{\
			return dep->data[last];\
		}\
		size_t size(Deque_##t *dep)\
		{\
			size_t a = (last + dep->LIMIT - first + 1) % dep->LIMIT;\
			return a;\
		}\
		bool empty(Deque_##t *dep)\
		{\
			if(dep->size(dep) == 0)\
			return true;\
			else\
			return false;\
		}\
		Deque_##t##_Iterator begin(Deque_##t * dep)\
		{\
			Deque_##t##_Iterator it;\
			it.current = 0;\
			it.now = dep->data;\
			it.first = dep->first;\
			it.limit = dep->LIMIT;\
			return it;\
		}\
		Deque_##t##_Iterator end(Deque_##t * dep)\
		{\
			Deque_##t##_Iterator it;\
			it.current = dep->size(dep);\
			it.now = dep->data;\
			it.first = dep->first;\
			it.limit = dep->LIMIT;\
			return it;\
		}\
		t &at(Deque_##t *ap, int i) {\
        	return ap->data[(i + ap->first) % ap->LIMIT];\
    	}\
    	void clear(Deque_##t *dep)\
    	{;}\
    	void dtor(Deque_##t *dep)\
    	{ free(dep->data);}\
    	void sort(Deque_##t * dep, Deque_##t##_Iterator it, Deque_##t##_Iterator it2)\
    	{\
    		t data[it2.current - it.current];\
    		Deque_##t##_Iterator it1 = it;\
    		int i = 0;\
    		for (;it1.current < it2.current; it1.current++) {\
            		data[i] = it1.deref(&it1);\
            		i++;\
            }\
            it1 = it;\
            i = 0;\
            std::sort(data, data + it2.current - it.current, dep->f);\
    		for (;it1.current < it2.current; it1.current++) {\
            		it1.deref(&it1) = data[i];\
            		i++;\
            }\
    	}\
	};\
\
	void Deque_##t##_ctor(Deque_##t *dep, bool (*fun)(const t  &o1, const t &o2))\
	{\
		dep->f = fun;\
		strcpy(dep->type_name, "Deque_"#t);\
		dep->first = 0;\
		dep->last = -1;\
		dep->data = (t *)malloc(sizeof(t)*dep->LIMIT);\
	}\
\
	bool Deque_##t##_Iterator_equal(Deque_##t##_Iterator it1, Deque_##t##_Iterator it2)\
	{\
		if(it1.now == it2.now && it1.current == it2.current) return true;\
		else return false;\
	}\
	bool Deque_##t##_equal(Deque_##t &deq1, Deque_##t & deq2)\
	{\
		if(deq1.size(&deq1) != deq2.size(&deq2)) return false;\
		else\
			{\
				Deque_##t##_Iterator it1 = deq1.begin(&deq1);\
				Deque_##t##_Iterator it2 = deq2.begin(&deq2);\
				for (;\
         			!Deque_##t##_Iterator_equal(it1, deq1.end(&deq1)); it1.inc(&it1), it2.inc(&it2)) {\
            			if(deq1.f(it1.deref(&it1),it2.deref(&it2)) || deq2.f(it2.deref(&it2),it1.deref(&it1))) return false;\
        		}\
			}\
		return true;\
	}
#endif																																			
