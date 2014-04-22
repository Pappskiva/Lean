#ifndef ARRAY_H

#include <utility>

typedef			unsigned char					uchar;
typedef			unsigned short					ushort;
typedef			unsigned int					uint;
typedef			unsigned long					ulong;

#define			ARRAY_INCREASE_VALUE			64

/*=============================================================================

	Array that can't actually be decreased in size and swaps when removing
	items instead of moving the last parts of the array back 1 step.

=============================================================================*/

template <typename _Type>
class	Array
{
public:

							Array(const uint startSize = ARRAY_INCREASE_VALUE, const bool fastIncrease = true, const bool fix = false);
							~Array();

	_Type &					Append();
	_Type *					Append(const _Type &item);
	_Type &					GetLast() const;
	void					Remove(const uint iElement);
	void					RemoveLast();
	void					Clear();
	uint					Length() const;
	_Type *					Pointer();
	void					Sort();
	void					Fix(const bool fix);
	void					SafeIncrease();
	void					FastIncrease();
	void					SetIncreaseValue(const uint in);
	void					Reserve(const uint amount);
	uint					Capacity() const;

	Array&					operator = (const Array<_Type> &in);
	_Type &					operator []	(const uint iIndex) const;

protected:

	void					QuickSort(const uint left, const uint right);
	bool					_ExtendArray();

	_Type					*pArray;
	uint					length;
	uint					capacity;
	bool					fixed;
	bool					fastArrayIncrease;
	uint					increaseValue;

private:

};

#include "Array.inl"

#define ARRAY_H
#endif