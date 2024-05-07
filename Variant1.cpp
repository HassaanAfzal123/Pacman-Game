#include <SFML/Graphics.hpp>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <ctime>
#include <signal.h>
using namespace std;

const int MAZE_WIDTH = 19; // Adjusted the size to match the array size
const int MAZE_HEIGHT = 23; // Adjusted the size to match the array size
const int PACMAN_SPEED = 1; 

int pacmanx=17;
int pacmany=10;

int** generateMaze() {
    int** mazeData = new int*[MAZE_HEIGHT];
    for (int i = 0; i < MAZE_HEIGHT; i++) {
        mazeData[i] = new int[MAZE_WIDTH];
    }

  int m[23][19] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1},
    {2, 2, 2, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 2, 2, 2},
    {2, 2, 2, 1, 0, 1, 4, 4, 4, 4, 4, 4, 4, 1, 0, 1, 2, 2, 2},
    {1, 1, 1, 1, 0, 1, 4, 1, 1, 3, 1, 1, 4, 1, 0, 1, 1, 1, 1},
    {9, 2, 2, 2, 0, 0, 4, 1, 5, 5, 5, 1, 4, 0, 0, 2, 2, 2, 9},
    {1, 1, 1, 1, 0, 0, 4, 1, 5, 5, 5, 1, 4, 0, 0, 1, 1, 1, 1},
    {2, 2, 2, 1, 0, 1, 4, 1, 1, 1, 1, 1, 4, 1, 0, 1, 2, 2, 2},
    {2, 2, 2, 1, 0, 1, 4, 4, 4, 4, 4, 4, 4, 1, 0, 1, 2, 2, 2},
    {1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1},
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

    for (int i = 0; i < 23; i++)
		{
			for (int j = 0; j < 19; j++)
			{
				mazeData[i][j] = m[i][j];
			}
		}


    return mazeData;
}

float getCellSize(sf::RenderWindow& window) {
  int windowSize = std::min(window.getSize().x, window.getSize().y);
  float cellSize = static_cast<float>(windowSize) / MAZE_WIDTH;
  return cellSize;
}

void drawMaze(sf::RenderWindow& window, int** mazeData) {
      float cellSize = getCellSize(window);

    sf::Texture wallTexture;
    if (!wallTexture.loadFromFile("IMG/wall.png")) {
        std::cerr << "Failed to load wall image." << std::endl;
        return;
    }
    
    sf::CircleShape pellet(cellSize / 10);
    pellet.setFillColor(sf::Color(255, 255, 150));

    sf::Sprite wallSprite(wallTexture);

for (int y = 0; y < MAZE_HEIGHT; y++) {
    for (int x = 0; x < MAZE_WIDTH; x++) {
      if (mazeData[y][x] == 1) {
        wallSprite.setPosition(x * cellSize, y * cellSize);
        window.draw(wallSprite);
      } else if (mazeData[y][x] == 0) {
        pellet.setPosition(x * cellSize + cellSize / 2, y * cellSize + cellSize / 2);
        window.draw(pellet);
      }
    }
  }
}

void drawpacman(sf::RenderWindow& window,int x, int y){
  float cellSize = getCellSize(window);

  sf::Texture pacmanTexture;
  if (!pacmanTexture.loadFromFile("IMG/pacman.png")) {
    std::cerr << "Failed to load wall image." << std::endl;
    return;
  }

  sf::Sprite pacmanSprite(pacmanTexture);

  pacmanSprite.setPosition(x * cellSize, y * cellSize);
  window.draw(pacmanSprite);

}

bool isValidMove(int** mazeData, int x, int y) {
  if (x < 0 || x >= MAZE_WIDTH || y < 0 || y >= MAZE_HEIGHT ||
      mazeData[y][x] == 1 || mazeData[y][x] == 9) {
    return false;
  } else {
    return true;
  }
}

void gameLoop(sf::RenderWindow& window, int** mazeData) {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
          case sf::Keyboard::Up:
            if (isValidMove(mazeData, pacmanx, pacmany - 1)) {
              pacmany -= PACMAN_SPEED; 
            }
            break;
          case sf::Keyboard::Down:
            if (isValidMove(mazeData, pacmanx, pacmany + 1)) {
              pacmany += PACMAN_SPEED; 
            }
            break;
          case sf::Keyboard::Left:
            if (isValidMove(mazeData, pacmanx - 1, pacmany)) {
              pacmanx -= PACMAN_SPEED; 
            }
            break;
          case sf::Keyboard::Right:
            if (isValidMove(mazeData, pacmanx + 1, pacmany)) {
              pacmanx += PACMAN_SPEED; 
            }
            break;
          default:
     
            break;
         }
        }
       }
        window.clear(sf::Color::Black);
        drawMaze(window, mazeData);
        drawpacman(window,pacmanx,pacmany);
        window.display();
    }
}

void* gameEngine(void* arg) {
    srand(time(0));

    int** mazeData = generateMaze();

    sf::RenderWindow window(sf::VideoMode(MAZE_WIDTH * 22, MAZE_HEIGHT * 26), "Pacman Game");
    gameLoop(window, mazeData);

    pthread_exit(NULL); 
    return NULL;
}


int main() {
   
    pthread_t gameEngineThread;
    pthread_create(&gameEngineThread, NULL, gameEngine, NULL);

    
    pthread_exit(NULL);  
    return 0;
    
}    
    

