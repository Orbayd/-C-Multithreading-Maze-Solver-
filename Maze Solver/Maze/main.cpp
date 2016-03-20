#include <stdio.h>
#include <assert.h>

#include "Timer.h"
#include "STMazeSolverDFS.h"
#include "STMazeSolverBFS.h"
#include "MTMazeStudentSolver.h"
#include "MTMazeStudentSolver_2.h"
#define INPUT_NAME_SIZE 64

// change to "1" for the final submission
#define FINAL_SUBMIT 1


int main(int argc, char *argv[]) 
{

#if !FINAL_SUBMIT
	char inFileName[INPUT_NAME_SIZE] = "Maze10Kx10K_D.data";
#else
	char inFileName[INPUT_NAME_SIZE];
	memset(inFileName,0x0,INPUT_NAME_SIZE);
	strcpy_s( inFileName, INPUT_NAME_SIZE, argv[1]);
#endif

	printf("Maze: start(%s) ------------\n",inFileName);

	// Initialize Timer
	timer::initTimer();

	// Create timers 
	timer ST_DFS_Timer;
	timer ST_BFS_Timer;
	timer MT_Student_Timer;
	timer MT_Student_Timer_2;
	 
	// Create a maze
	Maze *pMaze = new Maze();

	// Load it
	pMaze->Load(inFileName);

	// -- STMazeSolverBFS -----------------------------------------------------------
	printf("\n Maze: STMazeSolverBFS\n");

		// Solve it
		ST_BFS_Timer.tic(); 

			STMazeSolverBFS stSolverBFS(pMaze);
			std::list<Direction> *pSolutionBFS = stSolverBFS.Solve();
		
		ST_BFS_Timer.toc();

		// Verify solution - do not delete, it is not timed
		pMaze->checkSolution( pSolutionBFS );
		delete pSolutionBFS;

	// -- STMazeSolverDFS -----------------------------------------------------------
	printf("\n Maze: STMazeSolverDFS\n");

		// Solve it
		ST_DFS_Timer.tic(); 

			STMazeSolverDFS stSolverDFS(pMaze);
			std::list<Direction> *pSolutionDFS = stSolverDFS.Solve();
		
		ST_DFS_Timer.toc();

		// Verify solution - do not delete, it is not timed
		pMaze->checkSolution( pSolutionDFS );
		delete pSolutionDFS;

	// -- Multi-Threaded Student Solver ----------------------------------------------
	//printf("\n Maze: MTStudentSolver\n");

		// Solve it
		//MT_Student_Timer.tic(); 

		//	MTMazeStudentSolver mtStudentSolver(pMaze);
		//	std::list<Direction> *pSolutionStudent = mtStudentSolver.Solve();
		//
		//MT_Student_Timer.toc();

		//// Verify solution - do not delete, it is not timed
		//pMaze->checkSolution( pSolutionStudent );
		//delete pSolutionStudent;



// -- Multi-Threaded Student Solver ----------------------------------------------
	printf("\n Maze: MTStudentSolver\n");

		// Solve it
		MT_Student_Timer_2.tic(); 

			MTMazeStudentSolver_2 mtStudentSolver_2(pMaze);
			std::list<Direction> *pSolutionStudent_2 = mtStudentSolver_2.Solve();
		
		MT_Student_Timer_2.toc();

		// Verify solution - do not delete, it is not timed
		pMaze->checkSolution( pSolutionStudent_2 );
		delete pSolutionStudent_2;		 
	// Stats --------------------------------------------------------------------------

		float ST_DFSTime = ST_DFS_Timer.timeInSeconds();
		float ST_BFSTime = ST_BFS_Timer.timeInSeconds();
		float MT_StudentTime = MT_Student_Timer_2.timeInSeconds();

		printf("\n");
		printf("Results (%s):\n\n",inFileName);
		printf("   BFS      : %f s\n",ST_BFSTime);
		printf("   DFS      : %f s\n",ST_DFSTime);
		printf("   Student  : %f s\n",MT_StudentTime);

	printf("\nMaze: end() --------------\n\n"); 

#if !FINAL_SUBMIT
	system("PAUSE");
#endif

}

