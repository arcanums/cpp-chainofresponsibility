// ChainOfResponsibility.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <vector>
#include <ctime>
#include <memory>
using namespace std;

class Base
{
	// Pointer in the base class
	std::shared_ptr< Base > next;
public:
	Base()
	{
		next = nullptr;
	}
	void setNext(std::shared_ptr< Base > n)
	{
		next = n;
	}
	void add(std::shared_ptr< Base > n)
	{
		if (next)
			next->add(n);
		else
			next = n;
	}
	// 2. The "chain" method in the base class always delegates to the next obj
	virtual void handle(int i, std::vector<int>& numbers)
	{
		if (next)
			next->handle(i, numbers);
	}

	void showPassed(std::string str, int i)
	{
		cout << str.c_str() << " passed (" << i << ")" << endl;
	}

	void showNext(std::string str, int i)
	{
		cout << str.c_str() << "(" << i << ") next -> ";
	}

	~Base()
	{
		cout << "~Base()" << endl;
	}
};

class Handler1 : public Base
{
public:
	/*virtual*/void handle(int i, std::vector<int>& numbers)
	{
		if (i == 1)
		{
			Base::showPassed("H1", i);
			numbers.push_back(i);
		}
		else
		{
			Base::showNext("H1", i);
			Base::handle(i, numbers);
		}

	}
	~Handler1()
	{
		cout << "~Handler1()" << endl;
	}
};

class Handler2 : public Base
{
public:
	/*virtual*/void handle(int i, std::vector<int>& numbers)
	{
		if (i == 2)
		{
			Base::showPassed("H2", i);
			numbers.push_back(i);
		}
		else
		{
			Base::showNext("H2", i);
			Base::handle(i, numbers);
		}
	}

	~Handler2()
	{
		cout << "~Handler2()" << endl;
	}
};

class Handler3 : public Base
{
public:
	/*virtual*/void handle(int i, std::vector<int>& numbers)
	{
		if (i == 3)
		{
			Base::showPassed("H3", i);
			numbers.push_back(i);
		}
		else
		{
			Base::showNext("H3", i);
			Base::handle(i, numbers);
		}
	}

	~Handler3()
	{
		cout << "~Handler3()" << endl;
	}
};

class Handler4 : public Base
{
public:
	/*virtual*/void handle(int i, std::vector<int>& numbers)
	{
		if (i == 4)
		{
			Base::showPassed("H4", i);
			numbers.push_back(i);
		}
		else
		{
			Base::showNext("H4", i);
			Base::handle(i, numbers);
		}
	}

	~Handler4()
	{
		cout << "~Handler4()" << endl;
	}
};

class Handler5 : public Base
{
public:
	/*virtual*/void handle(int i, std::vector<int>& numbers)
	{
		if (i == 5)
		{
			Base::showPassed("H5", i);
			numbers.push_back(i);
		}
		else
		{
			Base::showNext("H5", i);
			Base::handle(i, numbers);
		}
	}

	~Handler5()
	{
		cout << "~Handler5()" << endl;
	}
};


template < typename THandlerType, typename... TOtherHandlers, typename ... TArgs >
std::shared_ptr< Base > createHandlersChain(TArgs& ... args)
{
	cout << "root->" << typeid(THandlerType).name() << endl;
	return createHandlersChain< TOtherHandlers... >(std::make_shared< THandlerType >(args...), args...);
}

template < typename THandlerType, typename... TOtherHandlers, typename ... TArgs >
std::shared_ptr< Base > createHandlersChain(std::shared_ptr< Base > handler, TArgs& ... args)
{
	cout << "root2->" << typeid(THandlerType).name() << endl;
	handler->add(std::make_shared< THandlerType >(args...));
	return createHandlersChain< TOtherHandlers... >(handler, args...);
}

template < typename ... TArgs >
std::shared_ptr< Base > createHandlersChain(std::shared_ptr< Base > handler, TArgs& ... args)
{
	cout << "root3->" << typeid(handler).name() << endl;
	return std::move(handler);
}


template < typename THandlerType, typename... TOtherHandlers >
std::size_t sizeHandlersChain()
{

	return sizeof...(TOtherHandlers);
}


int main()
{
	std::cout << "Hello World!\n";

	auto root = createHandlersChain<Handler1, Handler2, Handler3, Handler4, Handler5>();

	std::vector<int> numbers;
	cout << "1=";
	root->handle(1, numbers);
	cout << "3=";
	root->handle(3, numbers);
	cout << "2=";
	root->handle(2, numbers);
	cout << "5=";
	root->handle(5, numbers);
	cout << "4=";
	root->handle(4, numbers);


	for (size_t i = 0; i < numbers.size(); i++)
	{

		cout << numbers[i] << '\n';
	}
}


