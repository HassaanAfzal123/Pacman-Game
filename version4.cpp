#include <SFML/Graphics.hpp>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <ctime>
#include <X11/Xlib.h>
#include <string>
#include <queue>
#include "path.h"
using namespace std;

//menu and start
const int MAZE_WIDTH = 19;
const int MAZE_HEIGHT = 23;
const int PACMAN_SPEED = 1;
string str;


//UI Varables
int lives = 3;
int score = 0;
bool start = false;
int capturex=0;
int capturey=0;


// Semaphores
sem_t emptyb;
sem_t fullb;


int** generateMaze() {
    int** mazeData = new int*[MAZE_HEIGHT];
    for (int i = 0; i < MAZE_HEIGHT; i++) {
        mazeData[i] = new int[MAZE_WIDTH];
    }

  int m[23][19] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 1, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1},
    {4, 4, 4, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 4, 4, 4},
    {4, 4, 4, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 4, 4, 4},
    {1, 1, 1, 1, 0, 1, 0, 1, 1, 3, 1, 1, 0, 1, 0, 1, 1, 1, 1},
    {9, 2, 2, 2, 0, 8, 0, 1, 5, 5, 5, 1, 0, 8, 0, 2, 2, 2, 9},
    {1, 1, 1, 1, 0, 0, 0, 1, 5, 5, 5, 1, 0, 0, 0, 1, 1, 1, 1},
    {4, 4, 4, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 4, 4, 4},
    {4, 4, 4, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 4, 4, 4},
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
    
    sf::Texture wallTexture2;
    if (!wallTexture2.loadFromFile("IMG/l.png")) {
        std::cerr << "Failed to load wall image." << std::endl;
        return;
    }
    
    sf::Texture wallTexture3;
    if (!wallTexture3.loadFromFile("IMG/gate.png")) {
        std::cerr << "Failed to load wall image." << std::endl;
        return;
    }
    
    sf::CircleShape pellet(cellSize / 10);
    pellet.setFillColor(sf::Color(255, 255, 150));

    sf::Sprite wallSprite(wallTexture);
    sf::Sprite wallSprite2(wallTexture2);
    sf::Sprite wallSprite3(wallTexture3);

