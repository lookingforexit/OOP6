#include "Visitor.h"

#include <utility>

BattleVisitor::BattleVisitor(std::shared_ptr<NPC> attacker, std::vector<std::shared_ptr<IObserver>>& observers) :
                            attacker(std::move(attacker)), observers(observers) {}

void BattleVisitor::visit(Dragon& defender)
{
    if (attacker->get_type() == "Frog" || attacker->get_type() == "Knight")
    {
        defender.is_alive = false;
        notify(defender.get_type());
    }
}

void BattleVisitor::visit(Frog& defender)
{
    if (attacker->get_type() == "Frog")
    {
        defender.is_alive = false;
        notify(defender.get_type());
    }
}

void BattleVisitor::visit(Knight& defender)
{
    if (attacker->get_type() == "Frog" || attacker->get_type() == "Dragon")
    {
        defender.is_alive = false;
        notify(defender.get_type());
    }
}

void BattleVisitor::notify(const std::string& victim_type) const
{
    for (const auto& observer : observers)
    {
        observer->on_kill(attacker->get_type(), victim_type);
    }
}