#include "HString.h"
#include <stdio.h>

#pragma warning(disable: 4996)

wchar_t __hstringbuffer[HSTRING_BUFFER_SIZE];

HString::HString()
{
	capacity = 0;
	string = 0;
	_IncreaseCapacity(HSTRING_INCREASE_VALUE);
	string[0] = '\0';
	length = 1;
}

HString::HString(const char chIn)
{
	capacity = 0;
	string = 0;
	_IncreaseCapacity(HSTRING_INCREASE_VALUE);
	string[0] = chIn;
	string[1] = '\0';
	length = 2;
}

HString::HString(const char* chIn)
{
	capacity = 0;
	string = 0;
	uint cchIn = (uint)strlen(chIn) + 1;;
	_IncreaseCapacity(cchIn);
	memcpy(string, chIn, cchIn);
	length = cchIn;
}

HString::HString(const wchar_t wchIn)
{
	capacity = 0;
	string = 0;
	_IncreaseCapacity(HSTRING_INCREASE_VALUE);
	wcstombs(string, &wchIn, 1);
	string[1] = L'\0';
	length = 2;
}

HString::HString(const wchar_t* wchIn)
{
	capacity = 0;
	string = 0;
	uint cwchIn = (uint)wcslen(wchIn) + 1;
	_IncreaseCapacity(cwchIn);
	wcstombs(string, wchIn, cwchIn);
	length = cwchIn;
}

HString::HString(const HString& szIn)
{
	//chString = 0;
	capacity = 0;
	string = 0;
	_IncreaseCapacity(szIn.length);
	memcpy(string, szIn.string, szIn.length);
	length = szIn.length;
}

HString::HString(const short in)
{
	string = 0;

	FromShort(in);
}

HString::HString(const ushort in)
{
	string = 0;

	FromUShort(in);
}

HString::HString(const int in)
{
	string = 0;

	FromInt(in);
}

HString::HString(const uint in)
{
	string = 0;

	FromUInt(in);
}

HString::HString(const int64 in)
{
	string = 0;

	FromInt64(in);
}

HString::HString(const uint64 in)
{
	string = 0;

	FromUInt64(in);
}

HString::HString(const float in)
{
	string = 0;

	FromFloat(in);
}

HString::HString(const double in)
{
	string = 0;
	FromDouble(in);
}

HString::~HString()
{
	if (string)
		delete[] string;
}

const bool HString::AsBool() const
{
	if (*this == "True" || *this == "true" || *this == "TRUE")
		return true;
	else if (*this == "False" || *this == "false" || *this == "FALSE")
		return false;
	// add more permutations of the string?

	return false;	// :(
}

const uchar HString::AsUChar() const
{
	return atoi(string);
}

const short HString::AsShort() const
{
	return atoi(string);
}

const ushort HString::AsUShort() const
{
	return atoi(string);
}

const int HString::AsInt() const
{
	return atoi(string);
}

const uint HString::AsUInt() const
{
	return atoi(string);
}

const int64 HString::AsInt64() const
{
	return _atoi64(string);
}

const uint64 HString::AsUInt64() const
{
	return _atoi64(string);
}

const float HString::AsFloat() const
{
	return (float)atof(string);
}

const double HString::AsDouble() const
{
	return atof(string);
}

const char * HString::AsChar() const
{
	return string;
}

// max 16384 characters or it will return 0
const wchar_t * HString::AsWChar() const
{
	if(length > 0xffff)
		return 0;

	mbstowcs(__hstringbuffer, string, length);

	return __hstringbuffer;
}

const wchar_t * HString::AsWChar(wchar_t *buffer) const
{
	mbstowcs(buffer, string, length);

	return buffer;
}

const bool HString::IsInt() const
{
	for (uint i = 0; i < length - 1; ++i)
		if (string[i] < 48 || string[i] > 57)
			return false;

	return true;
}

const bool HString::IsFloat() const
{
	bool dotSpot = false;

	for (uint i = 0; i < length - 1; ++i)
		if (string[i] < 48 || string[i] > 57)
			if (string[i] == 46 && !dotSpot)
				dotSpot = true;
			else
				return false;

	return true;
}

