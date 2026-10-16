#include "AutoSearchPathsQOSXenon.h"

const std::vector<std::string>& AutoSearchPathsQOSXenon::RecognizedZoneDirs() const
{
    static const std::vector<std::string> recognizedZoneDirs{
        "english",
        "french",
        "german",
        "italian",
        "spanish",
    };
    return recognizedZoneDirs;
}

const std::vector<std::string>& AutoSearchPathsQOSXenon::AdditionalSearchPaths() const
{
    static const std::vector<std::string> additionalSearchPaths{"."};
    return additionalSearchPaths;
}
