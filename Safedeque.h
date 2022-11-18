

#ifndef __SAFEDEQUE__
#define __SAFEDEQUE__

#include<iostream>
#include<iomanip>
#include <cassert>

using namespace std;

// forward declaration
template<typename T>
class Safedeque;  //declare Safedeque class

template<typename T>
class Iterator;   //declare Iterator class

template<typename T>
class Iterator {
	friend class Safedeque<T>;

private:
	const Safedeque<T>* dequePointer_;  //Safedeque pointer
	size_t baseIndex_;  //iterate through a specific element of certain index


public:
	//constructors
	Iterator();
	Iterator(const Safedeque<T>& sd);
	Iterator(const Iterator& I);


	//dereference operator 
	T& operator *() const; //Return reference to the current T value
	T& operator [] (size_t i) const; //Return reference to Tval at index i

	//operator overload
	Iterator<T>& operator++();     //prefix
	Iterator<T> operator++(int);  //postfix
	Iterator<T>& operator--();		//prefix
	Iterator<T> operator--(int);  //postfix

	bool operator==(const Iterator& i2) const; //check if two iterators point to the same safedeque and the same position
	bool operator!=(const Iterator& i2) const; //check if two iterators point to different safedeque and the same position
};

template <typename T>
Iterator<T>::Iterator() : dequePointer_(0), baseIndex_(0)
{}

template <typename T>
Iterator<T>::Iterator(const Safedeque<T>& sd) : dequePointer_(&sd), baseIndex_(0) //Safedeque pointer of Safedeque sd, defaultly iterate from the first element of sd
{}

template <typename T>
Iterator<T>::Iterator(const Iterator& i) : dequePointer_(i.dequePointer_), baseIndex_(i.baseIndex_) //iterator to an iterator, point to the postion and safedeque pointed by Iterator i
{}

// dereference
template <typename T>
T& Iterator<T>::operator[] (size_t index) const {
	assert(!dequePointer_);  //ensure that the iterator is pointing to a Safedeque
	return dequePointer_->operator[](index + baseIndex_);
}

template <typename T>
T& Iterator<T>::operator *() const {
	assert(baseIndex_ >= 0);
	return dequePointer_->operator[](baseIndex_); //deference the element pointed by the dequepointer 
}

// operator overload
template <typename T>
bool Iterator<T>::operator==(const Iterator& i2) const {
	return (dequePointer_ == i2.dequePointer_) && (baseIndex_ == i2.baseIndex_); //check if two iterators are pointing to the same Safadeque and if they are iterating through the same position of Safedeque
}

template <typename T>
bool Iterator<T>::operator!=(const Iterator& i2) const { //check if two iterators are not pointing to the same Safadeque and if they are not iterating through the same position of Safedeque
	return !(*this == i2);
}

template <typename T>
Iterator<T>& Iterator<T>::operator++() { //prefix
	++baseIndex_;  //iterate through next element
	return *this;  //return the new iterator
}

template <typename T>
Iterator<T> Iterator<T>::operator++(int) { //postfix
	Iterator<T> i(*this);
	operator ++();  //iterate through next element
	return i;       //return the old iterator
}

template <typename T>
Iterator<T>& Iterator<T>::operator--() {  //prefix
	--baseIndex_;  //iterate through previous element
	return *this;  //return the new iterator
}

template <typename T>
Iterator<T> Iterator<T>::operator--(int) {  //postfix
	Iterator<T> i(*this);
	operator --(); //iterate through previous element
	return i;  //return the old iterator
}


/////////////////////////////////////////////
/////////////////////////////////////////////

template<typename T>
class Safedeque {
private:
	size_t cap; //capacity ofthe deque
	size_t frontIndex_, backIndex_;
	T* data;  //store the data
	size_t sz;

