#ifndef GUARD_Queue_h
#define GUARD_Queue_h
#include<iostream>

template <class T> struct node {
	T data; node* link;
	node() { data = 0, link = nullptr; }
	explicit node(const T& t) { data = t, link = nullptr; }
};

template <class T> class Queue {
	typedef node<T> NODE;
public:
	typedef NODE* iterator;
	typedef const NODE* const_iterator;
	typedef size_t size_type;
	typedef NODE value_type;
	typedef NODE& reference;
	typedef const NODE& const_reference;
	
	Queue() { head = tail = nullptr; cap = 50; }//默认队列的最大容量为50
	explicit Queue(size_type max) { head = tail = nullptr; cap = max; }
	
	void push(const T& t);//在队尾插入元素
	void pop();//队首元素退队
	iterator front() { return head; }//取队首元素的值
	const_iterator front() const { return head; }
	iterator back() { return tail; }//取队尾元素的值
	const_iterator back() const { return tail; }
	size_type size() const;//求队列长度
	bool empty() { return size() == 0 ? true : false; }//判断队列是否为空
	bool full() { return size() >= cap ? true : false; }//判断队列是否满了
	void output();//输出队列元素
private:
	iterator head, tail;
	size_type cap;
};
template<class T>
void Queue<T>::push(const T& t) {
	NODE* node = new NODE(t);
	if (head == nullptr) head = node;
	else tail->link = node;
	tail = node;
}
template<class T>
void Queue<T>::pop(){
	NODE* tmp;
	if (head == nullptr) return;//队列为空
	tmp = head;
	head = head->link;
	delete tmp;
}
template<class T>
size_t Queue<T>::size() const {
	if (head == nullptr) return 0;
	size_t cnt = 1;
	NODE* tmp;
	tmp = head;
	while (tmp->link != nullptr) {
		tmp = tmp->link;
		cnt++;
	}
	return cnt;
}
template<class T>
void Queue<T>::output() {
	if (head != nullptr) {
		NODE* tmp;
		int enter = 0;
		tmp = head;
		while (tmp != nullptr) {
			std::cout << tmp->data << " ";
			tmp = tmp->link;
			enter++;
			if (enter % 10 == 0) std::cout << std::endl;
		}
	}
}

#endif // !GUARD_Queue_h
