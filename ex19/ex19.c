#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "hall_of_the_minotaur.h"
#include "object.h"

int process_input(Map *game) {
  assert(game != NULL);
  printf("\n> ");

  char ch = getchar();
  getchar(); // eat the Enter key

  int damage = (rand() % 4) + 1;

  switch(ch) {
    case EOF:
      puts("Giving up already?!?!?");
      return 0;
      break;

    case 'n':
      game->_(move)(game, NORTH);
      break;
    case 's':
      game->_(move)(game, SOUTH);
      break;
    case 'e':
      game->_(move)(game, EAST);
      break;
    case 'w':
      game->_(move)(game, WEST);
      break;

    case 'a':
      game->_(attack)(game, damage);
      break;

    case 'l':
      puts("You can go:");
      if (game->location->north) puts("North");
      if (game->location->south) puts("South");
      if (game->location->east) puts("East");
      if (game->location->west) puts("West");
      break;

    default:
      printf("What? I don't understand '%c'.", ch);
  }

  return 1;
}

int main(int argc, char *argv[]) {
  // setup the random seed
  srand(time(NULL));

  // make a map
  Map *game = NEW(Map, "The Hall of the Minotaur");

  printf("You enter the ");
  game->location->_(describe)(game->location);

  // game loop, exits when return is 0
  while(process_input(game)) {}

  game->_(destroy)(game);

  return 0;
}