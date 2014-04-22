/*==================
Array::Array

  Default constructor that sets values to 0.
==================*/

template <typename _Type>
Array<_Type>::Array(const uint startSize, const bool fastIncrease, const bool fix)
{
	length = 0;
	capacity = startSize;
	increaseValue = ARRAY_INCREASE_VALUE;
	fixed = fix;
	fastArrayIncrease = fastIncrease;

	pArray = new _Type[capacity];
}

/*==================
Array::~Array()

  Deletes the array.
==================*/

template <typename _Type>
Array<_Type>::~Array()
{
	if(pArray)
		delete[] pArray;

	pArray = 0;
}

/*==================
Array::_ExtendArray()

	Increases the size of the array
==================*/

template <typename _Type>
bool Array<_Type>::_ExtendArray()
{
	if (fixed)
		return false;

	uint newCapacity = capacity + increaseValue;
	_Type* newArray = new _Type[newCapacity];

	if (fastArrayIncrease)
		memcpy(newArray, pArray, sizeof(_Type) * length);
	else
	for (uint i = 0; i < length; ++i)
		newArray[i] = std::move(pArray[i]);
		
	delete[] pArray;

	pArray = newArray;
	capacity = newCapacity;

	return true;
}

/*==================
Array::Append

  Appends a new item which is not initialized, but
  returns a reference to the data.
==================*/

template <typename _Type>
_Type & Array<_Type>::Append()
{
	if (length == capacity && !_ExtendArray())
		return pArray[length - 1];

	return pArray[length++];
}

/*==================
Array::Append

  Appends a thing to the end of the array.
  If there's not enough space the array will be enlarged.
==================*/

template <typename _Type>
_Type * Array<_Type>::Append(const _Type &item)
{
	if (length == capacity && !_ExtendArray())
		return 0;

	pArray[length] = item;

	return &pArray[length++];
}

/*==================
Array::GetLast

  Returns a reference to the last element in the array
==================*/

template <typename _Type>
inline _Type & Array<_Type>::GetLast() const
{
	return pArray[length - 1];
}

/*==================
Array::Remove

  Removes the thing on the element index.
  Places the last thing on that element
  and decreases length by 1.
==================*/

template <typename _Type>
void Array<_Type>::Remove(const uint element)
{
	pArray[element] = pArray[--length];
}

template <typename _Type>
void Array<_Type>::RemoveLast()
{
	--length;
}

/*==================
Array::Clear

  Just sets length to 0.
==================*/

template <typename _Type>
void Array<_Type>::Clear()
{
	length = 0;
}

/*==================
Array::Length
==================*/

template <typename _Type>
uint Array<_Type>::Length() const
{
	return length;
}

/*==================
Array::Pointer
==================*/

template <typename _Type>
_Type* Array<_Type>::Pointer()
{
	return pArray;
}


/*==================
Array::Fix
==================*/

template <typename _Type>
void Array<_Type>::Fix(const bool fix)
{
	fixed = fix;
}

/*==================
Array::SafeIncrease
==================*/

template <typename _Type>
void Array<_Type>::SafeIncrease()
{
	fastArrayIncrease = false;
}

/*==================
Array::FastIncrease
==================*/

template <typename _Type>
void Array<_Type>::FastIncrease()
{
	fastArrayIncrease = true;
}

/*==================
Array::SetIncreaseValue
==================*/

template <typename _Type>
void Array<_Type>::SetIncreaseValue(const uint in)
{
	increaseValue = in;
}

/*==================
Array::Reserve
==================*/

template <typename _Type>
void Array<_Type>::Reserve(const uint size)
{
	if(pArray)
		delete[] pArray;

	length = 0;
	capacity = size;
	pArray = new _Type[capacity];
}

template <typename _Type>
uint Array<_Type>::Capacity() const
{
	return capacity;
}

/*==================
Array::operator []
==================*/

template <typename _Type>
inline _Type& Array<_Type>::operator [] (const uint index) const
{
	return pArray[index];
}

/*================
Array::operator =
================*/

template <typename _Type>
Array<_Type>& Array<_Type>::operator=(const Array<_Type> &in)
{
	
	fixed			= in.fixed;
	increaseValue	= in.increaseValue;
	length			= in.length;
	
	if (capacity != in.capacity)
	{
		if (pArray)
			delete[] pArray;
		pArray = new _Type[in.capacity];
	}
	
	capacity		= in.capacity;

	if (length)
		memcpy(pArray, in.pArray, sizeof(_Type) * length);	

	return *this;
}

/*==================
Array::Sort
==================*/

template <typename _Type>
void Array<_Type>::Sort()
{
	if(length)
		QuickSort(0, length - 1);
}

/*==================
Array::QuickSort
==================*/

template <typename _Type>
void Array<_Type>::QuickSort(const uint left, const uint right)
{
	if(right < left)		// maybe not required because of the ifs at the bottom?
		return;

	if(right - left < 2)
	{
		if(pArray[right] < pArray[left])
		{
			_Type temp = pArray[left];
			pArray[left] = pArray[right];
			pArray[right] = temp;
		}

		return;
	}

	uint pivotIndex = (left + right) / 2;
	_Type pivotElement = pArray[pivotIndex];
	_Type temp;

	pArray[pivotIndex] = pArray[right];
	pArray[right] = pivotElement;

	uint storeIndex = left;

	for(uint i = left; i < right; ++i)
		if(pArray[i] < pivotElement)
		{
			temp = pArray[i];
			pArray[i] = pArray[storeIndex];
			pArray[storeIndex++] = temp;
		}

	pArray[right] = pArray[storeIndex];
	pArray[storeIndex] = pivotElement;


	if(storeIndex > left + 1)
		QuickSort(left, storeIndex - 1);
	if(storeIndex < right - 1)
		QuickSort(storeIndex + 1, right);
}