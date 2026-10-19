#include "AutoSearchPathsIW2Xenon.h"

const std::vector<std::string>& AutoSearchPathsIW2Xenon::RecognizedZoneDirs() const
{
    static const std::vector<std::string> paths;
    return paths;
}

const std::vector<std::string>& AutoSearchPathsIW2Xenon::AdditionalSearchPaths() const
{
    static const std::vector<std::string> paths;
    return paths;
}
