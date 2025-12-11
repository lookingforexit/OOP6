#ifndef VISITOR_H
#define VISITOR_H

#include <memory>
#include <vector>
#include "Observer.h"
#include "NPC.h"

class INPCVisitor
{
public:
    virtual void visit(Dragon& dragon) = 0;
    virtual void visit(Frog& frog) = 0;
    virtual void visit(Knight& knight) = 0;

    virtual ~INPCVisitor() = default;
};

class BattleVisitor final: public INPCVisitor
{
private:
    std::shared_ptr<NPC> attacker;
    std::vector<std::shared_ptr<IObserver>>& observers;
public:
    BattleVisitor(std::shared_ptr<NPC> attacker, std::vector<std::shared_ptr<IObserver>>& observers);
    ~BattleVisitor() override = default;
public:
    void visit(Dragon& defender) override;
    void visit(Frog& defender) override;
    void visit(Knight& defender) override;
public:
    void notify(const std::string& victim_type) const;
};

#endif //VISITOR_H