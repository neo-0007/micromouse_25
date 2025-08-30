#ifndef MYQUEUE_H
#define MYQUEUE_H

#include<iostream>
#include<stdexcept>
using namespace std;

template <typename T>
class MyQueue{

	struct Node {
		T data;
		Node* next;
		Node(T val) : data(val), next(nullptr) {}
	};
	
	Node* head = nullptr;
	Node* tail = nullptr;

	public:
		int enqueue(T val);
		T dequeue();
		T peek() const;
		bool isEmpty() const {return (head==nullptr);}
		int size() const;
		int display() const;
		~MyQueue(){
			while (!isEmpty())
				dequeue();
		}
};

template <typename T>
int MyQueue<T>::enqueue(T val){
	Node* newnode = new Node(val);
	if (head == nullptr){
		head = newnode;
		tail = newnode;
	}
	else{
		tail->next = newnode;
		tail = newnode;
	}
	return 1;
}

template <typename T>
T MyQueue<T>::dequeue(){
	if (head == nullptr)
		throw underflow_error("Queue Empty!");
	Node* temp = head;
	T val = head->data;
	head = head->next;
	if (head == nullptr)
		tail = nullptr;
	delete temp;
	return val;
}

template <typename T>
T MyQueue<T>::peek() const {
	if (head == nullptr)
		throw underflow_error("Empty Queue!");
	return head->data;
}

template <typename T>
int MyQueue<T>::size() const {
	if (head==nullptr)
		return 0;
	Node* temp = head;
	int count = 0;
	while (temp!=nullptr){
		temp = temp->next;
		count++;
	}
	return count;
}

template <typename T>
int MyQueue<T>::display() const {
	if (head==nullptr)
		return 0;
	Node* temp = head;
	while (temp!=nullptr){
		cout << temp->data << " ";
		temp = temp->next;
	}
	cout << endl;
	return 1;
}

#endif