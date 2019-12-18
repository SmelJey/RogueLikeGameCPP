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
    void setHp(int hp);

    int getMaxHp() const;
    void setMaxHp(int hp);
 
    void setDmg(int dmg);
    int getDmg() const;

    virtual void interact(Projectile&, util::GameInfo&) override;
    virtual void update(util::GameInfo& game) = 0;

    void getHit(int dmg, util::GameInfo& game);

protected:

    Object& findCollision(util::Point pos, util::GameInfo& game) const;

    int maxHp = 10;
    int hp;
    
    int damage = 1;
};