const bool HString::IsIP() const
{
	uchar c = 0;
	uchar b = 0;
	uchar cDigits[4] = {0, 0, 0, 0};

	for (uint i = 0; i < length && c <= 3 && b <= 3 && string[i] != L'\0'; ++i)
		if (string[i] < L'0' || string[i] > L'9')
			if (string[i] == L'.')
			{
				b++;
				c = 0;
				continue;
			}
			else
				return false;
		else
		{
			c++;
			cDigits[b]++;
		}

	if (b != 3 || b == 0)
		return false;

	ushort sum = 0;
	uchar nbrs[3] = {1, 10, 100};

	for (uint i = 0, b = 0, c = 0; true; ++i)
	{
		if (c == cDigits[b])
		{
			if (sum > 255)
				return false;
			
			b++;

			if (b == 4)
				break;

			c = 0;
			sum = 0;
			++i;
		}

		sum += (string[i] - L'0') * nbrs[cDigits[b] - c - 1];
		++c;
	}

	return true;
}

char * HString::CharCopy() const
{
	char *newString = new char[length];
	memcpy(newString, string, length);

	return newString;
}

wchar_t * HString::WCharCopy() const
{
	wchar_t *newString = new wchar_t[length];
	mbstowcs(newString, string, length);

	return newString;
}

HString & HString::Append(const char addition)
{
	if (length + 1 > capacity)
		_IncreaseCapacity(HSTRING_INCREASE_VALUE);

	string[length - 1] = addition;
	string[length++] = '\0';

	return *this;
}

HString & HString::Append(const char *addition)
{
	uint cchAddition = (uint)strlen(addition) + 1;

	if(length + cchAddition - 1 > capacity)
		_IncreaseCapacity(cchAddition);	// no + HSTRING_INCREASE_VALUE?

	memcpy(&string[length - 1], addition, cchAddition);
	length += cchAddition - 1;

	return *this;
}

HString & HString::Append(const wchar_t addition)
{
	if (length + 1 > capacity)	// +1 for the wchar_t addition
		_IncreaseCapacity(HSTRING_INCREASE_VALUE);

	wcstombs(&string[length - 1], &addition, 1);
	string[length++] = L'\0';

	return *this;
}

HString & HString::Append(const wchar_t *addition)
{
	uint cwchAddition = (uint)wcslen(addition) + 1;	// excl. null-terminator
	if (length + cwchAddition - 1 > capacity)
		_IncreaseCapacity(cwchAddition);

	wcstombs(&string[length - 1], addition, cwchAddition);
	length += cwchAddition - 1;

	return *this;
}

HString & HString::Append(const char *addition, const uint cch)	// cch is without the null-terminator
{
	if (length + cch > capacity)
		_IncreaseCapacity(cch);

	memcpy(&string[length - 1], addition, cch);
	length += cch;

	return *this;
}

HString & HString::Append(const wchar_t *addition, const uint cwch)
{
	if (length + cwch > capacity)
		_IncreaseCapacity(cwch);

	wcstombs(&string[length - 1], addition, cwch);
	length += cwch;
	string[length - 1] = '\0';

	return *this;
}

HString& HString::Append(const HString &addition)
{
	if (length + addition.length - 1 > capacity)
		_IncreaseCapacity(addition.length - 1);

	memcpy(&string[length - 1], addition.string, addition.length);
	length += addition.length - 1;

	return *this;
}

HString& HString::Append(const int addition)
{
	if (length + HSTRING_INT_MAX_LENGTH > capacity)
		_IncreaseCapacity(HSTRING_INT_MAX_LENGTH);

	length += sprintf_s(&string[length - 1], HSTRING_INT_MAX_LENGTH, "%d", addition);

	return *this;
}

HString& HString::Append(const uint addition)
{
	if (length + HSTRING_INT_MAX_LENGTH > capacity)
		_IncreaseCapacity(HSTRING_INT_MAX_LENGTH);

	length += sprintf_s(&string[length - 1], HSTRING_INT_MAX_LENGTH, "%u", addition);

	return *this;
}

HString& HString::Append(const float addition)
{
	if (length + HSTRING_FLOAT_MAX_LENGTH > capacity)
		_IncreaseCapacity(HSTRING_FLOAT_MAX_LENGTH);

	length += sprintf_s(&string[length - 1], HSTRING_INT_MAX_LENGTH, "%g", addition);

	return *this;
}