	bool full() {  //check if the Safedeque is full
		return cap == size();
	}
	bool insideRange(size_t i) { // check if certain index is within the range limited by the front and back "pointer"
		if (frontIndex_ <= backIndex_) {
			return (i <= backIndex_) && (i >= frontIndex_);
		}
		if (backIndex_ == -1)
			return 0;
		return (i >= frontIndex_) || (i <= backIndex_);
	}
	void initializeOneSpot(); //initialize the deque with one spot
	void doubleCapacityAndReorganize(); //double the capacity of the deque and reorganize the element in the deque
public:
	// constructors
	Safedeque();

	// get size of the deque, consider the case that backIndex_ is behind frontIndex_
	size_t size() const {
		if (empty())	return 0;
		if (backIndex_ >= frontIndex_) { //if the backIndex is on the right side of the frontIndex, or the backIndex is in the same position with the frontIndex
			return backIndex_ - frontIndex_ + 1;
		}
		return cap + backIndex_ - frontIndex_ + 1; //if the backIndex is on the left side of the frontIndex
	}

	// check if the deque is empty
	bool empty() const {
		return backIndex_ == -1;
	}

	// return the index of the first element (if exists)
	size_t front() {
		return frontIndex_;
	};

	// return the index of the last element (if exists)
	size_t back() {
		return backIndex_;
	}

	//return the first(front) element
	T& get_front() const {
		assert(front() != back());
		return operator[](0);
	}

	//return the last(back) element
	T& get_back() const {
		assert(front() != back());
		return operator[](size() - 1);
	}

	void push_front(const T& key); //append values at the front of the deque;
	void push_back(const T& key);  //append values at the back of the deque;
	void pop_front();  //remove the frontmost element
	void pop_back();   //remove the backmost element

	size_t capacity() {
		return cap;  //get the capacity of the deque
	}

	bool empty() {
		return backIndex_ == -1;
	}
	void print();  //print all elements from front to back
	void real_print();  //print all elements in data in their actual array positions from the first to the last
	void debug_print();   //a hepler function to debug the print function

	// operator 
	T& operator [] (size_t i) const;
	// Iterator 
	friend class Iterator<T>;
	Iterator<T> begin() const;  //return the Iterator iterate through the first element of the deque
	Iterator<T> end() const;  //return the Iterator iterate through the element one past the final element of the deque
};

//default constuctor of Safedeque
template <typename T>
Safedeque<T>::Safedeque() {
	cap = 0;
	data = new T[cap];
	frontIndex_ = 0;  //initialize the position of frontIndex
	backIndex_ = -1;  //initialize the position of backIndex
}

template <typename T>
void Safedeque<T>::initializeOneSpot() { //Initialize a deque with one spot
	assert(cap == 0); //ensure that the deque is empty
	cap = 1;  //Initialize one spot
	T* newData = new T[cap];  //allocate memory for the new array
	delete[] data;
	data = newData;
	frontIndex_ = 0; //make frontIndex and backIndex point to the only spot of deque
	backIndex_ = 0;

	cout << "One spot added. Current capacity: " << cap << endl;
	return;
}

template <typename T>
void Safedeque<T>::doubleCapacityAndReorganize() { //re-organize the elements in deque when doubling the capacity of deque
	assert(cap > 0); //ensure that the deque is not empty
	auto newCapacity = cap * 2;

	// allocate memory for the new array
	T* newData = new T[newCapacity];

	// move data to the new array
	//If the frontIndex is on the right side of backIndex, or they are in the same position
	if (frontIndex_ <= backIndex_) {
		for (auto i = frontIndex_; i <= backIndex_; i++) { //copy the element in data to newData
			newData[i] = data[i];  
		}
	}

	//If front Index is on the left side of backIndex
	else {
		for (auto i = frontIndex_; i < cap; i++) {  //copy the element in data to newData
			newData[i] = data[i]; 
		}
		for (auto i = 0; i <= backIndex_; i++) {  //copy the element in data to newData
			newData[i + cap] = data[i];
		}
		backIndex_ += cap;

	}
	delete[] data;
	data = newData;  //update the element in data
	cap = newCapacity;  //update the capacity of deque
	cout << "Capacity doubled. Current capacity: " << cap << endl;
}


