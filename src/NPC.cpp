#include "NPC.h"
#include <Visitor.h>

NPC::NPC(int x, int y) : x(x), y(y) {}

bool NPC::is_close(const NPC& other, size_t distance) const
{
    return (x - other.x) * (x - other.x) + (y - other.y) * (y - other.y) <= distance * distance;
}

Dragon::Dragon(int x, int y) : NPC(x, y) {}

void Dragon::accept(INPCVisitor& visitor)
{
    visitor.visit(*this);
}

std::string Dragon::get_type() const
{
    return "Dragon";
}

Frog::Frog(int x, int y) : NPC(x, y) {}

void Frog::accept(INPCVisitor& visitor)
{
    visitor.visit(*this);
}

std::string Frog::get_type() const
{
    return "Frog";
}

Knight::Knight(int x, int y) : NPC(x, y) {}

void Knight::accept(INPCVisitor& visitor)
{
    visitor.visit(*this);
}

std::string Knight::get_type() const
{
    return "Knight";
}