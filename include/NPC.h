#ifndef NPC_H
#define NPC_H

#include <string>

class INPCVisitor;

class NPC
{
public:
    int x;
    int y;
    bool is_alive = true;
public:
    NPC(int x, int y);
    virtual ~NPC() = default;
public:
    virtual void accept(INPCVisitor& visitor) = 0;
public:
    virtual std::string get_type() const = 0;
public:
    bool is_close(const NPC& other, size_t distance) const;
};

class Dragon final: public NPC
{
public:
    Dragon(int x, int y);
    ~Dragon() override = default;

    void accept(INPCVisitor& visitor) override;
    std::string get_type() const override;
};

class Frog final: public NPC
{
public:
    Frog(int x, int y);
    ~Frog() override = default;

    void accept(INPCVisitor& visitor) override;
    std::string get_type() const override;
};

class Knight final: public NPC
{
public:
    Knight(int x, int y);
    ~Knight() override = default;

    void accept(INPCVisitor& visitor) override;
    std::string get_type() const override;
};

#endif //NPC_H