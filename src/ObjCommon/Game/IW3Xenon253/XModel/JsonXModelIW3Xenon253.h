#pragma once

#include "Json/JsonCommon.h"
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace IW3Xenon253
{
    class JsonXModelLod
    {
    public:
        std::string file;
        float distance;
    };

    NLOHMANN_DEFINE_TYPE_EXTENSION(JsonXModelLod, file, distance);

    enum class JsonXModelType
    {
        RIGID,
        ANIMATED,
        VIEWHANDS
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(JsonXModelType,
                                 {
                                     {JsonXModelType::RIGID,     "rigid"    },
                                     {JsonXModelType::ANIMATED,  "animated" },
                                     {JsonXModelType::VIEWHANDS, "viewhands"}
    });

    class JsonXModel
    {
    public:
        std::optional<JsonXModelType> type;
        std::vector<JsonXModelLod> lods;
        std::optional<std::string> rootBoneName;
        std::optional<int> collLod;
        std::optional<std::string> physPreset;
        unsigned flags;
    };

    NLOHMANN_DEFINE_TYPE_EXTENSION(JsonXModel, type, lods, rootBoneName, collLod, physPreset, flags);
} // namespace IW3Xenon253
