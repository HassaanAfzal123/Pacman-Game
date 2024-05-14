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
static sf::Clock clock1;
static sf::Clock clock123;
static sf::Clock clock1234;

//UI Varables
int lives = 3;
int score = 0;
bool start = false;
int capturex=0;
int capturey=0;
bool booster1state=0;
bool booster2state=0;
int capturexg1=0;
int captureyg1=0;
int capturexg2=0;
int captureyg2=0;
bool showkey1=false;
bool showkey2=false;

// Synchrnosiation
sem_t writesem;
sem_t mutex;
int s1Count=0;

sem_t mutex2;
sem_t keysem;
sem_t permitsem;

sem_t consem; 
sem_t prodsem;

sem_t consem2;
sem_t prodsem2;

sem_t consem3;
sem_t prodsem3;

int buffer=0;
int buffer2=0;
int buffer3=0;

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
    {1, 1, 1, 1, 0, 1, 0, 1, 3, 3, 3, 1, 0, 1, 0, 1, 1, 1, 1},
    {9, 2, 2, 2, 0, 8, 0, 1, 5, 5, 5, 1, 0, 8, 0, 2, 2, 2, 9},
    {1, 1, 1, 1, 0, 0, 0, 1, 5, 5, 5, 1, 0, 0, 0, 1, 1, 1, 1},
    {4, 4, 4, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 4, 4, 4},
    {4, 4, 4, 1, 0, 1, 0, 0, 0, 11, 0, 0, 0, 1, 0, 1, 4, 4, 4},
    {1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 1, 0, 0, 1},
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

