//
//  main.cpp

 /*-------------------------------------------------------------------------+
 |	No idea what this is doing.  Ask Chris.									|
 +-------------------------------------------------------------------------*/

#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <thread>
//
#include "gl_frontEnd.h"

//==================================================================================
//	Custom data types
//==================================================================================


//==================================================================================
//	Function prototypes
//==================================================================================
void displayGridPane(void);
void displayStatePane(void);
void initializeApplication(void);
void* pacmanLoop(void* v);
void* ghostLoop(void* v);
void pacmanThreadFunc(PacMan* pacman, GameWorld& gameWorld);
void ghostThreadFunc(Ghost* ghost, GameWorld& gameWorld);


//==================================================================================
//	Application-level global variables
//==================================================================================

//	Don't touch
extern int	GRID_PANE, STATE_PANE;
extern int	gMainWindow, gSubwindow[2];

//	The state grid and its dimensions
GameWorld gameWorld;

//	pacman game stats
int numLiveThreads = 0;
int score = 0;
int dots_collected = 0;
int pacman_lives = 3;

//==================================================================================
//	These are the functions that tie the simulation with the rendering.
//	Some parts are "don't touch."  Other parts need your intervention
//	to make sure that access to critical section is properly synchronized
//==================================================================================


void displayGridPane(void)
{
	//	This is OpenGL/glut magic.  Don't touch
	glutSetWindow(gSubwindow[GRID_PANE]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//---------------------------------------------------------
	//	This is the call that makes OpenGL render the grid.
	//
	//	You *must* synchronize this call.
	//
	//---------------------------------------------------------
    drawGameWorld(&gameWorld);
	
	//	This is OpenGL/glut magic.  Don't touch
	glutSwapBuffers();
	
	glutSetWindow(gMainWindow);
}

void displayStatePane(void)
{
	//	This is OpenGL/glut magic.  Don't touch
	glutSetWindow(gSubwindow[STATE_PANE]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//---------------------------------------------------------
	//	This is the call that makes OpenGL render information
	//	about the state of the simulation.
	//
	//	You *must* synchronize this call.
	//
	//---------------------------------------------------------
    drawState(numLiveThreads, score, dots_collected, pacman_lives);
	
	
	//	This is OpenGL/glut magic.  Don't touch
	glutSwapBuffers();
	
	glutSetWindow(gMainWindow);
}

//------------------------------------------------------------------------
//	You shouldn't have to change anything in the main function
//------------------------------------------------------------------------
int main(int argc, char** argv)
{
    srand(static_cast<unsigned int>(time(NULL)));

	initializeFrontEnd(argc, argv, displayGridPane, displayStatePane);
	
	//	Now we can do application-level
	initializeApplication();

	//	Now we enter the main loop of the program and to a large extend
	//	"lose control" over its execution.  The callback functions that 
	//	we set up earlier will be called when the corresponding event
	//	occurs
	glutMainLoop();
		
	//	This will never be executed (the exit point will be in one of the
	//	call back functions).
	return 0;
}


//==================================================================================
//
//	This is a part that you have to edit and add to.
//
//==================================================================================


void initializeApplication(void)
{
    //Allocate the grid and all objects
    initializeGameWorldFile(gameWorld, "Levels/level_01.txt");

	//	Probably here would be a good place to create the threads
	std::thread *pacmanThread = new std::thread(pacmanThreadFunc, gameWorld.pacman, gameWorld);
	std::thread *ghostThreads[gameWorld.ghost_count];
	for (int i = 0; i < gameWorld.ghost_count; i++) {
		ghostThreads[i] = new std::thread(ghostThreadFunc, gameWorld.ghost_array[i], gameWorld);
	}

 }

//-------------------------------------------------------------------
//	This is the main work that needs to be done by multiple threads:
//		One thread for Pacman, and one for each ghost
//-------------------------------------------------------------------
void updatePacAndGhosts(void)
{
	//update pacman
	if (gameWorld.pacman != nullptr) 
	{
		gameWorld.pacman->update(gameWorld);
	}
		
	// update ghosts
	for(int k=0; k<gameWorld.ghost_count; k++)
	{
		gameWorld.ghost_array[k].ghostAI(gameWorld);
	}
}

void pacmanThreadFunc(PacMan* pacman, GameWorld& gameWorld) {
	while (true) {
		pacman->update(gameWorld);
		usleep(10000);
	}
}

void ghostThreadFunc(Ghost* ghost, GameWorld& gameWorld) {
	while (true) {
		ghost->ghostAI(gameWorld);
		usleep(10000);
	}
}

