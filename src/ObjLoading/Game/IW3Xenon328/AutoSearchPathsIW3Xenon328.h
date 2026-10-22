#pragma once

#include "Game/AutoSearchPaths.h"

class AutoSearchPathsIW3Xenon328 final : public AutoSearchPaths
{
protected:
    [[nodiscard]] const std::vector<std::string>& RecognizedZoneDirs() const override;
    [[nodiscard]] const std::vector<std::string>& AdditionalSearchPaths() const override;
};