template <typename T>
void Safedeque<T>::push_front(const T& key) {
	if (cap == 0) {  //when the deque is empty, initialize the deque with one spot
		initializeOneSpot();
		data[frontIndex_] = key; //frontIndex indicates this inserted key
	}
	//when the deque is not empty
	else {
		if (full()) {  //when the deque is full, double the capacity of deque
			doubleCapacityAndReorganize();
		}
		if (front() == 0) {  //when the frontIndex indicate the frontest element
			frontIndex_ = cap;
		}
		--frontIndex_;  //let the frontindex indicate the element on the rightest side of deque
		data[frontIndex_] = key; //key become the new frontest element

	}
}
template <typename T>
void Safedeque<T>::push_back(const T& key) {
	// special treatment for capacity = 1
	if (cap == 0) {
		initializeOneSpot();
		data[frontIndex_] = key;
	}
	// when the deque is not empty
	else {
		if (full()) { //when the deque is full, double the capacity of deque
			doubleCapacityAndReorganize();
		}
		++backIndex_;
		if (back() == cap) {
			backIndex_ = 0;
		}
		data[backIndex_] = key;
	}
}


template <typename T>
void Safedeque<T>::pop_front() {
	assert(!empty()); //throw exceptions if the container is already empty
	if (frontIndex_ == backIndex_) { //when frontIndex and backIndex indicate the same element
		frontIndex_ = 0;
		backIndex_ = -1;
	}
	else {
		if (++frontIndex_ == cap)
			frontIndex_ = 0;
	}
}
template <typename T>
void Safedeque<T>::pop_back() {
	assert(!empty()); //throw exceptions if the container is already empty
	if (frontIndex_ == backIndex_) { //change frontIndex and backIndex when they indicate the same position
		frontIndex_ = 0;
		backIndex_ = -1;
	}
	else {  // if frontIndex and backIndex indicate different position
		if (backIndex_ == 0)
			backIndex_ = cap;
		--backIndex_;
	}
}

template <typename T>
void Safedeque<T>::print() {
	//print all elements from front to back
	for (auto i = 0; i < size(); i++) {
		cout << operator[](i) << " ";
	}
	cout << endl;
}

template <typename T>
// print all elements in data in their actual array positions from the first to the last
void Safedeque<T>::real_print() {

	for (auto i = 0; i < capacity(); i++) {
		if (!insideRange(i)) {  //when out of range, print "*"
			cout << "*" << " ";
		}
		else {
			cout << data[i] << " "; //print out the element within the range
		}
	}
	cout << endl;
}

template <typename T>
void Safedeque<T>::debug_print() { //a helper function used to figure out the problam of the print function
	cout << "print() >>> " << endl;
	print();
	cout << "real_print() >>> " << endl;
	real_print();
	cout << "frontIndex_ = " << frontIndex_ << ", backIndex_ = " << backIndex_ << ", capacity_ = " << cap << ", size = " << size() << endl; // track the frontIndex, backIndex, and capacity 
	cout << "-----------------------------------------------------------------" << endl;

}

template <typename T>
//return the specific element of the deque
T& Safedeque<T>::operator [] (size_t i) const {
	assert(size() >= i);
	i += frontIndex_;
	if (i >= cap) { //when the index out of capacity, go back
		i -= cap;
	}
	return data[i];
}

template <typename T>
//return the Iterator iterate through the first element of the deque
Iterator<T> Safedeque<T>::begin() const {
	Iterator<T> iter;
	iter.dequePointer_ = this;  //set the iterator point to corresponding deque
	iter.baseIndex_ = 0;  //set the iterator indicate the first element of the deque

	return iter;
}

template <typename T>
//return the Iterator iterate through the element one past the final element of the deque
Iterator<T> Safedeque<T>::end() const {
	Iterator<T> iter;
	iter.dequePointer_ = this; //set the iterator point to corresponding deque
	iter.baseIndex_ = size();  //set the iterator indicate the element one past the final element of the deque
	return iter;
}

#endif