#include <SFML/Graphics.hpp>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <ctime>
#include <string>
using namespace std;

// maza and map
int** mazeData;
int pacmanx=17;
int pacmany=10;
bool eaten=false;

// Ghost Variables
float GHOST_SPEED[4] = {0.5,0.5,0.5,0.5}; 
bool ghostkey[4]={false,false,false,false};        
int ghostposx[4]={8,10,9,9};           
int ghostposy[4]={10,11,10,11};
bool swapper[2]={false,true};
bool die[4]={false,false,false,false};
bool reversed=false;
bool reversed2=false;
bool flicker1=true;
bool flicker2=true;

void correctpath3(){
    static sf::Clock clock2;
    
    if(ghostposx[2]==9 && ghostposy[2]==10){
        ghostposx[2]=14;
        ghostposy[2]=4;
        clock2.restart();
    }
    else if(ghostposy[2]!=17 && ghostposx[2]==14 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[2]){
        ghostposy[2]+=1;
        clock2.restart();
    }
    else if(ghostposy[2]==17 && ghostposx[2]!=4 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[2]){
        ghostposx[2]-=1;
        clock2.restart();
    }   
    else if(ghostposy[2]!=4 && ghostposx[2]==4 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[2]){
        ghostposy[2]-=1;
        clock2.restart();
    }  
    else if(ghostposy[2]==4 && ghostposx[2]!=14 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[2]){
        ghostposx[2]+=1;  
        clock2.restart();
    } 
}

void reversepath3(){
    static sf::Clock clock2;
    
    if(ghostposx[2]==9 && ghostposy[2]==10){
        ghostposx[2]=14;
        ghostposy[2]=4;
        clock2.restart();
    }
    else if(ghostposy[2]!=17 && ghostposx[2]==4 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[2]){
        ghostposy[2]+=1;
        clock2.restart();
    }
    else if(ghostposy[2]==4 && ghostposx[2]!=4 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[2]){
        ghostposx[2]-=1;
        clock2.restart();
    }   
    else if(ghostposy[2]!=4 && ghostposx[2]==14 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[2]){
        ghostposy[2]-=1;
        clock2.restart();
    }  
    else if(ghostposy[2]==17 && ghostposx[2]!=14 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[2]){
        ghostposx[2]+=1;  
        clock2.restart();
    } 
}

void ghost3algo(){
    if( reversed==false && ((pacmanx==ghostposx[2] && ghostposy[2]-pacmany<0 && ghostposx[2]==14) || (pacmany==ghostposy[2] && ghostposx[2]-pacmanx>0 && ghostposy[2]==17) || (pacmanx==ghostposx[2] && ghostposy[2]-pacmany>0 && ghostposx[2]==4) || (pacmany==ghostposy[2] && ghostposx[2]-pacmanx<0 && ghostposy[2]==4)) ){
        if(eaten)
            reversed=true;
    }
    else if( reversed==true && ((pacmanx==ghostposx[2] && ghostposy[2]-pacmany>0 && ghostposx[2]==14) || (pacmany==ghostposy[2] && ghostposx[2]-pacmanx<0 && ghostposy[2]==17) || (pacmanx==ghostposx[2] && ghostposy[2]-pacmany<0 && ghostposx[2]==4) || (pacmany==ghostposy[2] && ghostposx[2]-pacmanx>0 && ghostposy[2]==4) )){
        if(eaten)
            reversed=false;
    }

    if( reversed==false && ((pacmanx==ghostposx[2] && ghostposy[2]-pacmany>0 && ghostposx[2]==14) || (pacmany==ghostposy[2] && ghostposx[2]-pacmanx<0 && ghostposy[2]==17) || (pacmanx==ghostposx[2] && ghostposy[2]-pacmany<0 && ghostposx[2]==4) || (pacmany==ghostposy[2] && ghostposx[2]-pacmanx>0 && ghostposy[2]==4)) ){
        if(!eaten)
            reversed=true;
    }
    else if( reversed==true && ((pacmanx==ghostposx[2] && ghostposy[2]-pacmany<0 && ghostposx[2]==14) || (pacmany==ghostposy[2] && ghostposx[2]-pacmanx>0 && ghostposy[2]==17) || (pacmanx==ghostposx[2] && ghostposy[2]-pacmany>0 && ghostposx[2]==4) || (pacmany==ghostposy[2] && ghostposx[2]-pacmanx<0 && ghostposy[2]==4) )){
        if(!eaten)
            reversed=false;
    }

    if(reversed)
        reversepath3();
    else if (reversed==false)
        correctpath3();                 
}

