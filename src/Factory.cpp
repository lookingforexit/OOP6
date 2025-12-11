#include "Factory.h"

#include <stdexcept>

std::shared_ptr<NPC> INPCFactory::create_npc(const std::string& type, int x, int y)
{
    if (type == "Dragon")
    {
        return std::make_shared<Dragon>(x, y);
    }
    if (type == "Frog")
    {
        return std::make_shared<Frog>(x, y);
    }
    if (type == "Knight")
    {
        return std::make_shared<Knight>(x, y);
    }

    throw std::invalid_argument("Unknown type");
}