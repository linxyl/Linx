#include <iostream>
#include "Linx/Protocol/Ini.h"
#include "Linx/FunctionLibrary/StringLibrary.h"
#include "Linx/IOStream/FileStream.h"

using namespace std;

bool Linx::Ini::Load(std::istream& IStream)
{
	string Name;
	string line;

	if (IStream.fail())
	{
		return false;
	}

	while (std::getline(IStream, line))
	{
		TrimAll(line);

		if ('#' == line[0]) // It is a comment
		{
			IniData[line];
			continue;
		}
		else if ('[' == line[0]) // It is a section
		{
			int pos = line.find_first_of(']');
			if (-1 != pos)
			{
				Name = line.substr(1, pos - 1);
				IniData[Name];
			}
		}
		else // It is the "key=value" line
		{
			int pos = line.find_first_of('=');
			if (pos <= 0)
			{
				continue;
			}

			// Add new key to the last section in the storage
			string Key = line.substr(0, pos);
			string Value = line.substr(pos + 1, line.size() - pos - 1);
			if (IniData.find(Name) == IniData.end())
			{
				return false;
			}
			IniData[Name][Key].first = Value;

			pos = line.find('#');
			if (-1 == pos)
			{
				continue;
			}

			IniData[Name][Key].second = line.substr(pos, line.size() - pos);
		}
	}

	IStream.clear();
	return true;
}

void Linx::Ini::Save(std::ostream& OStream)
{
	//OStream.seekp(0);

	for (const auto& DataIt : IniData)
	{
		// Write section line
		if ('#' == DataIt.first[0])
		{
			OStream << DataIt.first << endl;
			continue;
		}

		OStream << "[" << DataIt.first << "]\n";
		for (const auto& ItSection : DataIt.second)
		{
			// Write "key = value" line
			OStream << ItSection.first << " = " << (string)ItSection.second.first;
			if (!ItSection.second.second.empty())
			{
				OStream << ' ' << ItSection.second.second;
			}
			OStream  << "\n";
		}
		OStream << "\n";
	}
	OStream.flush();
}

Linx::VariableString Linx::Ini::Get(const IniOption& Option)
{
	if (IniData.find(Option.Section) != IniData.end())
	{
		if (IniData[Option.Section].find(Option.Key) != IniData[Option.Section].end())
		{
			return IniData[Option.Section][Option.Key].first;
		}
	}
	throw std::logic_error("Option not found");
}

void Linx::Ini::Set(const IniOption& Option, const VariableString& Val)
{
	IniData[Option.Section][Option.Key].first = Val;
}

bool Linx::IniFile::Load()
{
	FileStream File(Filename, EFileFlag::ERead | EFileFlag::EOpen);
	return Ini::Load(File);
}

void Linx::IniFile::Save()
{
	FileStream File(Filename, EFileFlag::EWrite | EFileFlag::ECreate | EFileFlag::ETruncate);
	Ini::Save(File);
}
