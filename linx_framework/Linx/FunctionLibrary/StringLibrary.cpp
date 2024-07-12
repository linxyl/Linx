#include "StringLibrary.h"

std::string& Linx::Replace(std::string& Target, const std::string& Old, const std::string& New)
{
    size_t Pos = 0;
    while ((Pos = Target.find(Old, Pos)) != std::string::npos)
    {
        Target.replace(Pos, Old.length(), New);
        Pos += New.length();
    }
    return Target;
}
