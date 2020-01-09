#pragma once
template <class T>
class MySmartPointer {

	class RefCounter {
	private:
		int count;
	public:
		RefCounter() { count = 0; }

		int add() { return (++count); }
		int dec() { return (--count); };
		int get() const { return (count); }
	};

private:
	RefCounter* counter;
	T* pointer;
	bool isArray;

public:
	MySmartPointer(T* pointer, bool isArray);
	MySmartPointer(const MySmartPointer& other);
	~MySmartPointer();
public:
	T& operator*();
	T* operator->();
	T& operator[](int index);
	T at(int index) const;
	T* get() const;
	void operator=(MySmartPointer<T>& other);
private:
	void deallocateWhenNoRef() {
		if (counter->dec() == 0) {
			if (isArray)
				delete[] pointer;
			else delete pointer;
			delete counter;
		}
	}
};

template <class T>
MySmartPointer<T>::MySmartPointer(T* pointer, bool isArray) {
	this->pointer = pointer;
	this->isArray = isArray;
	counter = new RefCounter();
	counter->add();
}

template <class T>
MySmartPointer<T>::MySmartPointer(const MySmartPointer& other) {
	pointer = other.pointer;
	isArray = other.isArray;
	counter = other.counter;
	counter->add();
}

template <class T>
MySmartPointer<T>::~MySmartPointer() {
	deallocateWhenNoRef();
}

template <class T>
T& MySmartPointer<T>::operator*() { return (*pointer); }

template <class T>
T* MySmartPointer<T>::operator->() { return (pointer); }

template <class T>
T& MySmartPointer<T>::operator[](int index) {
	if (!isArray) throw;
	return pointer[index];
}

template <class T>
T MySmartPointer<T>::at(int index) const {
	if (!isArray) throw;
	return *(pointer + index);
}

template <class T>
T* MySmartPointer<T>::get() const { return pointer; }

template <class T>
void MySmartPointer<T>::operator=(MySmartPointer<T>& other) {
	deallocateWhenNoRef();
	pointer = other.pointer;
	counter = other.counter;
	counter->add();
}
