#ifndef LIST_H_
#define LIST_H_

#define NULL 0

/*Lista cuva elemente po kopiji, promeni da cuva po referenci*/
template<class T>
class LinkedList {
public:

	LinkedList() :
		size(0), head(NULL), tail(NULL) {
	}
	~LinkedList() {
		clear();
	}

	void applyAll(int (*func)(T));

	class LinkedListNode {
	public:
		LinkedListNode(T d) :
			data(d), next(NULL), prev(NULL) {
		}
		T data;
		LinkedListNode* next, * prev;
	};

	class Iterator {
	public:
		Iterator(LinkedListNode* e = NULL) :
			elem(e) {
		}

		T& operator*() {
			return elem->data;
		}
		const T& operator*() const {
			return elem->data;
		}
		int operator!=(const Iterator& other) {
			return elem != other.elem;
		}
		int operator==(const Iterator& other) {
			return elem == other.elem;
		}
		Iterator operator++() {
			elem = elem->next;
			return *this;
		}
		Iterator operator++(int) {
			elem = elem->next;
			return *this;
		}
		int isNULL(){
			if (elem == NULL){
				return 1;
			}
			return 0;
		}

	protected:
		friend class LinkedList<T>;
	private:
		LinkedListNode* elem;
	};

	const T& first();
	const T& last();

	Iterator begin() {
		return (size != 0) ? Iterator(head) : Iterator(NULL);
	}
	Iterator end() {
		return (size != 0) ? Iterator(tail->next) : Iterator(NULL);
	}

	Iterator find(const T& e) {
		for (Iterator it = begin(); it != end(); ++it) {
			if (*it == e) {
				return it;
			}
		}
		return Iterator(NULL);
	}


	int isEmpty();
	int getSize();

	void clear();
	void push_back(const T& e);
	void push(const T& e);

	void remove(const T& e) {
		LinkedListNode* it;
		for (it = head; it != tail; it = it->next) {
			if (it->data == e) {
				break;
			}
		}

		if (it == head) {
			head = head->next;
		}
		if (it == tail) {
			tail = tail->prev;
		}

		LinkedListNode* next = it->next, * prev = it->prev;

		if (prev != NULL) {
			prev->next = next;
		}
		if (next != NULL) {
			next->prev = prev;
		}

		it->next = it->prev = NULL;
		size--;

		delete it;

	}

	void insert_before(Iterator position, const T& e) {
		LinkedListNode* cur = position.elem;
		LinkedListNode* prev = cur->prev;

		LinkedListNode* _new = new LinkedListNode(e);

		_new->next = cur;
		_new->prev = prev;

		if (cur != NULL)
			cur->prev = _new;

		if (prev != NULL)
			prev->next = _new;

		if (head == cur)
			head = _new;

		size++;
	}

	void insert_after(Iterator position, const T& e) {
		LinkedListNode* cur = position.elem;
		LinkedListNode* next = cur->next;

		LinkedListNode* _new = new LinkedListNode(e);

		_new->prev = cur;
		_new->next = next;

		if (cur != NULL)
			cur->next = _new;

		if (next != NULL)
			next->prev = _new;

		if (tail == cur)
			tail = _new;

		size++;
	}

	void insert_priority(int (*func)(T, T) , const T& e) {
		Iterator it;
		for (it = begin(); it != end(); ++it) {
				if(func((*it), e)) break;
			}
		if(it == end()) insert_after(it, e);
		else insert_before(it, e);
		}

protected:
	int size;

private:
	LinkedListNode* head, * tail;
};

template<class T>
void LinkedList<T>::push_back(const T& e) {
	LinkedListNode* newE = new LinkedListNode(e);
	if (isEmpty()) {
		head = newE;
	}
	else {
		tail->next = newE;
		newE->prev = tail;
	}

	tail = newE;
	size++;
}

template<class T>
void LinkedList<T>::push(const T& e) {
	LinkedListNode* newE = new LinkedListNode(e);
		if(isEmpty()){
			tail = newE;
		}
		else{
			head->prev = newE;
			newE->next = head;
		}
		head = newE;
		size++;
}

template<class T>
void LinkedList<T>::clear() {
	LinkedListNode* e = head;
	while (e != NULL) {
		head = head->next;
		e->next = e->prev = NULL;
		delete e;
		e = head;
		size--;
	}
	head = tail = NULL;
}

template<class T>
const T& LinkedList<T>::first() {
	return head->data;
}

template<class T>
const T& LinkedList<T>::last() {
	return tail->data;
}

template<class T>
int LinkedList<T>::getSize() {
	return size;
}

template<class T>
int LinkedList<T>::isEmpty() {
	return size == 0;
}

template<class T>
void LinkedList<T>::applyAll(int (*func)(T)) {
	/*Unsynchronized function*/
	for (Iterator it = begin(); it != end(); ++it) {
		if(func((*it))) break;
	}

}
#endif /* LIST_H_ */
