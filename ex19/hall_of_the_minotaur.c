#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "hall_of_the_minotaur.h"

// Monster implementation

int Monster_attack(void *self, int damage) {
  assert(self != NULL);
  assert(damage >= 0);

  Monster *monster = self;

  if (monster->hit_points <= 0) {
    puts("It's already dead!");
    return 1;
  }

  monster->hit_points -= damage;
  printf("You attack %s for %d damage!\n", monster->_(description), damage);

  if (monster->hit_points > 0) {
    printf("It's still alive with %d health!", monster->hit_points);
    return 0;
  } else {
    puts("You killed it!");
    return 1;
  }
}

int Monster_init(void *self) {
  assert(self != NULL);

  Monster *monster = self;
  monster->hit_points = 10;
  return 1;
}

Object MonsterProto = {
  .init = Monster_init,
  .attack = Monster_attack
};

// Room implementation

void *Room_move(void *self, Direction direction) {
  assert(self != NULL);

  Room *room = self;
  Room *next = NULL;

  if (direction == NORTH && room->north) {
    puts("You go north, into:\n");
    next = room->north;
  } else if (direction == SOUTH && room->south) {
    puts("You go south, into:\n");
    next = room->south;
  } else if (direction == EAST && room->east) {
    puts("You go east, into:\n");
    next = room->east;
  } else if (direction == WEST && room->west) {
    puts("You go west, into:\n");
    next = room->west;
  } else {
    puts("You can't go in that direction.");
    next = NULL;
  }

  if (next) {
    next->_(describe)(next);
  }

  return next;
}

int Room_attack(void *self, int damage) {
  assert(self != NULL);
  assert(damage >= 0);

  Room *room = self;
  Monster *monster = room->baddie;

  if (monster) {
    monster->_(attack)(monster, damage);
    return 1;
  } else {
    puts("You flail in the air at nothing.");
    return 0;
  }
}

Object RoomProto = {
  .move = Room_move,
  .attack = Room_attack
};

// Map implementation

void *Map_move(void *self, Direction direction) {
  assert(self != NULL);

  Map *map = self;
  Room *location = map->location;
  Room *next = NULL;

  assert(map->location != NULL);

  next = location->_(move)(location, direction);

  if (next) {
    map->location = next;
  }

  return next;
}

int Map_attack(void *self, int damage) {
  assert(self != NULL);
  assert(damage >= 0);

  Map *map = self;
  Room *location = map->location;

  assert(location != NULL);

  return location->_(attack)(location, damage);
}

int Map_init(void *self) {
  assert(self != NULL);

  Map *map = self;

  // some rooms
  Room *hall = NEW(Room, "The Great Hall");
  Room *throne = NEW(Room, "The Throne Room");
  Room *arena = NEW(Room, "The Area... with the minotaur");
  Room *kitchen = NEW(Room, "The Kitchen... with the knife");

  // put the baddie in the arena
  arena->baddie = NEW(Monster, "the evil but misunderstood minotaur");

  // setup rooms on the map
  hall->north = throne;

  throne->west = arena;
  throne->east = kitchen;
  throne->south = hall;

  arena->east = throne;
  kitchen->west = throne;

  // start off in the hall
  map->start = hall;
  map->location = hall;

  return 1;
}

Object MapProto = {
  .init = Map_init,
  .move = Map_move,
  .attack = Map_attack
};