using namespace std;

#define INIT_SIZE 128

#define Deque_DEFINE(T)\
	struct Deque_##T##_Iterator {\
		int index;\
		int b_size;\
		T *q;\
		T& (*deref)(Deque_##T##_Iterator *);\
		void (*inc)(Deque_##T##_Iterator *);\
		void (*dec)(Deque_##T##_Iterator *);\
	};\
	\
	T& Deque_##T##_Iterator_deref(Deque_##T##_Iterator *it){\
		return it->q[it->index];\
	}\
	void Deque_##T##_Iterator_inc(Deque_##T##_Iterator *it){\
		it->index = (it->index+1)%it->b_size;\
	}\
	void Deque_##T##_Iterator_dec(Deque_##T##_Iterator *it){\
		it->index = (it->index-1+it->b_size)%it->b_size;\
	}\
	bool Deque_##T##_Iterator_equal(Deque_##T##_Iterator it_1, Deque_##T##_Iterator it_2){\
		if (it_1.q != it_2.q) return false;\
		if (it_1.index != it_2.index) return false;\
		return true;\
	}\
	\
	struct Deque_##T {\
		T *data;\
		int buff_size;\
		int front_id;\
		int back_id;\
		bool (*less_than)(const T& a, const T& b);\
		char type_name[sizeof("Deque_"#T)];\
		size_t (*size)(Deque_##T *);\
		bool (*empty)(Deque_##T *);\
		void (*push_back)(Deque_##T *, T);\
		void (*push_front)(Deque_##T *, T);\
		T& (*front)(Deque_##T *);\
		T& (*back)(Deque_##T *);\
		T& (*at)(Deque_##T *, int);\
		void (*pop_front)(Deque_##T *);\
		void (*pop_back)(Deque_##T *);\
		Deque_##T##_Iterator (*begin)(Deque_##T *q);\
		Deque_##T##_Iterator (*end)(Deque_##T *q);\
		void (*clear)(Deque_##T *q);\
		void (*dtor)(Deque_##T *q);\
		void (*sort)(Deque_##T *q, const Deque_##T##_Iterator& b, const Deque_##T##_Iterator& e);\
	};\
	\
	\
	size_t Deque_##T##_size(Deque_##T *q){\
		return (size_t)((q->back_id-q->front_id+q->buff_size-1)%q->buff_size);\
	}\
	\
	\
	bool Deque_##T##_empty(Deque_##T *q){\
		return q->front_id == q->back_id-1;\
	}\
	\
	\
	void Deque_##T##_push_back(Deque_##T *q, T t){\
		int s = (q->back_id - (q->front_id+1) + q->buff_size)%q->buff_size;\
		if (s >= q->buff_size/2) {\
			T *tmp = (T*)malloc(sizeof(T)*q->buff_size*2);\
			\
			for (size_t i = 0; i < s; i++) {\
				tmp[i] = q->data[(i+q->front_id+1)%q->buff_size];\
			}\
			q->buff_size *= 2;\
			q->front_id = q->buff_size-1;\
			q->back_id = s;\
			tmp[q->back_id] = t;\
			q->back_id += 1;\
			\
			free(q->data);\
			q->data = tmp;\
		} else {\
			q->data[q->back_id] = t;\
			q->back_id = (q->back_id+1)%q->buff_size;\
		}\
	}\
	\
	\
	void Deque_##T##_push_front(Deque_##T *q, T t){\
		int s = (q->back_id - (q->front_id+1) + q->buff_size)%q->buff_size;\
		if (s >= q->buff_size/2) {\
			T *tmp = (T*)malloc(sizeof(T)*q->buff_size*2);\
			for (size_t i = 0; i < s; i++) {\
				tmp[i] = q->data[(i+q->front_id+1)%q->buff_size];\
			}\
			q->buff_size *= 2;\
			q->front_id = q->buff_size-1;\
			q->back_id = s;\
			tmp[q->front_id] = t;\
			q->front_id -= 1;\
			\
			free(q->data);\
			q->data = tmp;\
		} else {\
			q->data[q->front_id] = t;\
			q->front_id = (q->front_id-1+q->buff_size)%q->buff_size;\
		}\
	}\
	\
	T& Deque_##T##_at(Deque_##T * q, int i){\
		return q->data[(q->front_id+1+i)%q->buff_size];\
	}\
	\
	T& Deque_##T##_front(Deque_##T *q){\
		return q->data[(q->front_id+1)%q->buff_size];\
	}\
	\
	\
	T& Deque_##T##_back(Deque_##T *q){\
		return q->data[(q->back_id-1+q->buff_size)%q->buff_size];\
	}\
	\
	\
	void Deque_##T##_pop_front(Deque_##T *q){\
		q->front_id = (q->front_id+1)%q->buff_size;\
	}\
	\
	\
	void Deque_##T##_pop_back(Deque_##T *q){\
		q->back_id = (q->back_id-1+q->buff_size)%q->buff_size;\
	}\
	\
	\
	Deque_##T##_Iterator Deque_##T##_begin(Deque_##T *q){\
		Deque_##T##_Iterator it;\
		it.index = (q->front_id+1)%q->buff_size;\
		it.b_size = q->buff_size;\
		it.q = q->data;\
		it.deref = &Deque_##T##_Iterator_deref;\
		it.inc = &Deque_##T##_Iterator_inc;\
		it.dec = &Deque_##T##_Iterator_dec;\
		return it;\
	}\
	Deque_##T##_Iterator Deque_##T##_end(Deque_##T *q){\
		Deque_##T##_Iterator it;\
		it.index = q->back_id;\
		it.b_size = q->buff_size;\
		it.q = q->data;\
		it.deref = &Deque_##T##_Iterator_deref;\
		it.inc = &Deque_##T##_Iterator_inc;\
		it.dec = &Deque_##T##_Iterator_dec;\
		return it;\
	}\
	void Deque_##T##_clear(Deque_##T *q){\
		q->front_id = 0;\
		q->back_id = 1;\
	}\
	\
	void Deque_##T##_dtor(Deque_##T *q){\
		free(q->data);\
	}\
	void quick_sort_##T(int start, int end, T* data) {\
		while (start < end) {\
		}\
	}\
	void Deque_##T##_sort(Deque_##T* q, const Deque_##T##_Iterator& b, const Deque_##T##_Iterator& e){\
		sort(&q->data[b.index], &q->data[e.index], q->less_than);\
		\
	}\
	\
	void Deque_##T##_ctor(Deque_##T* q, bool(*cmp)(const T& a, const T& b)){\
		q->data = (T*)malloc(sizeof(T)*INIT_SIZE);\
		q->less_than = cmp;\
		q->front_id = 0;\
		q->back_id = 1;\
		q->buff_size = INIT_SIZE;\
		strcpy(q->type_name, "Deque_"#T);\
		q->size = &Deque_##T##_size;\
		q->empty = &Deque_##T##_empty;\
		q->push_back = &Deque_##T##_push_back;\
		q->push_front = &Deque_##T##_push_front;\
		q->at = &Deque_##T##_at;\
		q->front = &Deque_##T##_front;\
		q->back = &Deque_##T##_back;\
		q->pop_front = &Deque_##T##_pop_front;\
		q->pop_back = &Deque_##T##_pop_back;\
		q->begin = &Deque_##T##_begin;\
		q->end = &Deque_##T##_end;\
		q->clear = &Deque_##T##_clear;\
		q->dtor = &Deque_##T##_dtor;\
		q->sort = &Deque_##T##_sort;\
	}\
	bool Deque_##T##_equal(Deque_##T& q1, Deque_##T& q2){\
		size_t sz1 = q1.size(&q1);\
		size_t sz2 = q2.size(&q2);\
		if (sz1 != sz2) return false;\
		for (size_t i = 0; i < sz1; i++) {\
			bool less =q1.less_than(q1.data[(i+q1.front_id+1)%q1.buff_size], q2.data[(i+q2.front_id+1)%q2.buff_size]);\
			bool more =q1.less_than(q2.data[(i+q2.front_id+1)%q2.buff_size], q1.data[(i+q1.front_id+1)%q1.buff_size]);\
			if (less || more) return false;\
		}\
		return true;\
	}\
