#include "AutoSearchPathsNX1.h"

const std::vector<std::string>& AutoSearchPathsNX1::RecognizedZoneDirs() const
{
    static const std::vector<std::string> recognizedZoneDirs;
    return recognizedZoneDirs;
}

const std::vector<std::string>& AutoSearchPathsNX1::AdditionalSearchPaths() const
{
    static const std::vector<std::string> additionalSearchPaths;
    return additionalSearchPaths;
}
