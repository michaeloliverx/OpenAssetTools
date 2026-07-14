#include "StateMapDefinition.h"

#include <limits>

using namespace state_map;

StateMapRule::StateMapRule()
    : m_value(0u),
      m_passthrough(false)
{
}

StateMapEntry::StateMapEntry()
    : m_default_index(std::numeric_limits<size_t>::max())
{
}

StateMapDefinition::StateMapDefinition(std::string name, const size_t entryCount)
    : m_name(std::move(name)),
      m_state_map_entries(entryCount)
{
}
