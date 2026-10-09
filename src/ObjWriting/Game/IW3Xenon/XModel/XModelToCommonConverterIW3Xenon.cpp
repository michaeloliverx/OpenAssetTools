#include "XModelToCommonConverterIW3Xenon.h"

namespace xmodel
{
    std::optional<XModelCommon> ToCommonConverterIW3Xenon::Convert(const XAssetInfoGeneric&, const unsigned)
    {
        // Xenon uses a different packed-vertex representation. Until its GLB
        // export path is implemented, fail safely instead of reinterpreting it
        // as an IW3 PC XModel.
        return std::nullopt;
    }
} // namespace xmodel