HString& HString::Insert(const HString &addition, const uint i)
{
	if (i >= length - 1)
		return Append(addition);

	if (length + addition.length - 1 > capacity)
		_IncreaseCapacity(addition.length - 1);

	uint moveChunkSize = length - i;
	uint addSize = addition.length - 1;
	char *buffer = new char[moveChunkSize];
	memcpy(buffer, &string[i], moveChunkSize);
	memcpy(&string[i], addition.string, addSize);
	memcpy(&string[i + addSize], buffer, moveChunkSize);

	delete[] buffer;
	length += addition.length - 1;

	return *this;
}

void HString::FromBool(const bool in)
{
	if (in)
		*this = "True";
	else
		*this = "False";
}

void HString::FromShort(const short in)
{
	if (string)
		delete[] string;

	capacity = HSTRING_SHORT_MAX_LENGTH;
	string = new char[capacity];
	sprintf_s(string, HSTRING_SHORT_MAX_LENGTH, "%d", in);
	length = (uint)strlen(string) + 1;
}

void HString::FromUShort(const ushort in)
{
	if (string)
		delete[] string;

	capacity = HSTRING_SHORT_MAX_LENGTH;
	string = new char[capacity];
	sprintf_s(string, HSTRING_SHORT_MAX_LENGTH, "%d", in);
	length = (uint)strlen(string) + 1;
}

void HString::FromInt(const int in)
{
	if (string)
		delete[] string;

	capacity = HSTRING_INT_MAX_LENGTH;
	string = new char[capacity];
	sprintf_s(string, HSTRING_INT_MAX_LENGTH, "%d", in);
	length = (uint)strlen(string) + 1;
}

void HString::FromUInt(const uint in)
{
	if (string)
		delete[] string;

	capacity = HSTRING_INT_MAX_LENGTH;
	string = new char[capacity];
	sprintf_s(string, HSTRING_INT_MAX_LENGTH, "%d", in);
	length = (uint)strlen(string) + 1;
}

void HString::FromInt64(const int64 in)
{
	if (string)
		delete[] string;

	capacity = HSTRING_INT64_MAX_LENGTH;
	string = new char[capacity];
	sprintf_s(string, HSTRING_INT64_MAX_LENGTH, "%d", in);
	length = (uint)strlen(string) + 1;
}

void HString::FromUInt64(const uint64 in)
{
	if (string)
		delete[] string;

	capacity = HSTRING_INT64_MAX_LENGTH;
	string = new char[capacity];
	sprintf_s(string, HSTRING_INT64_MAX_LENGTH, "%d", in);
	length = (uint)strlen(string) + 1;
}

void HString::FromFloat(const float in)
{
	if (string)
		delete[] string;

	capacity = HSTRING_FLOAT_MAX_LENGTH;
	string = new char[capacity];
	sprintf_s(string, HSTRING_FLOAT_MAX_LENGTH, "%g", in);
	length = (uint)strlen(string) + 1;
}

void HString::FromDouble(const double in)
{
	if (string)
		delete[] string;

	capacity = HSTRING_DOUBLE_MAX_LENGTH;
	string = new char[capacity];
	sprintf_s(string, HSTRING_DOUBLE_MAX_LENGTH, "%g", in);
	length = (uint)strlen(string) + 1;
}

const uint HString::Length() const
{
	return length - 1;
}

const uint HString::RealLength() const
{
	return length;
}

const bool HString::IsEmpty() const
{
	return length <= 1;
}

// preceding and trailing whitespaces are removed
void HString::Trim()
{
	if (length < 2)
		return;

	uint p = 0;		// preceding
	uint t = 0;		// trailing

	// count preceding white spaces
	for ( ; p < length; ++p)
		if (!IsWhitespace((char)string[p]))
			break;

	if(p > 0) {
		memcpy(string, &string[p], length - p);
		length = length - p;
	}

	if (length < 2)
		return;

	for (t = length - 2; t > 0; --t)
		if (!IsWhitespace((char)string[t]))
			break;

	string[(++t)++] = L'\0';
	length = t;
}

void HString::RemoveLast()
{
	if (length < 2)
		return;

	string[length - 2] = L'\0';
	length--;
}

void HString::RemoveLast(const uint count)
{
	if (length < count + 1)
		return;

	string[length - count - 1] = L'\0';
	length -= count;
}

void HString::Clear()
{
	if (string)
	{
		delete[] string;
		string = nullptr;
	}

	capacity = 0;
	_IncreaseCapacity(HSTRING_INCREASE_VALUE);
	string[0] = '\0';
	length = 1;
}

