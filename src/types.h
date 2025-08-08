#pragma once
namespace System
{
	class String
	{
	public:
		char zpad[0x10];
		int size;
		wchar_t buffer[256];
    };
};