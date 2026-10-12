#pragma once

#include "Game/IW3Xenon/IW3Xenon.h"
#include "Menu/IMenuWriter.h"

#include <memory>
#include <ostream>

namespace menu
{
    class IWriterIW3Xenon : public IWriter
    {
    public:
        virtual void WriteMenu(const IW3Xenon::menuDef_t& menu) = 0;
    };

    std::unique_ptr<IWriterIW3Xenon> CreateMenuWriterIW3Xenon(std::ostream& stream);
} // namespace menu