void ghost4algo(){
    static sf::Clock clock2;
    
    if(ghostposx[3]==9 && ghostposy[3]==11){
        ghostposx[3]=9;
        ghostposy[3]=8;
        clock2.restart();
    }          
    else if(ghostposy[3]==8 && ghostposx[3]!=8 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[3]){
        ghostposx[3]-=1;
        clock2.restart();
    }
    else if(ghostposy[3]!=6 && ghostposx[3]==8 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[3]){
        ghostposy[3]-=1;
        clock2.restart();
    }   
    else if(ghostposy[3]==6 && ghostposx[3]!=6 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[3]){
        ghostposx[3]-=1;
        clock2.restart();
    } 
    else if(ghostposy[3]!=4 && ghostposx[3]==6 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[3]){
        ghostposy[3]-=1;
        clock2.restart();
    } 
    else if(ghostposy[3]==4 && ghostposx[3]!=5 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[3]){
        ghostposx[3]-=1;
        clock2.restart();
    }
    else if(ghostposy[3]!=1 && ghostposx[3]==5 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[3]){
        ghostposy[3]-=1;
        clock2.restart();
    }
    else if(ghostposy[3]==1 && ghostposx[3]!=1 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[3]){
        ghostposx[3]-=1;
        clock2.restart();
    } 
    else if(ghostposy[3]!=3 && ghostposx[3]==1 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[3]){
        ghostposy[3]+=1;
        clock2.restart();
    }
    else if(ghostposy[3]==3 && ghostposx[3]<4 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[3]){
        ghostposx[3]+=1;
        clock2.restart();
    }
    else if(ghostposy[3]!=1 && ghostposx[3]==4 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[3]){
        ghostposy[3]-=1;
        clock2.restart();
    } 
}

void ghost2algo(){
    static sf::Clock clock2;
    
    if(ghostposx[1]==10 && ghostposy[1]==11){
        ghostposx[1]=9;
        ghostposy[1]=8;
        clock2.restart();
    }
    else if(ghostposy[1]==8 && ghostposx[1]!=10 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[1]){
        ghostposx[1]+=1;
        clock2.restart();
    }
    else if(ghostposy[1]!=6 && ghostposx[1]==10 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[1]){
        ghostposy[1]-=1;
        clock2.restart();
    }   
    else if(ghostposy[1]==6 && ghostposx[1]!=12 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[1]){
        ghostposx[1]+=1;
        clock2.restart();
    } 
    else if(ghostposy[1]!=4 && ghostposx[1]==12 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[1]){
        ghostposy[1]-=1;
        clock2.restart();
    } 
    else if(ghostposy[1]==4 && ghostposx[1]!=13 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[1]){
        ghostposx[1]+=1;
        clock2.restart();
    }
    else if(ghostposy[1]!=1 && ghostposx[1]==13 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[1]){
        ghostposy[1]-=1;
        clock2.restart();
    }
    else if(ghostposy[1]==1 && ghostposx[1]!=17 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[1]){
        ghostposx[1]+=1;
        clock2.restart();
    } 
    else if(ghostposy[1]!=3 && ghostposx[1]==17 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[1]){
        ghostposy[1]+=1;
        clock2.restart();
    }
    else if(ghostposy[1]==3 && ghostposx[1]>14 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[1]){
        ghostposx[1]-=1;
        clock2.restart();
    }
    else if(ghostposy[1]!=1 && ghostposx[1]==14 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[1]){
        ghostposy[1]-=1;
        clock2.restart();
    } 
}

