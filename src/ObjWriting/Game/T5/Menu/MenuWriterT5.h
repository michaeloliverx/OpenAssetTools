#pragma once

#include "Game/T5/T5.h"
#include "Menu/IMenuWriter.h"

#include <memory>
#include <ostream>

namespace menu
{
    class IWriterT5 : public IWriter
    {
    public:
        virtual void WriteMenu(const T5::menuDef_t& menu) = 0;
    };

    std::unique_ptr<IWriterT5> CreateMenuWriterT5(std::ostream& stream);
} // namespace menu