for (int y = 0; y < MAZE_HEIGHT; y++) {
    for (int x = 0; x < MAZE_WIDTH; x++) {
      if (mazeData[y][x] == 1) {
        wallSprite.setPosition(x * cellSize, y * cellSize);
        window.draw(wallSprite);}
      else if (mazeData[y][x] == 7) {
        wallSprite2.setPosition(x * cellSize, y * cellSize);
        window.draw(wallSprite2);
      } else if (mazeData[y][x] == 3) {
        wallSprite3.setPosition(x * cellSize, y * cellSize);
        window.draw(wallSprite3);   
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

void drawghost(sf::RenderWindow& window,int x, int y,int index){
  float cellSize = getCellSize(window);
    sf::Texture ghostTexture;
    
  if(!eaten){
 if(index==0 || index==3){
  if (!ghostTexture.loadFromFile("IMG/red.png")) {
    std::cerr << "Failed to load wall image." << std::endl;
    return;
  }
 }
 
 if(index==1 || index==2){
  if (!ghostTexture.loadFromFile("IMG/yellow.png")) {
    std::cerr << "Failed to load wall image." << std::endl;
    return;
  }
 }
 
 }
 else if(eaten){
 
   if (!ghostTexture.loadFromFile("IMG/blue.png")) {
    std::cerr << "Failed to load wall image." << std::endl;
    return;
  }
 
 }

  sf::Sprite ghostSprite(ghostTexture);

  ghostSprite.setPosition(x * cellSize, y * cellSize);
  window.draw(ghostSprite);

}

void drawPowerPellets(sf::RenderWindow& window, int ** mazeData) {
    float cellSize = getCellSize(window);
    sf::CircleShape pellet(cellSize / 4); 
    pellet.setFillColor(sf::Color(200, 100, 150));

    float offsetX = (cellSize - pellet.getRadius() * 2) / 2;
    float offsetY = (cellSize - pellet.getRadius() * 2) / 2;

    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            if (mazeData[y][x] == 8) {
                pellet.setPosition(x * cellSize + offsetX, y * cellSize + offsetY);
                window.draw(pellet); 
            }
        }
    }
}

bool isValidMove(int** mazeData, int x, int y) {
  if (x < 0 || x >= MAZE_WIDTH || y < 0 || y >= MAZE_HEIGHT ||
      mazeData[y][x] == 1 || mazeData[y][x] == 9 ||  mazeData[y][x] == 3 ||  mazeData[y][x] == 7 ) {
    return false;
  } else {
    return true;
  }
}

void displayscorefunc(sf::RenderWindow& window){
      sf::Font font;
    if (!font.loadFromFile("IMG/arial.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
        return;
    }

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Red);
    scoreText.setPosition(20, window.getSize().y - 70);
    window.draw(scoreText);
}

void displaylivesfunc(sf::RenderWindow& window){
    sf::Font font;
    if (!font.loadFromFile("IMG/arial.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
        return;
    }

    sf::Text livesText;
    livesText.setFont(font);
    livesText.setString("Lives: " + std::to_string(lives));
    livesText.setCharacterSize(24);
    livesText.setFillColor(sf::Color::Red);
    livesText.setPosition(300, window.getSize().y - 70);
    window.draw(livesText);
}

void menu(sf::RenderWindow& window){
   while (window.isOpen()) {

    sf::Font font;
    if (!font.loadFromFile("IMG/arial.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
        return;
    }
    
    sf::Texture texture;
    if (!texture.loadFromFile("IMG/title.png")) {
        std::cerr << "Failed to load image." << std::endl;
        return;
    }
    
    sf::Sprite sprite(texture);
    float scale = std::min(window.getSize().x / (float)texture.getSize().x, window.getSize().y / (float)texture.getSize().y);
    sprite.setScale(scale, scale);
    sprite.setPosition(window.getSize().x / 2 - sprite.getGlobalBounds().width / 2, 120);
    window.draw(sprite);
    
    sf::Text titleText("PACMAN GAME", font, 48);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(window.getSize().x / 2 - titleText.getGlobalBounds().width / 2, 50);
    window.draw(titleText);

    sf::Text promptText11("Instructions", font, 24);
    promptText11.setFillColor(sf::Color::Red);
    promptText11.setPosition(140, window.getSize().y - 220);
    window.draw(promptText11);

    sf::Text promptText1("Use arrow keys to save pacman!", font, 20);
    promptText1.setFillColor(sf::Color::Blue);
    promptText1.setPosition(60, window.getSize().y - 180);
    window.draw(promptText1);
    
    sf::Text promptText2("Eat power pellets for extra points!", font, 20);
    promptText2.setFillColor(sf::Color::Blue);
    promptText2.setPosition(60, window.getSize().y - 150);
    window.draw(promptText2);
    
    sf::Text promptText3("Eat blue ghosts for extra points!", font, 20);
    promptText3.setFillColor(sf::Color::Blue);
    promptText3.setPosition(60, window.getSize().y - 120);
    window.draw(promptText3);
        
    
    sf::Text promptText("Press S to start the game", font, 24);
    promptText.setFillColor(sf::Color::White);
    promptText.setPosition(70, window.getSize().y - 40);
    window.draw(promptText);
    
   
    
    sf::Event event;
    while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
          case sf::Keyboard::S:
                 start=true;
                 window.close();
            break;
           }
         } 
        } 
        
     window.display();  
    }
}


void gameLoop(sf::RenderWindow& window, int** mazeData){
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
       for(int i=0;i<4;i++)
         drawghost(window,ghostposx[i],ghostposy[i],i);
       drawPowerPellets(window,mazeData);
       displaylivesfunc(window);
       displayscorefunc(window);
              
       window.display(); 
      }      
        
}


