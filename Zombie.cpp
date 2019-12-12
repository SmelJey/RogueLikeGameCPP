#pragma once

#include "Enemy.hpp"
#include "Utility.hpp"

#include "Zombie.hpp"

Zombie::Zombie(util::Point pos, const Entity& player) : Enemy(pos, 'Z', player) {}
