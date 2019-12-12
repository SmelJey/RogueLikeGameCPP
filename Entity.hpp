#pragma once
#include "Object.hpp"
#include "Utility.hpp"

#include <vector>
#include <string>
#include <utility>

class Entity : public Object {
public:
    Entity(util::Point coords, char symb);

    virtual std::pair<Object&, Object&> update(util::GameInfo& game) = 0;

    int getHp() const;

    int getMaxHp() const;

    void setMaxHp(int hp);

    int getDmg() const;

    void setDmg(int dmg);

    util::Point getPos() const;

    void setPos(util::Point pos);

    void getHit(int dmg);

protected:
    int maxHp = 10;
    int hp;
    
    int damage = 1;
};
