#ifndef GUARD_Queue_h
#define GUARD_Queue_h
#include<iostream>

template <class T>
class Queue
{
public:
	typedef size_t size_type;//�˴����size_type�����Ͷ���

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

	Queue& operator=(const Queue&);		//��ֵ����������ض���

	bool enQueue(T);	//����
	bool deQueue();		//����

	T front()const { return head->val; } //���ض���Ԫ�ص�ֵ
	T back()const { return rear->val; }	//���ض�βԪ�ص�ֵ

	size_type size()const;		//�˴�ʹ��ǰ�涨�������size_type
	bool isEmpty(void) const { return size() == 0 ? true : false; }	//�ж϶����Ƿ�Ϊ��
	bool isFull(void) const { return size() >= 50 ? true : false; }	//�ж϶����Ƿ�����

	void output(void)const;		//�������
private:
	struct NODE {
		T val;
		NODE* next;
	}*head, * rear;	//������βԪ��ָ��

	//�ڴ������
	void copyLink(NODE*);	//������������
	void freeLink();		//�ͷ�����������ռ�ռ�
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
	if (head == nullptr) return;//����Ϊ��
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
	if (head == nullptr) return false;//����Ϊ��
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
