#pragma once

#include <unordered_map>
#include <iosfwd>
#include "Linx/Utils/VariableString.h"

namespace Linx
{
	struct IniOption
	{
		std::string Section;
		std::string Key;
	};

	class Ini
	{
	public:
		using SectionContent = std::pair<VariableString, std::string>;
		using SectionMap = std::unordered_map<std::string, SectionContent>;

	public:
		bool Load(std::istream& IStream);

		void Save(std::ostream& OStream);

		VariableString Get(const IniOption& Option);

		void Set(const IniOption& Option, const VariableString& Val);

	private:
		inline SectionMap& operator[](const std::string& InSection) noexcept { return IniData[InSection]; }

		std::unordered_map<std::string, SectionMap> IniData;
	};

	class IniFile : public Ini
	{
	public:
		inline IniFile() { Load(); }
		inline IniFile(const std::string& InFilename) { Open(InFilename); }

	public:
		inline void Open(const std::string& InFilename)
		{
			Filename = InFilename;
		}

		bool Load();

		void Save();

	private:
		std::string Filename;
	};
}