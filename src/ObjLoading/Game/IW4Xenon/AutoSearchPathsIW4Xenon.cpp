#include "AutoSearchPathsIW4Xenon.h"

const std::vector<std::string>& AutoSearchPathsIW4Xenon::RecognizedZoneDirs() const
{
    // Retail Xenon fastfiles live directly in the game's root folder rather
    // than in IW4 PC's zone/<language> hierarchy.
    static const std::vector<std::string> recognizedZoneDirs;
    return recognizedZoneDirs;
}

const std::vector<std::string>& AutoSearchPathsIW4Xenon::AdditionalSearchPaths() const
{
    static const std::vector<std::string> additionalSearchPaths;
    return additionalSearchPaths;
}
