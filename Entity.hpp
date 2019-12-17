#pragma once
#include "Object.hpp"
#include "Utility.hpp"

#include <vector>
#include <string>
#include <utility>

class Entity : public Object {
public:
    Entity(char sym, int maxHp, int dmg, int id = -1, util::Point pos = util::Point(-1, -1));

    int getHp() const;

    int getMaxHp() const;

    void setMaxHp(int hp);

    void setHp(int hp);

    int getDmg() const;

    virtual void interact(Projectile&, util::GameInfo&) override;
    virtual void update(util::GameInfo& game) = 0;

    void setDmg(int dmg);

    util::Point getPos() const;

    void setPos(util::Point pos);

    void getHit(int dmg, util::GameInfo& game);

protected:

    Object& findCollision(util::Point pos, util::GameInfo& game);

    int maxHp = 10;
    int hp;
    
    int damage = 1;
};
