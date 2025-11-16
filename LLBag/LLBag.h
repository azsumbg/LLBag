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
constexpr int LLunknown_error{ 5005 };

class LLBAG_API LLException
{
private:
	int what_happened{ 0 };
	LPCWSTR bad_pointer{ L"Null pointer passed to \nLLBag container function!" };
	LPCWSTR bad_index{ L"Out of bounds index passed\nto LLBag container function!" };
	LPCWSTR bad_header{ L"Bad header information passed\nto LLBag container function!" };
	LPCWSTR bad_tail{ L"Bad tail information passed\nto LLBag container function!" };
	LPCWSTR unknown_error{ L"Unknown error occurred\nin LLBag container function!" };

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
	size_t global_index{ 0 };

public:
	LLContainer() :header_ptr{ new Node<U>() }, tail_ptr{ new Node<U>() }
	{
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
		if (global_index >= 0)
		{
			if (global_index >= 1)
			{
				Node* next_to_delete{ tail_ptr };

				for (size_t count = global_index - 1; count >= 0; --count)
				{
					Node* temp{ next_to_delete->prev_node };
					delete next_to_delete;
					next_to_delete = temp;
				}
			}
			else delete header_ptr;;
		}
	}

	size_t size() const
	{
		return global_index;
	}

	void push_back(U what)
	{
		if (!tail_ptr)throw(LLException(LLbad_tail));

		Node<U>* new_tail{ new Node<U>(what) };

		++global_index;

		new_tail->prev_node = tail_ptr;
		new_tail->next_node = nullptr;
		new_tail->index = global_index;

		tail_ptr = new_tail;

		if (!new_tail)throw(LLException(LLbad_tail));
	}
	void push_front(U what)
	{
		if (!header_ptr)throw(LLException(LLbad_header));
		if (!tail_ptr)throw(LLException(LLbad_tail));


		Node<U>* temp{ new Node<U>(what) };

		temp->prev_node = nullptr;
		temp->next_node = header_ptr;

		header_ptr->prev_node = temp;

		header_ptr = temp;
	}
	void insert(size_t index, U what)
	{
		if (index < 0 || index > global_index)throw(LLException(LLbad_index));
		
		if (index == 0)
		{
			push_front(what);
			return;
		}

		Node<U>* temp{ new Node<U>(what) };

		Node* current_element = header_ptr;

		for (size_t count = 0; count <= global_index; ++count)
		{
			if (current_element->index == index)
			{
				temp->prev_node = current_element->prev_node;
				temp->next_node = current_element;
				temp->index = current_element->index;
				
				current_element->prev_node = temp;

				Node<U>* element_to_update = current_element;
				for (size_t i = count + 1; i < global_index; ++i)
				{
					++element_to_update->index;
					element_to_update = element_to_update->next_node;
				}
				break;

				++global_index;
			}
		}
		
	}

	Node<U>& operator[](size_t index)
	{
		if (!header_ptr)throw LLException(LLbad_header);
		if (index < 0 || index > global_index)throw(LLException(LLbad_index));

		Node<U>* current_node = header_ptr;

		for (size_t count = 0; count <= global_index; ++count)
		{
			if (current_node->index == index)return *current_node;
			else current_node = current_node->next_node;
		}
		
	}

	void erase(size_t index)
	{
		if (!header_ptr)throw LLException(LLbad_header);
		if (index < 0 || index>global_index)throw LLException(LLbad_index);

		Node<U>* item{ header_ptr };

		for (size_t item_to_erase = 0; item_to_erase <= global_index; ++item_to_erase)
		{
			if (item->index == index)
			{
				Node<U>* prev_item = item->prev_node;
				Node<U>* next_item = item->next_node;

				prev_item->next_node = next_item;
				next_item->prev_node = prev_item;

				delete item;

				for (size_t count = index + 1; count <= global_index; ++count)
				{
					--next_item->index;
					next_item = next_item->next_node;
				}
			}
			else item = item->next_node;
		}
	}

};