void correctpath1(){
    static sf::Clock clock2;
    if((ghostposx[0]==8 && ghostposy[0]==10) || (ghostposx[0]==8 && ghostposy[0]==19)){
        ghostposx[0]=9;
        ghostposy[0]=8;
        clock2.restart();
    }
    else if(ghostposy[0]!=13 && ghostposx[0]==12 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[0]){
        ghostposy[0]+=1;
        clock2.restart();
    }
    else if(ghostposy[0]==8 && ghostposx[0]!=12 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[0]){
        ghostposx[0]+=1;
        clock2.restart();
    }   
    else if(ghostposy[0]!=8 && ghostposx[0]==6 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[0]){
        ghostposy[0]-=1;
        clock2.restart();
    }  
    else if(ghostposy[0]==13 && ghostposx[0]!=6 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[0]){
        ghostposx[0]-=1;  
        clock2.restart();
    } 
}

void reversepath1(){
    static sf::Clock clock2;
    if((ghostposx[0]==8 && ghostposy[0]==10) || (ghostposx[0]==8 && ghostposy[0]==19)){
        ghostposx[0]=9;
        ghostposy[0]=8;
        clock2.restart();
    }
    else if(ghostposy[0]!=8 && ghostposx[0]==12 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[0]){
        ghostposy[0]-=1;
        clock2.restart();
    }
    else if(ghostposy[0]==8 && ghostposx[0]!=6 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[0]){
        ghostposx[0]-=1;
        clock2.restart();
    }   
    else if(ghostposy[0]!=13 && ghostposx[0]==6 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[0]){
        ghostposy[0]+=1;
        clock2.restart();
    }  
    else if(ghostposy[0]==13 && ghostposx[0]!=12 && clock2.getElapsedTime().asSeconds() >= GHOST_SPEED[0]){
        ghostposx[0]+=1;  
        clock2.restart();
    } 
}

void ghost1algo(){
    if( reversed2==false && ((pacmanx==ghostposx[0] && ghostposy[0]-pacmany<0 && ghostposx[0]==12) || (pacmany==ghostposy[0] && ghostposx[0]-pacmanx>0 && ghostposy[0]==13) || (pacmanx==ghostposx[0] && ghostposy[0]-pacmany>0 && ghostposx[0]==6) || (pacmany==ghostposy[0] && ghostposx[0]-pacmanx<0 && ghostposy[0]==8)) ){
        if(eaten)
            reversed2=true;
    }
    else if( reversed2==true && ((pacmanx==ghostposx[0] && ghostposy[0]-pacmany>0 && ghostposx[0]==12) || (pacmany==ghostposy[0] && ghostposx[0]-pacmanx<0 && ghostposy[0]==13) || (pacmanx==ghostposx[0] && ghostposy[0]-pacmany<0 && ghostposx[0]==6) || (pacmany==ghostposy[0] && ghostposx[0]-pacmanx>0 && ghostposy[0]==8) )){
        if(eaten)
            reversed2=false;
    }

    if( reversed2==false && ((pacmanx==ghostposx[0] && ghostposy[0]-pacmany>0 && ghostposx[0]==12) || (pacmany==ghostposy[0] && ghostposx[0]-pacmanx<0 && ghostposy[0]==13) || (pacmanx==ghostposx[0] && ghostposy[0]-pacmany<0 && ghostposx[0]==6) || (pacmany==ghostposy[0] && ghostposx[0]-pacmanx>0 && ghostposy[0]==8)) ){
        if(!eaten)
            reversed2=true;
    }
    else if( reversed2==true && ((pacmanx==ghostposx[0] && ghostposy[0]-pacmany<0 && ghostposx[0]==12) || (pacmany==ghostposy[0] && ghostposx[0]-pacmanx>0 && ghostposy[0]==13) || (pacmanx==ghostposx[0] && ghostposy[0]-pacmany>0 && ghostposx[0]==6) || (pacmany==ghostposy[0] && ghostposx[0]-pacmanx<0 && ghostposy[0]==8) )){
        if(!eaten)
            reversed2=false;
    }

    if(reversed2)
        reversepath1();
    else if (reversed2==false){
        correctpath1(); 
    }
}

