#include "Game.h"

#include <list>

std::list<Point> snake;
enum Direction {Up, Down, Left, Right};
Direction direction = Direction::Up;
float accumulatedTime;
float timePerTileMoved;
float timeDecrementPerApple = 0.02f;
float lowestTimePerTileMoved = 0.1f;
float delayWhenDead = 2.0f;
bool hasTurned = false;
Point apple;
bool dead = false;
std::list<Direction> moveQueue;
const int maxInputCommands = 4;

bool pointIsInSnake(Point p) {
  for(auto part : snake) {
    if(part.x == p.x && part.y == p.y) {
      return true;
    }
  }

  return false;
}

bool pointIsInWall(Point p) {
  return p.x == 0 || p.x == 31 || p.y == 0 || p.y == 17;
}

void randomizeApple(Screen &screen) {
  do {
    apple.x = rand()%30 + 1;
    apple.y = rand()%16 + 1;
  } while((pointIsInSnake(apple)));
  screen.setTile(apple.x, apple.y, 3);
}

void resetGame(Screen &screen) {
  for(int y = 1;y < 17;y++) {
    for(int x = 1;x < 31;x++) {
      screen.setTile(x, y, 0);
    }
  }

  dead = false;

  accumulatedTime = 0;
  timePerTileMoved = 0.5f;

  snake.clear();
  snake.push_back({16, 9});
  screen.setTile(16, 9, 8);
  snake.push_back({16, 10});
  screen.setTile(16, 10, 17);
  snake.push_back({16, 11});
  screen.setTile(16, 11, 12);

  direction = Direction::Up;

  apple.x = 16;
  apple.y = 9;
  randomizeApple(screen);
}

void
Game::init() {
  srand(time(0));

  // Do one-time setup here
  screen.hideCursor();

  screen.loadTileset("tiles.png");

  screen.setTile(0, 0, 4);
  screen.setTile(31, 0, 5);
  screen.setTile(0, 17, 6);
  screen.setTile(31, 17, 7);
  for(int x = 1;x < 31;x++) {
    screen.setTile(x, 0, 1);
    screen.setTile(x, 17, 1);
  }
  for(int y = 1;y < 17;y++) {
    screen.setTile(0, y, 2);
    screen.setTile(31, y, 2);
  }

  resetGame(screen);
}

void
Game::update(float deltaTime) {
  if(input.quit() || input.pressed(SDLK_ESCAPE)) {
    shouldQuit = true;
  }

  if(input.pressed(SDLK_f)) {
    screen.toggleFullscreen();
  }

  accumulatedTime += deltaTime;

  if(dead) {
    if(accumulatedTime > delayWhenDead) {
      resetGame(screen);
    }
    return;
  }

  while(accumulatedTime > timePerTileMoved) {
    hasTurned = false;

    if(!moveQueue.empty()) {
      direction = moveQueue.front();
      moveQueue.pop_front();
    }

    accumulatedTime -= timePerTileMoved;
    Point head = snake.front();

    switch(direction) {
    case Direction::Up:
      head.y--;
      break;
    case Direction::Down:
      head.y++;
      break;
    case Direction::Left:
      head.x--;
      break;
    case Direction::Right:
      head.x++;
      break;
    }

    Point previousTail = snake.back();
    snake.pop_back();
    screen.setTile(previousTail.x, previousTail.y, 0);

    if((pointIsInSnake(head)) || pointIsInWall(head)) {
      // DED
      dead = true;
      for(auto part : snake) {
	screen.setTile(part.x, part.y, 18);
      }
      screen.drawTiles();
      return;
    }

    snake.push_front(head);

    screen.setTile(head.x, head.y, 8 + direction);

    // select tile depending on where we came from and where we're
    // going

    auto it = snake.begin();
    it++;
    Point secondTailPiece = *it;
    it++;
    Point thirdTailPiece = *it;
    if(head.x == thirdTailPiece.x) {
      screen.setTile(secondTailPiece.x, secondTailPiece.y, 17);
    } else if(head.y == thirdTailPiece.y) {
      screen.setTile(secondTailPiece.x, secondTailPiece.y, 16);
    } else if((head.x > secondTailPiece.x && thirdTailPiece.y > secondTailPiece.y) || (head.y > secondTailPiece.y && thirdTailPiece.x > secondTailPiece.x)) {
	screen.setTile(secondTailPiece.x, secondTailPiece.y, 20);
    } else if((head.x < secondTailPiece.x && thirdTailPiece.y > secondTailPiece.y) || (head.y > secondTailPiece.y && thirdTailPiece.x < secondTailPiece.x)) {
	screen.setTile(secondTailPiece.x, secondTailPiece.y, 21);
    } else if((head.x > secondTailPiece.x && thirdTailPiece.y < secondTailPiece.y) || (head.y < secondTailPiece.y && thirdTailPiece.x > secondTailPiece.x)) {
	screen.setTile(secondTailPiece.x, secondTailPiece.y, 22);
    } else if((head.x < secondTailPiece.x && thirdTailPiece.y < secondTailPiece.y) || (head.y < secondTailPiece.y && thirdTailPiece.x < secondTailPiece.x)) {
	screen.setTile(secondTailPiece.x, secondTailPiece.y, 23);
    }

    // direction depends on where second to last is
    Point last = snake.back();
    auto end = snake.rbegin();
    while((*end).x == last.x && (*end).y == last.y) {
      end++;
    }
    Point secondToLast = *end;

    if(last.x < secondToLast.x) {
      screen.setTile(last.x, last.y, 15);
    } else if(last.x > secondToLast.x) {
      screen.setTile(last.x, last.y, 14);
    } else if(last.y < secondToLast.y) {
      screen.setTile(last.x, last.y, 13);
    } else if(last.y > secondToLast.y) {
      screen.setTile(last.x, last.y, 12);
    }

    if(head.x == apple.x && head.y == apple.y) {
      if(timePerTileMoved > lowestTimePerTileMoved) {
	timePerTileMoved -= timeDecrementPerApple;
      }
      snake.push_back(snake.back());
      snake.push_back(snake.back());
      snake.push_back(snake.back());
      randomizeApple(screen);
    }
  }

  if(moveQueue.size() < maxInputCommands) {
    Direction dirToCheck = moveQueue.empty() ? direction : moveQueue.back();

    if(input.pressed(SDLK_LEFT) && (dirToCheck == Direction::Up || dirToCheck == Direction::Down)) {
      moveQueue.push_back(Direction::Left);
    } else if(input.pressed(SDLK_RIGHT) && (dirToCheck == Direction::Up || dirToCheck == Direction::Down)) {
      moveQueue.push_back(Direction::Right);
    } else if(input.pressed(SDLK_UP) && (dirToCheck == Direction::Left || dirToCheck == Direction::Right)) {
      moveQueue.push_back(Direction::Up);
    } else if(input.pressed(SDLK_DOWN) && (dirToCheck == Direction::Left || dirToCheck == Direction::Right)) {
      moveQueue.push_back(Direction::Down);
    }
  }

  screen.drawTiles();
}
