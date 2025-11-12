#pragma once

#ifdef LLBAG_EXPORTS
#define LLBAG_API __declspec(dllexport)
#else 
#define LLBAG_API __declspec(dllimport)
#endif

#include <Windows.h>

constexpr int LLbad_pointer{ 5001 };
constexpr int LLbad_index{ 5002 };
constexpr int LLbad_header{ 5003 };
constexpr int LLbad_tail{ 5004 };

class LLBAG_API LLException
{
private:
	int what_happened{ 0 };
	LPCWSTR bad_pointer{ L"Null pointer passed to \nLLBag container function!" };
	LPCWSTR bad_index{ L"Out of bounds index passed\nto LLBag container function!" };
	LPCWSTR bad_header{ L"Bad header information passed\nto LLBag container function!" };
	LPCWSTR bad_tail{ L"Bad tail information passed\nto LLBag container function!" };

public:

	LLException(int err_code);

	LPCWSTR GetError() const;

};

template<typename U>class LLContainer;

template<typename T> class Node
{
private:
	Node* prev_node{ nullptr };
	Node* next_node{ nullptr };
	T node_data{};
	size_t index{ 0 };

public:
	Node() {};
	Node(T header_data) :node_data{ header_data } {};
	virtual ~Node() {};

	T GetData() const
	{
		return node_data;
	}
	Node* GetPrevNode() const
	{
		return prev_node;
	}
	Node* GetNextNode() const
	{
		return next_node;
	}
	size_t GetIndex()const
	{
		return index;
	}

	friend class LLContainer<T>;
};

template<typename U>class LLContainer
{
private:
	Node<U>* header_ptr{ nullptr };
	Node<U>* tail_ptr{ nullptr };
	size_t container_size{ 0 };
	size_t global_index{ 0 };

public:
	LLContainer() :header_ptr{ new Node<U>() }, tail_ptr{ new Node<U>() }
	{
		container_size = 2;
		if (!header_ptr || !tail_ptr)throw LLException(LLbad_pointer);
		else
		{
			header_ptr->index = global_index;
			++global_index;
			tail_ptr->index = global_index;

			header_ptr->prev_node = nullptr;
			header_ptr->next_node = tail_ptr;

			tail_ptr->prev_node = header_ptr;
			tail_ptr->next_node = nullptr;
		}
	}
	LLContainer(U header_init) :header_ptr{ new Node<U>(header_init) }, tail_ptr{ new Node<U>() }
	{
		container_size = 2;
		if (!header_ptr || !tail_ptr)throw LLException(LLbad_pointer);
		else
		{
			header_ptr->index = global_index;
			++global_index;
			tail_ptr->index = global_index;

			header_ptr->prev_node = nullptr;
			header_ptr->next_node = tail_ptr;

			tail_ptr->prev_node = header_ptr;
			tail_ptr->next_node = nullptr;
		}
	}

	~LLContainer()
	{
		if (container_size > 0)
		{
			if (container_size > 1)
			{
				Node* next_to_delete{ tail_ptr };

				for (size_t count = container_size - 1; count >= 0; --count)
				{
					Node* temp{ next_to_delete->prev_node };
					delete next_to_delete;
					next_to_delete = temp;
				}
			}
			else delete header_ptr;;
		}
	}

	void push_back(U what)
	{
		Node<U>* new_tail{ new (Node<U>(what)) };

		++global_index;

		new_tail->prev_node = tail_ptr;
		new_tail->next_node = nullptr;
		new_tail->index = global_index;

		tail_ptr = new_tail;
	}


};

