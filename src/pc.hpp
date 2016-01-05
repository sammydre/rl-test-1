#pragma once

#include "entity.hpp"

struct Character : public Entity
{
};

struct NPC : public Character
{
};

struct Player : public Entity
{
  Player();
};
