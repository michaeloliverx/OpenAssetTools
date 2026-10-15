#pragma once

#include "Game/T4Xenon/T4Xenon.h"
#include "Menu/IMenuWriter.h"

#include <memory>
#include <ostream>

namespace menu
{
    class IWriterT4Xenon : public IWriter
    {
    public:
        virtual void WriteMenu(const T4Xenon::menuDef_t& menu) = 0;
    };

    std::unique_ptr<IWriterT4Xenon> CreateMenuWriterT4Xenon(std::ostream& stream);
} // namespace menu
