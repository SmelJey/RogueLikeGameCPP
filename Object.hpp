#pragma once

#include "Utility.hpp"


class Object {
public:
    Object();
    Object(char ch, int id, util::Point position);

    util::Point getPos() const;
    char getSym() const;
    bool isEnabled() const;

    void setEnabled(bool val = true);

    void draw(util::GameInfo& game) const;

    virtual void update(util::GameInfo& game) = 0;

    virtual void interact(Enemy&, util::GameInfo&);
    virtual void interact(Player&, util::GameInfo&);
    virtual void interact(Projectile&, util::GameInfo&);

    virtual Object& getRef() = 0;

    friend bool operator<(const Object& lhs, const Object& rhs);
    bool operator==(const Object& rhs) const;
    bool operator!=(const Object& rhs) const;
protected:
    util::Point pos;
    char symbol;
    bool enabled = true;
    int id;
};

template<typename T>
class ObjectWrapper : public Object {
    ObjectWrapper(T& object) : obj(object) {}
    T& obj;
};
