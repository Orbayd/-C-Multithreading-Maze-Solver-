#ifndef MT_Maze_Student_Solver_H
#define MT_Maze_Student_Solver_H

#include <assert.h>
#include <list>
//My Additions 
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include "safeQueue.h"
#include "ThreadHelper.h"
#include "joinThreads.h"

#include "Maze.h"
#include "MazeSolver.h"
#include "SkippingMazeSolver.h"
#include "Direction.h"
struct MyDir
{
	Direction dir;
	Position at;
	Choice  *parent;
};
// Feel free to change your class any way you see fit
// It needs to inherit at some point from the MazeSolver
class MTMazeStudentSolver : public SkippingMazeSolver 
{
public: 
	//Ctor
	MTMazeStudentSolver(Maze *maze) : SkippingMazeSolver( maze ),joiner(threads)
	{
		assert(pMaze);
		done = false;
		pSolve = new std::list<Direction>();
	    unsigned const thread_count=std::thread::hardware_concurrency()-4;
	
        try
        {
            for(unsigned i=0;i<thread_count;++i)
            {
			
				
				threadsafe_queue<MyDir> *que = new threadsafe_queue<MyDir>();
				
				threadsQueues.push_back(que);
                
				threads.push_back(
					std::thread(&MTMazeStudentSolver::worker_thread,this,std::ref(que)));
				
				
            }
			threads.push_back(
					std::thread(&MTMazeStudentSolver::controler_thread,this));
        }
        catch(...)
        {
            done=true;
            throw;
        }


	}
	//dtor
	~MTMazeStudentSolver()
    {
        done=true;
	
    }

	std::list<Direction> *Solve() override
	{
		// Do your magic here
		
		submit(firstChoice(pMaze->getStart()));
		  std::unique_lock<std::mutex> lk(m);
		  cv.wait(lk);
		  
		  return pSolve;
	}
	void worker_thread(threadsafe_queue<MyDir> *work_queue);
	void controler_thread();
	void submit(Choice *pChoice);
	Choice * expand(Direction dir,Position at,Choice * parent);
	Choice * follow(Direction dir,Position at);
	Choice * firstChoice(Position pos);
	void backTrack(Choice*parent,Direction _from);
private:
	std::atomic_bool done;
	threadsafe_queue<MyDir> cont_queue;
	std::vector<std::thread> threads;
	std::vector<threadsafe_queue<MyDir>*> threadsQueues;
	join_threads joiner;
	std::list<Direction> *pSolve;

	std::mutex m;
    std::condition_variable cv;
};


#endif