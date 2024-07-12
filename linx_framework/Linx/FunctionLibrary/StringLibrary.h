#pragma once

#include <string>

namespace Linx
{
	/**
	 * Replace the old string with the new string.
	 * @param Target	The string to be operated on.
	 * @param Old		The string to be replaced.
	 * @param New		The new string after replacement.
	 * @return The reference to the replaced string.
	 */
	std::string& Replace(std::string& Target, const std::string& Old, const std::string& New);
}