void drawbooster(sf::RenderWindow& window, int ** mazeData) {
    float cellSize = getCellSize(window);
    sf::CircleShape pellet(cellSize / 4); 
    pellet.setFillColor(sf::Color::White);

    float offsetX = (cellSize - pellet.getRadius() * 2) / 2;
    float offsetY = (cellSize - pellet.getRadius() * 2) / 2;

    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            if (mazeData[y][x] == 6 || mazeData[y][x] == 11) {
                pellet.setPosition(x * cellSize + offsetX, y * cellSize + offsetY);
                window.draw(pellet); 
            }
        }
    }
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
    
    if(showkey1 || showkey2){
 
       sf::Font font2;
    if (!font2.loadFromFile("IMG/arial.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
        return;
    }

    sf::Text boost;
    boost.setFont(font);
    boost.setString("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    boost.setCharacterSize(24);
    boost.setFillColor(sf::Color::Red);
    boost.setPosition(100, window.getSize().y - 30);
    window.draw(boost);   
    
        if(showkey1)
            showkey1=false;
        if(showkey2)
            showkey2=false;    
    }
}

void checkend(sf::RenderWindow& window){
    if(lives==0){
          
          sf::Font font;
          if (!font.loadFromFile("IMG/arial.ttf")) {
              std::cerr << "Failed to load font." << std::endl;
              return;
           }

         sf::Text end;
         end.setFont(font);
         end.setString("GAME END");
         end.setCharacterSize(25);
         end.setFillColor(sf::Color::Yellow);
         end.setPosition(130, window.getSize().y - 90);
         window.draw(end);
                    
    }

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

void pacmanmovement(sf::RenderWindow& window){

      sem_wait(&writesem);   // requests for critical section
    
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
       
       sem_post(&writesem);   // leaves critical section

}

void gameLoop(sf::RenderWindow& window, int** mazeData){
    while (window.isOpen()) {
         
       pacmanmovement(window);
       
     
       window.clear(sf::Color::Black);
       drawMaze(window, mazeData);
       drawpacman(window,pacmanx,pacmany);
       for(int i=0;i<4;i++)
         drawghost(window,ghostposx[i],ghostposy[i],i);
       drawPowerPellets(window,mazeData);
       drawbooster(window,mazeData);
       checkend(window);
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

void put3(int item) {
    sem_wait(&prodsem3);
    if (buffer3 == 0) {
         buffer3 = item;
      }
    sem_post(&consem3);
}

int get3() {
    sem_wait(&consem3);
    int item = -1;
        if (buffer3 != 0) {
            item = buffer3;
            buffer2 = 0;
        }
    sem_post(&prodsem3);
    return item;
}


void consumebooster2(int i){ 
   if (mazeData[ghostposy[i]][ghostposx[i]] == 6){
   
        mazeData[ghostposy[i]][ghostposx[i]] = 2;  
        capturexg2 = ghostposy[i];
        captureyg2 = ghostposx[i];
        booster2state = true;
        showkey1=true;  
        GHOST_SPEED[i]=0.25;
        clock1234.restart(); 
        int item = get3();                     
     }
}

void producebooster2(){    
    if(booster2state && clock1234.getElapsedTime().asSeconds() >=5){
         booster2state = false; 
         GHOST_SPEED[2]=0.5;
         mazeData[capturexg2][captureyg2] = 6;
         put3(0);     
     }        
}



void put2(int item) {
    sem_wait(&prodsem2);
    if (buffer2 == 0) {
         buffer2 = item;
      }
    sem_post(&consem2);
}

int get2() {
    sem_wait(&consem2);
    int item = -1;
        if (buffer2 != 0) {
            item = buffer2;
            buffer2 = 0;
        }
    sem_post(&prodsem2);
    return item;
}


void consumebooster1(int i){ 
   if (mazeData[ghostposy[i]][ghostposx[i]] == 11){
   
        mazeData[ghostposy[i]][ghostposx[i]] = 2;  
        capturexg1 = ghostposy[i];
        captureyg1 = ghostposx[i];
        showkey2=true;  
        booster1state = true; 
        GHOST_SPEED[i]=0.25; 
        clock123.restart(); 
        int item = get2();                     
     }
}

void producebooster1(){    
    if(booster1state && clock123.getElapsedTime().asSeconds() >=5){
         booster1state = false; 
         GHOST_SPEED[0]=0.5;
         mazeData[capturexg1][captureyg1] = 11;
         put2(0);     
     }        
}


void put(int item) {
    sem_wait(&prodsem);
    if (buffer == 0) {
         buffer = item;
      }
    sem_post(&consem);
}

int get() {
    sem_wait(&consem);
    int item = -1;
        if (buffer != 0) {
            item = buffer;
            buffer = 0;
        }
    sem_post(&prodsem);
    return item;
}


void consumepellet(){ 
   if (!eaten && mazeData[pacmany][pacmanx] == 8){
        score += 10;
        mazeData[pacmany][pacmanx] = 2;  
        capturex = pacmany;
        capturey = pacmanx;
        eaten = true;  
        clock1.restart(); 
        int item = get();                     
     }
}

void producepellet(){     
    if(eaten && clock1.getElapsedTime().asSeconds() >=10){
         eaten = false; 
         mazeData[capturex][capturey] = 8;
         put(0);    
     }        
}

void livesfunc(int ** mazeData){

  for(int i=0;i<4;i++){
   if(ghostposx[i]==pacmanx && ghostposy[i]==pacmany && !eaten){
      lives--;
      pacmanx=17;
      pacmany=10;  
      break;  
    }
   }      
}


void ghostmove(int ** mazeData,int a){        // inside the cage
   static sf::Clock clock;
  if (clock.getElapsedTime().asMilliseconds() >= 500) {
   if(swapper[a]){
     ghostposy[a]-=GHOST_SPEED[a];
     swapper[a]=false;
    }
    else if(!swapper[a]){
      ghostposy[a]+=GHOST_SPEED[a];
      swapper[a]=true;
     }
     clock.restart(); 
   }       
}

void ghostmovement(int i){

      sem_wait(&mutex); 
      s1Count++;             // reader added
      if (s1Count == 1) {
          sem_wait(&writesem);  // ensures no writer can enter
      }
      sem_post(&mutex);

      if(i==0)
         ghost1algo(); 
      else if(i==1)
         ghost2algo(); 
      else if(i==2)
         ghost3algo(); 
      else if(i==3)
         ghost4algo(); 
         
       sem_wait(&mutex); 
       s1Count--;          // reader leaves
       if (s1Count == 0) {
           sem_post(&writesem);   // now writer can enter
       }
       sem_post(&mutex);
}

void ghostdeath(int i){

     if(ghostposx[i]==pacmanx && ghostposy[i]==pacmany && eaten){
        if(i==0){
          ghostposx[i]=8;
          ghostposy[i]=10;
          die[i]=true; 
          flicker1=false;   
         }
         if(i==1){
          ghostposx[i]=10;
          ghostposy[i]=11;  
          die[i]=true;   
          flicker2=false;       
         }
        else if(i==2){
          ghostposx[i]=9;
          ghostposy[i]=10;
          die[i]=true;   
        }
        else if(i==3){
          ghostposx[i]=9;
          ghostposy[i]=11;
          die[i]=true;   
        }
          score+=5;    
     }
}

void cagefree(int i){
    sem_wait(&mutex2); 
    
    sem_wait(&keysem);
    sem_wait(&permitsem);
    
    ghostkey[i] = true; 
    
    sem_post(&mutex2); 
    
    
    if (die[i]) {
        sem_wait(&mutex2); 
        ghostkey[i] = false; 
        die[i]=false;
        sem_post(&mutex2); 
    }     
}

void* releaseP_K(void*) {

   while (true) {
     if(start){
      static sf::Clock clock100;
      if (clock100.getElapsedTime().asSeconds() > 5) {
        sem_post(&keysem); // Release the key
        sem_post(&permitsem); // Release the permit
        clock100.restart();
       } 
   
     } 
   } 
    pthread_exit(NULL); 
    return NULL;   
}

void consumebooster(int i){
    consumebooster1(i);
    consumebooster2(i);
}

void * Ghost4(void* arg) {
    while (true) {
     if(start){
       if (die[3]) {
          ghostkey[3] = false; 
        }
       if(ghostkey[3]==false){
          cagefree(3);
        }
        else{
               ghostmovement(3);
               ghostdeath(3);
                consumebooster(3);
       }                     
     }  
    }  
    pthread_exit(NULL); 
    return NULL;    
}

void * Ghost3(void* arg) {
    while (true) {
     if(start){
       if (die[2]) {
          ghostkey[2] = false; 
        }
       if(ghostkey[2]==false){

          cagefree(2);
        }
        else{             
              ghostmovement(2);
              ghostdeath(2);
              consumebooster(2);
        }
      }    
    }
    pthread_exit(NULL); 
    return NULL;    
}




void * Ghost2(void* arg) {
    while (true) {
     
    if(start){
        if (die[1]) {
          ghostkey[1] = false; 
        }
        if(ghostkey[1]==false){
           if(flicker2){
             ghostmove(mazeData,1);  
           }
           cagefree(1);
           }
        else{            
            ghostmovement(1);
            ghostdeath(1);
             consumebooster(1);
        }    
      } 
    }   
    pthread_exit(NULL); 
    return NULL;    
}

void * Ghost1(void* arg) {
    while (true) { 
    
    if(start){
        if (die[0]) {
          ghostkey[0] = false; 
        }
        if(ghostkey[0]==false){
            if(flicker1){
               ghostmove(mazeData,0);  
             }
           cagefree(0);
           }
        else{
            ghostmovement(0);
            ghostdeath(0);
            consumebooster(0);
        }   
             
      } 
    }   
    pthread_exit(NULL); 
    return NULL;    
}


void* UserInterface(void* arg) {

    
    if(start==false){
       sf::RenderWindow window(sf::VideoMode(MAZE_WIDTH * 22, MAZE_HEIGHT * 26), "PACMAN GAME");
       menu(window);
    }

 
    while (true) {
        livesfunc(mazeData);
        scorefunc(mazeData);      
        consumepellet();
        producepellet();
        producebooster1();
        producebooster2();
    }
    

    pthread_exit(NULL); 
    return NULL;
}


void* gameEngine(void* arg) {

  while(true){
   
   if(start){
      sf::RenderWindow window(sf::VideoMode(MAZE_WIDTH * 22, MAZE_HEIGHT * 26), "PACMAN GAME");
      gameLoop(window, mazeData);
     } 
           
   }
      
    pthread_exit(NULL); 
    return NULL;
}


int main() {

    sem_init(&writesem, 0, 1);
    sem_init(&mutex, 0, 1);
    
    sem_init(&consem, 0, 1);
    sem_init(&prodsem, 0, 1);
    
    sem_init(&keysem, 0, 2);
    sem_init(&permitsem, 0, 2);
    sem_init(&mutex2, 0, 1);
    
    sem_init(&consem2, 0, 1);
    sem_init(&prodsem2, 0, 1);
    
    sem_init(&consem3, 0, 1);
    sem_init(&prodsem3, 0, 1);

    mazeData = generateMaze();
    XInitThreads();
   
    pthread_t gameEngineThread,UserInterfaceThread;
    pthread_t ghostThread[3];
    pthread_create(&gameEngineThread, NULL, gameEngine,NULL);
    pthread_create(&UserInterfaceThread, NULL, UserInterface,NULL);
    pthread_create(&ghostThread[0], NULL, Ghost1,NULL);
    pthread_create(&ghostThread[1], NULL, Ghost2,NULL);
    pthread_create(&ghostThread[2], NULL, Ghost3,NULL);
    pthread_create(&ghostThread[3], NULL, Ghost4,NULL);
    
    pthread_t releasePK;
    pthread_create(&releasePK, NULL, releaseP_K, NULL);
   
    sem_destroy(&mutex);
    sem_destroy(&writesem);
    sem_destroy(&keysem);
    sem_destroy(&permitsem);
    sem_destroy(&mutex2);
    sem_destroy(&prodsem);
    sem_destroy(&consem);
    sem_destroy(&prodsem2);
    sem_destroy(&consem2);
    sem_destroy(&prodsem3);
    sem_destroy(&consem3);
        
    pthread_exit(NULL);  
    return 0;
    
}    
    
