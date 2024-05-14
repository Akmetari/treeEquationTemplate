#pragma once

template <typename T>
class SmartPtr
{
public:
	SmartPtr<T>(T* Pointer)
	{
		pointer = Pointer;
		counter = new RefCounter();
		counter->Add();
	}

	SmartPtr<T>(const SmartPtr<T>& Other)
	{
		pointer = Other.pointer;
		counter = Other.counter;
		counter->add();
	}

	~SmartPtr<T>()
	{
		if (counter->dec() == 0)
		{
			delete pointer;
			delete counter;
		}
	}

	SmartPtr<T>& operator*() { return(*pointer); }
	SmartPtr<T>* operator->() { return(pointer); }
	void operator=(SmartPtr* origin) {
		if (pointer != nullptr) delete pointer;
		delete counter;
		pointer = origin->pointer;
		counter = origin->counter;
	}

	SmartPtr <T> duplicate() {
		return SmartPtr<T>(this);
	}

private:
	RefCounter* counter;
	T* pointer;
};



class RefCounter
{
public:
	RefCounter() { count; }
	int add() { return(++count); }
	int dec() { return(--count); };
	int get() { return(count); }
private:
	int count;
};