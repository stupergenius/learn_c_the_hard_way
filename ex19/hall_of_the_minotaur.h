#ifndef _hall_of_the_minotaur_h
#define _hall_of_the_minotaur_h

#include "object.h"

// Monster types

struct Monster {
  Object proto;
  int hit_points;
};

typedef struct Monster Monster;

Object MonsterProto;

// Room types

struct Room {
  Object proto;

  Monster *baddie;

  struct Room *north;
  struct Room *south;
  struct Room *east;
  struct Room *west;
};

typedef struct Room Room;

Object RoomProto;

// Map types

struct Map {
  Object proto;
  Room *start;
  Room *location;
};

typedef struct Map Map;

Object MapProto;

#endif