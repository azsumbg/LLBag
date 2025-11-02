#include "pch.h"
#include"LLBag.h"

// EXCEPTION CLASS ************************

LLException::LLException(int err_code)
{
	what_happened = err_code;
}

LPCWSTR LLException::GetError() const
{
	switch (what_happened)
	{
	case LLbad_header:
		return bad_header;
		
	case LLbad_index:
		return bad_index;

	case LLbad_pointer:
		return bad_pointer;

	case LLbad_tail:
		return bad_tail;

	}
}

///////////////////////////////////////////