void scorefunc(int ** mazeData){
     if ( mazeData[pacmany][pacmanx] == 0){
        score++;
        mazeData[pacmany][pacmanx] = 2;        
     }
}

void powerpellet(int ** mazeData){
   static sf::Clock clock1; 
   if (!eaten && mazeData[pacmany][pacmanx] == 8){
        score=score+10;
        mazeData[pacmany][pacmanx] = 2;  
        capturex=pacmany;
        capturey=pacmanx;
        eaten=true;  
        clock1.restart();                
     }
     
    if(eaten && clock1.getElapsedTime().asSeconds() >= 15){
         eaten=false; 
         mazeData[capturex][capturey]=8;              
     }
        
}

void livesfunc(int ** mazeData){


}



void ghostmove(int ** mazeData,int a){
   static sf::Clock clock;
  if (clock.getElapsedTime().asMilliseconds() >= 500) {
   if(swapper[a]){
     ghostposy[a]-=GHOST_SPEED;
     swapper[a]=false;
    }
    else if(!swapper[a]){
      ghostposy[a]+=GHOST_SPEED;
      swapper[a]=true;
     }
     clock.restart(); 
   }       
}


void * GhostCage4(void* arg) {
    while (true) {
       if(ghostkey[3]==false){
           // initally draw in cage
        }
        else{
               ghost4algo();
        }
    }  
      
    pthread_exit(NULL); 
    return NULL;    
}

void * GhostCage3(void* arg) {
    while (true) {
       if(ghostkey[2]==false){
          
        }
        else{
               ghost3algo();
        }
    }    
    
    pthread_exit(NULL); 
    return NULL;    
}




void * GhostCage2(void* arg) {
    while (true) {
        if(ghostkey[1]==false)
           ghostmove(mazeData,1);  
        else{
             ghost2algo();
        }    
    }    
    pthread_exit(NULL); 
    return NULL;    
}

void * GhostCage1(void* arg) {
    while (true) { 
        if(ghostkey[0]==false)
           ghostmove(mazeData,0);  
        else{
            ghost1algo();
        }   
             
    }    
    pthread_exit(NULL); 
    return NULL;    
}


void* UserInterface(void* arg) {

    
   // if(start==false){
  //     sf::RenderWindow window(sf::VideoMode(MAZE_WIDTH * 22, MAZE_HEIGHT * 26), "PACMAN GAME");
  //     menu(window);
  //  }

 
    while (true) {
        scorefunc(mazeData);      
        powerpellet(mazeData);
    }
    

    pthread_exit(NULL); 
    return NULL;
}


void* gameEngine(void* arg) {

  while(true){
   
  //  if(start){
      sf::RenderWindow window(sf::VideoMode(MAZE_WIDTH * 22, MAZE_HEIGHT * 26), "PACMAN GAME");
      gameLoop(window, mazeData);
  //    } 
           
   }
      
    pthread_exit(NULL); 
    return NULL;
}


int main() {
   
    sem_init(&emptyb, 0, 1);
    sem_init(&fullb, 0, 0);

    mazeData = generateMaze();
    XInitThreads();
   
    pthread_t gameEngineThread,UserInterfaceThread;
    pthread_t ghostThread[3];
    pthread_create(&gameEngineThread, NULL, gameEngine,NULL);
    pthread_create(&UserInterfaceThread, NULL, UserInterface,NULL);
    pthread_create(&ghostThread[0], NULL, GhostCage1,NULL);
    pthread_create(&ghostThread[1], NULL, GhostCage2,NULL);
    pthread_create(&ghostThread[2], NULL, GhostCage3,NULL);
    pthread_create(&ghostThread[3], NULL, GhostCage4,NULL);
   
    sem_destroy(&emptyb);
    sem_destroy(&fullb);
        
    pthread_exit(NULL);  
    return 0;
    
}    
    
