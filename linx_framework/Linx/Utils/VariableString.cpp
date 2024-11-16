#include "Linx/Utils/VariableString.h"

using namespace std;

long long Linx::VariableString::ConverToSigned()
{
	if (Str.size() > 1 && '0' == Str[0])
	{
		if ('x' == Str[1] || 'X' == Str[1])
			return std::stoll(Str.substr(2), nullptr, 16);
		else if ('b' == Str[1] || 'B' == Str[1])
			return std::stoll(Str.substr(2), nullptr, 2);
		else
			return std::stoll(Str.substr(1), nullptr, 8);
	}
	else
	{
		return std::stoll(Str);
	}
}

unsigned long long Linx::VariableString::ConvertToUnsigned()
{
	if (Str.size() > 1 && '0' == Str[0])
	{
		if ('x' == Str[1] || 'X' == Str[1])
			return std::stoull(Str.substr(2), nullptr, 16);
		else if ('b' == Str[1] || 'B' == Str[1])
			return std::stoull(Str.substr(2), nullptr, 2);
		else
			return std::stoull(Str.substr(1), nullptr, 8);
	}
	else
	{
		return std::stoull(Str);
	}
}
