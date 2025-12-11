#include "Arena.h"

int main()
{
    Arena& arena = Arena::get_instance();

    arena.load_from_file("../input.txt");
    arena.print_survivors();
    arena.battle(500);
}