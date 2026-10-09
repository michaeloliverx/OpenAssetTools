#include "AutoSearchPathsIW3Xenon.h"

const std::vector<std::string>& AutoSearchPathsIW3Xenon::RecognizedZoneDirs() const
{
    // Retail Xenon fastfiles live directly in the game's root folder rather
    // than in IW3 PC's zone/<language> hierarchy.
    static const std::vector<std::string> recognizedZoneDirs;
    return recognizedZoneDirs;
}

const std::vector<std::string>& AutoSearchPathsIW3Xenon::AdditionalSearchPaths() const
{
    static const std::vector<std::string> additionalSearchPaths;
    return additionalSearchPaths;
}
