using namespace std;

#define INIT_SIZE 64

#define Deque_DEFINE(T)\
	struct Deque_##T {\
		T data[INIT_SIZE];\
		size_t data_size = INIT_SIZE;\
		char type_name[10];\
		size_t (*size)(Deque_##T*);\
		bool (*empty)(Deque_##T*);\
	};\
	void Deque_##T##_ctor(Deque_##T* q, bool(const T&, const T&)) {\
		\
	}\
	size_t size(Deque_##T* q) {\
		return q->data_size;\
	}\
	bool empty(Deque_##T* q) {\
		return (q->data_size == 0);\
	}\