HString HString::SubString(const uint after)
{
	return HString(&string[after+1]);
}

HString *HString::Split(const wchar_t delimiter, uint &cWords) const
{
	// count whitespaces
	uint whitespaces = 0;
	bool lastWasDelimiter = true;
	cWords = 0;

	for (uint i = 0; i < length - 1; ++i)
		if (string[i] == delimiter)
			lastWasDelimiter = true;
		else if (lastWasDelimiter)		// implicitly also not the delimiter
		{
			cWords++;
			lastWasDelimiter = false;
		}

	HString* strings = new HString[cWords];
	lastWasDelimiter = true;
	int u = -1;

	for (uint i = 0; i < length - 1; ++i)
		if (string[i] == delimiter)
			lastWasDelimiter = true;
		else
		{
			if (lastWasDelimiter)
				++u;

			strings[u].Append(string[i]);
			lastWasDelimiter = false;
		}

	return strings;
}

void HString::Merge(const HString *strings, const uint cStrings, const wchar_t glue)
{
	Clear();

	for (uint i = 0; i < cStrings; ++i)
	{
		Append(strings[i]);
		
		if (glue != 0)
			Append(glue);
	}
}

void HString::CutOff(const uint i, const bool removeTrail)
{
	if (length <= i)
		return;

	string[i] = '\0';
	length = i + 1;

	if (removeTrail)
	{
		char* newString = new char[length];
		capacity = length;

		memcpy(newString, string, length);
		if (string)
			delete[] string;

		string = newString;
	}
}

HString HString::GetToken(const uint start) const
{
	if (start >= length - 1)
		return HString();

	HString token;
	char curChar;
	bool onlyWhitespaces = true;

	for (uint i = start; i < length - 1; ++i) {
		curChar = string[i];
		token.Append(curChar);

		if (!IsWhitespace(curChar))
			onlyWhitespaces = false;

		if (IsWhitespace(curChar) && !onlyWhitespaces)
			break;
	}

	token.Trim();

	return token;
}

HString HString::GetLine(const uint start) const
{
	if (start >= length - 1)
		return HString();

	HString line;

	for (uint i = start; i < length && string[i] != '\n'; ++i)
		line.Append(string[i]);

	return line;
}

const int HString::Find(const char symbol) const
{
	return Find(symbol, 0);
}

const int HString::Find(const char symbol, const uint start) const
{
	for (uint i = start; i < length; ++i)
		if (string[i] == symbol)
			return i;

	return -1;
}

const int HString::Find(const HString& word) const
{
	return Find(word, 0);
}

const int HString::Find(const HString& word, const uint start) const
{
	char curChar = word[0];
	uint wordIndex = 0;
	uint wordStartIndex;

	for (uint i = start; i < length; ++i) {
		if (string[i] == curChar) {
			if (wordIndex == 0)
				wordStartIndex = i;

			if (wordIndex == word.Length() - 1)
				return wordStartIndex;
			else
				curChar = word[++wordIndex];
		} else {
			wordIndex = 0;
			curChar = word[0];
		}
	}

	return -1;
}

HString HString::GetPath() const
{
	uint stop = 0;
	HString out;

	for (uint i = 0; i < length - 2; ++i)
		if (string[i] == '\\' || string[i] == '/')
			stop = i + 1;

	out.Append(string, stop);

	return out;
}

HString HString::GetFilenameNoExt() const
{
	HString szNew;
	uint start = 0;
	uint stop = 0;

	for (uint i = 0; i < length; ++i)
		if (string[i] == '/' || string[i] == '\\' || string[i] == ':')
			start = i + 1;

	for (uint i = start; i < length; ++i)
		if (string[i] == '.')
			stop = i;
	if (stop == 0)
		stop = length - 1;
	
	if (start != length - 1)
		szNew.Append(&string[start], stop - start);

	return szNew;
}

HString HString::GetFilename() const
{
	HString szNew;
	uint start = 0;

	for (uint i = 0; i < length; ++i)
		if (string[i] == '/' || string[i] == '\\' || string[i] == ':')
			start = i + 1;

	if (start != length - 1)
		szNew.Append(&string[start], length - start);

	return szNew;
}

