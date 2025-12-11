#ifndef FACTORY_H
#define FACTORY_H

#include <string>
#include <memory>
#include "NPC.h"

class INPCFactory
{
public:
    static std::shared_ptr<NPC> create_npc(const std::string& type, int x, int y);
};

#endif //FACTORY_H