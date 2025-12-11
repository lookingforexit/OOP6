#ifndef ARENA_H
#define ARENA_H

#include <memory>
#include <vector>
#include "Observer.h"
#include "NPC.h"

class Arena final
{
private:
    std::vector<std::shared_ptr<NPC>> npcs;
    std::vector<std::shared_ptr<IObserver>> observers;
private:
    Arena();
public:
    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;
public:
    static Arena& get_instance();
public:
    void add_npc(const std::string& type, int x, int y);
public:
    void save_to_file(const std::string& filename) const ;
    void load_from_file(const std::string& filename);
public:
    void print_survivors() const;
public:
    void battle(size_t distance);
public:
    void clear_npcs();
};

#endif //ARENA_H