#ifndef HSTRING_H

// Author: Hannes Nisula

#include <cstring>
#include <cmath>
#include <wchar.h>
#include <stdlib.h>

typedef		unsigned char					uchar;
typedef		unsigned short					ushort;
typedef		unsigned int					uint;
typedef		__int64							int64;
typedef		unsigned __int64				uint64;

#define		HSTRING_INCREASE_VALUE			16
#define		HSTRING_SHORT_MAX_LENGTH		8
#define		HSTRING_INT_MAX_LENGTH			16
#define		HSTRING_INT64_MAX_LENGTH		32
#define		HSTRING_FLOAT_MAX_LENGTH		32
#define		HSTRING_DOUBLE_MAX_LENGTH		64
#define		HSTRING_BUFFER_SIZE				0xffff

extern	wchar_t __hstringbuffer[HSTRING_BUFFER_SIZE];	// bad idea?

class HString
{
public:

							HString();
							HString(const char chIn);
							HString(const char *chIn);
							HString(const wchar_t wchIn);
							HString(const wchar_t *wchIn);
							HString(const HString &szIn);
							HString(const short in);
							HString(const ushort in);
							HString(const int in);
							HString(const uint in);
							HString(const int64 in);
							HString(const uint64 in);
							HString(const float in);
							HString(const double in);
							~HString();

	const bool				AsBool() const;
	const uchar				AsUChar() const;
	const short				AsShort() const;
	const ushort			AsUShort() const;
	const int				AsInt() const;
	const uint				AsUInt() const;
	const int64				AsInt64() const;
	const uint64			AsUInt64() const;
	const float				AsFloat() const;
	const double			AsDouble() const;
	const char *			AsChar() const;
	const wchar_t *			AsWChar() const;	// max 1024 characters
	const wchar_t *			AsWChar(wchar_t *buffer) const;	// any size

	const bool				IsInt() const;
	const bool				IsFloat() const;
	const bool				IsIP() const;

	char *					CharCopy() const;
	wchar_t *				WCharCopy() const;	// remember to delete, when using these two!

	HString &				Append(const char addition);
	HString &				Append(const char *addition);
	HString &				Append(const wchar_t addition);
	HString &				Append(const wchar_t *addition);
	HString &				Append(const char *addition, const uint cch);
	HString &				Append(const wchar_t *addition, const uint cwch);
	HString &				Append(const HString &addition);
	HString &				Append(const int addition);
	HString &				Append(const uint addition);
	HString &				Append(const float addition);
	HString &				Insert(const HString &addition, const uint i);	// 0-based index
	HString &				Set(const HString &addition);

	void					FromBool(const bool in);
	void					FromShort(const short in);
	void					FromUShort(const ushort in);
	void					FromInt(const int in);
	void					FromUInt(const uint in);
	void					FromInt64(const int64 in);
	void					FromUInt64(const uint64 in);
	void					FromFloat(const float in);
	void					FromDouble(const double in);

	const uint				Length() const;		// excluding null-terminator
	const uint				RealLength() const;	// including null-terminator
	const bool				IsEmpty() const;

	void					Trim();
	void					RemoveLast();
	void					RemoveLast(const uint count);
	void					Clear();

	HString					SubString(const uint after);
	HString *				Split(const wchar_t delimiter, uint &cParts) const;	// ändra parameter return?
	void					Merge(const HString *strings, const uint cStrings, const wchar_t glue);
	void					CutOff(const uint i, const bool removeTrail = false);

	HString					GetToken(const uint start) const;
	HString					GetLine(const uint start) const;

	// these methods use 0 based indexing, and returns -1 if nothing was found
	const int				Find(const char symbol) const;
	const int				Find(const char symbol, const uint start) const;
	const int				Find(const HString& word) const;
	const int				Find(const HString& word, const uint start) const;

	HString					GetPath() const;
	HString					GetFilenameNoExt() const;
	HString					GetFilename() const;
	HString					GetExtension() const;
	HString &				ChangeFilename(const HString &in);		// incomplete
	HString &				ChangeExtension(const HString &in);		// incomplete

	void					FromIP(const uint ip);
	void					FromIPPort(const uint ip, const short port);
	const uint				ToIP() const;		// funkar inte
	const uint				ToIPPort() const;	// funkar inte

	HString					operator +	(const HString &szIn) const;
	HString					operator +	(const char chIn) const;
	HString &				operator =	(const char *wchIn);
	HString &				operator =	(const HString &szIn);
	HString &				operator =	(HString &&szIn);
	HString &				operator +=	(const HString &szIn);
	HString &				operator += (const char *chIn);
	const bool				operator ==	(const HString &szIn) const;
	const bool				operator == (const char *wchIn) const;
	const bool				operator != (const HString &szIn) const;
	const bool				operator != (const char *wchIn) const;
	const char &			operator []	(const uint i) const;
	
	static const bool		IsWhitespace(const char character);
	static const bool		IsWhitespace(const wchar_t character);

private:

	void					_IncreaseCapacity(const uint increment);

	char					*string;
	uint					length;		// null-terminator included
	uint					capacity;
};

#define HSTRING_H
#endif