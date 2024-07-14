#ifndef GUARD_Queue_h
#define GUARD_Queue_h
#include<iostream>

template <class T>
class Queue
{
public:
	typedef size_t size_type;//此处添加size_type的类型定义

	Queue() { 
		head = rear = nullptr;
		std::cout << "default constructor called." << std::endl;
	} //default constructor
	Queue(const Queue& q) { 
		copyLink(q.head);
		std::cout << "copy constructor called." << std::endl;
	};	//copy constructor
	~Queue() { 
		freeLink(); 
		std::cout << "destructor called." << std::endl;
	}		//destructor

	Queue& operator=(const Queue&);		//赋值运算符的重载定义

	bool enQueue(T);	//进队
	bool deQueue();		//出队

	T front()const { return head->val; } //返回队首元素的值
	T back()const { return rear->val; }	//返回队尾元素的值

	size_type size()const;		//此处使用前面定义的类型size_type
	bool isEmpty(void) const { return size() == 0 ? true : false; }	//判断队列是否为空
	bool isFull(void) const { return size() >= 50 ? true : false; }	//判断队列是否已满

	void output(void)const;		//输出队列
private:
	struct NODE {
		T val;
		NODE* next;
	}*head, * rear;	//队列首尾元素指针

	//内存管理函数
	void copyLink(NODE*);	//复制已有链表
	void freeLink();		//释放已有链表所占空间
};
template<class T>
void Queue<T>::copyLink(NODE* h) {
	head = new NODE;
	head->val = h->val;
	NODE* loop = h;
	NODE* ptr = head;
	while (loop->next != nullptr) {
		NODE* tmp = new NODE;
		tmp->val = loop->next->val;
		tmp->next = nullptr;
		ptr->next = tmp;
		ptr = ptr->next;
		loop = loop->next;
	}
	rear = ptr;
}
template<class T>
void Queue<T>::freeLink() {
	NODE* tmp;
	if (head == nullptr) return;//队列为空
	while (head != nullptr) {
		tmp = head;
		head = head->next;
		delete tmp;
	}
}
template<class T>
Queue<T>& Queue<T>::operator=(const Queue& rhs) {
	if (&rhs != this) {
		freeLink();
		copyLink(rhs.head);
	}
	std::cout << "operator=() called." << std::endl;
	return *this;
}
template<class T>
bool Queue<T>::enQueue(T t) {
	if (size() == 50)return false;
	NODE* node = new NODE;
	node->val = t; node->next = nullptr;
	if (head == nullptr) head = node;
	else rear->next = node;
	rear = node;
	return true;
}
template<class T>
bool Queue<T>::deQueue() {
	NODE* tmp;
	if (head == nullptr) return false;//队列为空
	tmp = head;
	head = head->next;
	delete tmp;
	return true;
}
template<class T>
size_t Queue<T>::size() const {
	if (head == nullptr) return 0;
	size_t cnt = 1;
	NODE* tmp;
	tmp = head;
	while (tmp->next != nullptr) {
		tmp = tmp->next;
		cnt++;
	}
	return cnt;
}
template<class T>
void Queue<T>::output(void) const {
	if (head != nullptr) {
		NODE* tmp;
		int enter = 0;
		tmp = head;
		while (tmp != nullptr) {
			std::cout << (tmp->val - 1) * 8 << " ";
			tmp = tmp->next;
			enter++;
			if (enter % 10 == 0) std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}

#endif // !GUARD_Queue_h
