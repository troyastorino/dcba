/*
	File:			Array.cc

	Function:		Template definitions for Array.h

	Author(s):		Andrew Willmott

	Copyright:		(c) 1995-2000, Andrew Willmott

	Notes:			

*/

#ifndef __ArrayTmpl__
#define __ArrayTmpl__

#include "cl/Array.h"
#include <ctype.h>

/*! \class Array
	
	Note: We grow the array in size exponentially. That is, every time we
	need to increase the size of the array, we double its size rather than
	increasing it by a fixed amount. For appending n items to an empty list,
	this gives us O(n) copies, as opposed to the O(n^2) copies required if
	we increment by a fixed size each time. 
	
	It would be useful to have an array data structure that utilises a tree of
	fixed-size arrays, thus trading off access time (const vs. o(logbn))
	against eliminating copies. 
*/


TMPLArray TArray::Array(Int size, Int alloc) : items(size),
					allocated(alloc)
{
	Assert(size > 0, "(Array) Initial array size must be positive!");
	if (allocated < size)
		allocated = size;
	
	item = new T[allocated];
}

TMPLArray TArray::Array(const TArray &array) : items(array.items), 
	allocated(array.allocated)
{
	Int i;
	
	if (!allocated)
		allocated = items;
	item = new T[allocated];
	
	for (i = 0; i < array.items; i++)
		item[i] = array.item[i];
}

TMPLArray TArray::~Array()
{
	if (allocated) delete[] item;
}

TMPLArray TArray &TArray::operator = (const TArray &array)
{
	Int i;

	if (allocated < array.items)
	{
		if (allocated) delete[] item;
		allocated = array.items;	
		item = new T[allocated];	
	}
			
	for (i = 0; i < array.items; i++)
		item[i] = array.item[i];

	items = array.items;
	
	return(SELF);
}

TMPLArray ostream &operator << (ostream &s, TArray &array)
{	
	Int		i;
	Char	sepChar;

	s << '[';
	if (array.NumItems() >= 16)
		sepChar = '\n';
	else
		sepChar = ' ';
	
	if (array.NumItems() > 0)
	{
		s << array[0];

		for (i = 1; i < array.NumItems(); i++)
			s << sepChar << array[i];
	}
	
	s << ']';

	return(s);
}

TMPLArray istream &operator >> (istream &s, TArray &array)
{
    Char	c;
	
	//	Expected format: [a b c d ...]
	
    while (isspace(s.peek()))			// 	chomp white space
		s.get(c);
		
    if (s.peek() == '[')						
    {
    	s.get(c);
    	array.Clear();
    	
	    while (isspace(s.peek()))		// 	chomp white space
			s.get(c);
    	
		while (s.peek() != ']')
		{			
			array.Add(1);
			s >> array.Top();			//	read an item
    	
			if (!s)
			{
				_Warning("Couldn't read array component");
				return(s);
			}
	
		    while (isspace(s.peek()))	// 	chomp white space
				s.get(c);
		}			
		s.get(c);
	}
    else
	{
	    s.clear(ios::failbit);
	    _Warning("Error: Expected '[' while reading array");
	    return(s);
	}
	
    return(s);
}

TMPLArray Void TArray::PreAllocate(Int newSize)
{
	Int		i, oldAllocated = allocated;
	T		*newArray;
	
	if (newSize > allocated)
	{
		if (allocated == 0)
			allocated = kFirstAllocation;
		else
			allocated *= 2;	
		
		while (newSize > allocated)
			allocated *= 2;	
		
		newArray = new T[allocated];
	
		for (i = 0; i < items; i++)
			newArray[i] = item[i];	
		
		if (oldAllocated) delete[] item;
		item = newArray;
	}
}

TMPLArray Void TArray::SetSize(Int newSize)
{
	PreAllocate(newSize);
	items = newSize;
}

TMPLArray Void TArray::Add(Int n)
{
	SetSize(items + n);
}