HString HString::GetExtension() const
{
	HString szNew;
	uint start = 0;

	for (uint i = 0; i < length; ++i)
		if (string[i] == '.')
			start = i + 1;

	if (start != length - 1)
		szNew.Append(&string[start], length - start);

	return szNew;
}

void HString::FromIP(const uint ip)
{
	uchar a = ip;			// this order because of the intel architecture
	uchar b = ip >> 8;
	uchar c = ip >> 16;
	uchar d = ip >> 24;
	HString szA((uint)a);
	HString szB((uint)b);
	HString szC((uint)c);
	HString szD((uint)d);

	Clear();
	Append(szA);
	Append('.');
	Append(szB);
	Append('.');
	Append(szC);
	Append('.');
	Append(szD);
}

void HString::FromIPPort(const uint ip, const short port)
{
	uchar a = ip;			// this order because of the intel architecture
	uchar b = ip >> 8;
	uchar c = ip >> 16;
	uchar d = ip >> 24;
	HString szA((uint)a);
	HString szB((uint)b);
	HString szC((uint)c);
	HString szD((uint)d);
	HString szPort((uint)port);

	Clear();
	Append(szA);
	Append('.');
	Append(szB);
	Append('.');
	Append(szC);
	Append('.');
	Append(szD);
	Append(':');
	Append(szPort);
}

const uint HString::ToIP() const
{
	if (!IsIP())
		return 0;

	uint ip = 0;
	uchar cDigits[4] = {0, 0, 0, 0};

	for (uint i = 0, u = 0; i < length; ++i)
		if (string[i] != '.')
			cDigits[u]++;
		else
			u++;

	return ip;
}

HString HString::operator + (const HString& szIn) const
{
	HString res = *this;
	res.Append(szIn);

	return res;
}

HString HString::operator + (const char chIn) const
{
	HString res = *this;
	res.Append(chIn);

	return res;
}

HString & HString::operator = (const char *chIn)
{
	uint cchIn = (uint)strlen(chIn) + 1;
	capacity = cchIn + HSTRING_INCREASE_VALUE;

	if (string)
		delete[] string;
	string = new char[capacity];
	memcpy(string, chIn, cchIn);
	length = cchIn;

	return *this;
}

HString & HString::operator = (const HString& szIn)
{
	length = szIn.length;
	capacity = szIn.capacity;

	if (string)
		delete[] string;
	string = new char[capacity];
	memcpy(string, szIn.string, length);

	return *this;
}

HString & HString::operator = (HString&& szIn)
{
	char* tempString = string;
	uint tempLength = length;
	uint tempCapacity = capacity;

	length = szIn.length;
	capacity = szIn.capacity;
	string = szIn.string;

	szIn.length = tempLength;
	szIn.capacity = tempCapacity;
	szIn.string = tempString;

	return *this;
}

HString & HString::operator += (const HString &szIn)
{
	return Append(szIn);
}

HString & HString::operator += (const char *chIn)
{
	return Append(chIn);
}

const bool HString::operator == (const HString &szIn) const
{
	return strcmp(string, szIn.string) == 0;
}

const bool HString::operator == (const char *chIn) const
{
	return strcmp(string, chIn) == 0;
}

const bool HString::operator != (const HString &szIn) const
{
	return strcmp(string, szIn.string) != 0;
}

const bool HString::operator != (const char *chIn) const
{
	return strcmp(string, chIn) != 0;
}

const char & HString::operator [] (const uint i) const
{
	return string[i];
}

const bool HString::IsWhitespace(const char character)
{
	switch(character)	// there are probably more
	{
	case '\t':	// horizontal tab
	case '\n':	// new line/line feed
	case '\v':	// vertical tab
	case '\f':	// form feed
	case '\r':	// carriage return
	case ' ':	// space
		return true;
	default:
		return false;
	}
}

const bool HString::IsWhitespace(const wchar_t character)
{
	switch(character)
	{
	case L'\t':
	case L'\n':
	case L'\v':
	case L'\f':
	case L'\r':
	case L' ':
		return true;
	default:
		return false;
	}
}

void HString::_IncreaseCapacity(const uint increment)
{
	char* old = 0;
	uint newCapacity = capacity + increment;

	if (string)
		old = string;

	string = new char[newCapacity];

	if (old)
	{
		memcpy(string, old, length);	// +1 for null-terminator
		delete[] old;
	}

	capacity = newCapacity;
}