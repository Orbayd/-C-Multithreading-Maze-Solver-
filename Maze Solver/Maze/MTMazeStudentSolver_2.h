#ifndef MT_Maze_Student_Solver_2_H
#define MT_Maze_Student_Solver_2_H
#include <assert.h>
#include <list>
//My Additions 
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "joinThreads.h"
#include "Maze.h"
#include "MazeSolver.h"
#include "SkippingMazeSolver.h"
#include "Direction.h"

class MTMazeStudentSolver_2 : public SkippingMazeSolver 
{
public: 
	//Ctor
	MTMazeStudentSolver_2(Maze *maze) : SkippingMazeSolver( maze ),joiner(threads),finished_thread_count(0)
	{
		assert(pMaze);
		done = false;
		pSolutionPath = new std::list<Direction>();
		
	   /* unsigned const thread_count=std::thread::hardware_concurrency()-4;*/
		
        try
        {
            /*for(unsigned i=0;i<thread_count;++i)*/
            {
			
				
			
				
				pChoiceStack_forward = new std::list<Choice>();
                threads.push_back(
					std::thread(&MTMazeStudentSolver_2::worker_thread_forward,this,std::ref(pChoiceStack_forward)));
				
				pChoiceStack_bacward = new std::list<Choice>();
                threads.push_back(
					std::thread(&MTMazeStudentSolver_2::worker_thread_backward,this,std::ref(pChoiceStack_bacward)));
				
            }
			
        }
        catch(...)
        {
            done=true;
            throw;
        }


	}
	//dtor
	~MTMazeStudentSolver_2()
    {
        //done=true;
		while(1)
		{
			if(finished_thread_count == threads.size())
				break;
		}
		joiner.terminateThreads();
    }

	std::list<Direction> *Solve() override
	{
		// Do your magic here
		  std::unique_lock<std::mutex> lk(wait_soln_mtx);
		  cv.wait(lk);
		  backtrack();
		  return pSolutionPath;
	}
private:
	void worker_thread_forward(std::list<Choice> *pChoiceStack);
	void worker_thread_backward(std::list<Choice> *pChoiceStack);
	Choice follow_front( Position at, Direction dir );
	Choice follow_back( Position at, Direction dir );
	Choice firstChoice( Position pos,Direction from =Direction::Uninitialized );
	void  backtrack();
	void SolutionFound(Position at ,Direction to_go);
	void CellCheck(Position at,Direction go_to,MazeThread info);
	std::atomic<unsigned int > finished_thread_count;	
private:
	join_threads joiner;
	std::atomic_bool done;
	std::vector<std::thread> threads;
	std::list<Direction> *pSolutionPath;
	std::list<Choice> *pChoiceStack_forward ;
	std::list<Choice> *pChoiceStack_bacward ;
	std::mutex wait_soln_mtx;
    std::condition_variable cv;
    std::mutex throw_soln_mtx;
};

#endif