TMPLArray Void TArray::Shrink(Int n)
//	take away n items.
{
	items -= n;
}

TMPLArray Void TArray::Insert(Int i, Int n)
//	Make space at position i for n items.
{
	Assert(i >= 0 && i <= items, "(Array:Insert) Illegal index");
	Assert(n > 0, "(Array:Insert) Illegal insert amount");

	Int j;
	
	Add(n);
	
	for (j = items - 1; j >= i + n; j--)
		item[j] = (item - n)[j];
}

TMPLArray Void TArray::Delete(Int i, Int n)
//	Delete n items at position i.
{
	Assert(i >= 0 && i <= items, "(Array:Insert) Illegal index");
	Assert(n > 0, "(Array:Delete) Illegal insert amount");

	Int j;
	
	items -= n;
		
	for (j = i; j < items; j++)
		item[j] = (item + n)[j];
}

TMPLArray Void TArray::ShrinkWrap()
//	Shrink allocated space to be only the current size of array
// There is no realloc version of new in C++, so this involves another copy.
{
	Int	i, oldAllocated = allocated;
	T	*newArray;
	
	allocated = items;	
	
	newArray = new T[allocated];

	for (i = 0; i < items; i++)
		newArray[i] = item[i];	
	
	if (oldAllocated) delete[] item;
	item = newArray;
}

TMPLArray Void TArray::Grow()
//	Allocate more space for the array. Used internally prior to an items++.
{
	Int		i, oldAllocated = allocated;
	T		*newArray;
	
	if (allocated == 0)
		allocated = kFirstAllocation;
	else
		allocated *= 2;	
	
	newArray = new T[allocated];

	for (i = 0; i < items; i++)
		newArray[i] = item[i];	
	
	if (oldAllocated) delete[] item;
	item = newArray;
}

TMPLArray Void TArray::Append(const TArray &a)
{
	Int		i, j, newSize;
	
	newSize = items + a.items;
	PreAllocate(newSize);

	for (i = items, j = 0; j < a.items; i++, j++)
		item[i] = a.item[j];

	items = newSize;
}

TMPLArray Void TArray::SwapWith(TArray &a)
{
	Int a1, b1;
	
	Swap(a1, b1);
	
	Swap(items, a.items);
	Swap(allocated, a.allocated);
	Swap(item, a.item);
}

TMPLArray Void TArray::Replace(TArray &a)
{
	if (allocated) delete[] item;
	item = a.item;
	items = a.items;
	allocated = a.allocated;

	a.item = 0;
	a.items = 0;
	a.allocated = 0;
}

TMPLArray Void TArray::WriteFile(const Char *filename)
{
	FILE	*file;

	file = fopen(filename, "wb");
	if (file)
	{
		fwrite(item, sizeof(T), items, file);
		fclose(file);
	}
}

TMPLArray Void TArray::ReadFile(const Char *filename)
{
	FILE	*file = fopen(filename, "rb");

	if (file)
	{
		Int		fsize;

		fseek(file, 0, SEEK_END);
		fsize = ftell(file);
		rewind(file);
		items = fsize / sizeof(T);
		Assert(items * sizeof(T) == (size_t) fsize, "(Array::ReadFile) bad file size");
		item = new T[items];
		allocated = items;
		fread(item, sizeof(T), items, file);

		fclose(file);
	}
}

TMPLArray Int TArray::FWrite(FILE *file)
{
	fwrite(&items, sizeof(Int), 1, file);
	fwrite(item, sizeof(T), items, file);
	return(ferror(file));
}

TMPLArray Int TArray::FRead(FILE *file)
{
	if (fread(&items, sizeof(Int), 1, file) != 1)
		return(-1);
	item = new T[items];
	allocated = items;
	return(fread(item, sizeof(T), items, file) != (size_t) items);
}

TMPLArray Void TArray::Attach(T *itemsPtr, Int numItems, Bool shared)
{
	Clear();

	item = itemsPtr;
	items = numItems;

	if (!shared)
		allocated = numItems;
}

#endif
