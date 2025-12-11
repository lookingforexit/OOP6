#include "Arena.h"

#include <iostream>
#include "Factory.h"
#include "Visitor.h"

Arena::Arena()
{
    observers.push_back(std::make_shared<IConsoleObserver>());
    observers.push_back(std::make_shared<FileObserver>());
}

Arena& Arena::get_instance()
{
    static Arena instance;
    return instance;
}

void Arena::add_npc(const std::string& type, int x, int y)
{
    npcs.push_back(INPCFactory::create_npc(type, x, y));
}

void Arena::save_to_file(const std::string& filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        throw std::invalid_argument("Unable to save data to file");
    }

    for (const auto& npc : npcs)
    {
        if (npc->is_alive)
        {
            file << npc->get_type() << ' ' << npc->x << ' ' << npc->y << std::endl;
        }
    }
}

void Arena::load_from_file(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::invalid_argument("Unable to load data from file");
    }

    clear_npcs();

    int x;
    int y;
    std::string type;
    while (file >> type >> x >> y)
    {
        add_npc(type, x, y);
    }
}

void Arena::print_survivors() const
{
    for (const auto& npc : npcs)
    {
        if (npc->is_alive)
        {
            std::cout << npc->get_type() << ' ' << npc->x << ' ' << npc->y << std::endl;
        }
    }
}

void Arena::battle(size_t distance)
{
    size_t start_range = 0;
    while (start_range <= distance)
    {
        print_survivors();

        for (auto& attacker : npcs)
        {
            for (auto& defender : npcs)
            {
                if (attacker != defender && attacker->is_alive && defender->is_alive)
                {
                    if (attacker->is_close(*defender, start_range))
                    {
                        BattleVisitor visitor(attacker, observers);
                        defender->accept(visitor);
                    }
                }
            }
        }

        start_range += 10;
    }

    save_to_file("../res.txt");
}

void Arena::clear_npcs()
{
    npcs.clear();
}