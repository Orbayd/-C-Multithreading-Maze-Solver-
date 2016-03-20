#include "MTMazeStudentSolver.h"
#include <assert.h>
void MTMazeStudentSolver::worker_thread(threadsafe_queue<MyDir> *work_queue)
{
	 while(!done)
        {
		
			MyDir task;
			Choice *cho;
	
            if(work_queue->try_pop(task))
            {
				while(1)
				{
					cho = expand(task.dir,task.at,task.parent);
				
					if(cho!=0)
					{
						cho->parent = task.parent;
						if(!cho->isDeadend())
						{
							task.at =cho->at;
							task.parent =cho;
							task.dir = cho->pChoices.pop_front();
							submit(cho);
						}
						else{
							break;
							delete cho;
						}
				
					}
					else
						break;
				
			   }
			
			}
            else
            {
                std::this_thread::yield();
            }
        }
		printf("I finished\n (%llu)\n",GetCurrentID());



}
void MTMazeStudentSolver::controler_thread()
{
	size_t i =0;
	while(!done){
		if(!cont_queue.empty()){
			MyDir task;
			if(cont_queue.try_pop(task))
			{
				threadsQueues[i]->push(task);
				i++;
				if(i=threadsQueues.size())
				i=0;
			}
		}
	}
}
void MTMazeStudentSolver::submit(Choice *pChoice)
{
	ListDirection ds = pChoice->pChoices;
	for( Direction dir = ds.begin(); dir != Direction::Uninitialized; dir = ds.getNext(dir) )
		{
		
			MyDir  mydir;
			mydir.dir = dir;
			mydir.at =  pChoice->at;
			mydir.parent = pChoice;
			cont_queue.push(mydir);
		}

}
Choice* MTMazeStudentSolver::firstChoice(Position pos)
{

	    ListDirection Moves = pMaze->getMoves(pos);

		if(Moves.size() == 1)
		{
			// delete pMoves;  need to verify
				Direction tmp = Moves.begin();
			   return follow( tmp, pos);
		}
		else
		{
			Choice *p = new Choice(pos, Direction::Uninitialized, Moves);
			return p;		
		}


}
Choice * MTMazeStudentSolver::expand(Direction dir,Position at,Choice* parent)
{
	Choice * c ;
	try
		{
			c =  follow( dir,at);
			return c;
		}
	catch(SolutionFoundSkip e)
	{
		backTrack(parent,e.from);
		//pMaze->checkSolution( pSolve );
	    cv.notify_one();
			
		return 0;
	}

}

Choice * MTMazeStudentSolver::follow(Direction dir,Position at)
{
		Position parent = at;
	    ListDirection Choices;
		Direction go_to = dir;
		Direction came_from = reverseDir(dir);
		at = at.move(go_to);

		do 
		{
			if( at == (pMaze->getEnd()) )
			{
				throw SolutionFoundSkip( at, reverseDir(go_to) );
				
			}
		/*	else if( at == (pMaze->getStart()) )
			{
				throw SolutionFoundSkip( at, reverseDir(go_to) );
			}*/
			Choices = pMaze->getMoves(at);
			Choices.remove(came_from);
	
			if(Choices.size() == 1) 
			{
				go_to = Choices.begin();
				at = at.move(go_to);
				came_from = reverseDir(go_to);
			}

		} while( Choices.size() == 1 );
		
		//Choice pRet( at, came_from, Choices, parent ) ;
		Choice *pRet  =  new Choice( at, came_from, Choices ) ;
		return pRet;

}

void MTMazeStudentSolver::backTrack(Choice*parent, Direction _from)
{
		ListDirection pChoices;
		Choice *pa = parent;
		
			
			Direction came_from = reverseDir(_from);
			pSolve->push_front(came_from);
			Direction go_to;
			//at = at.move(go_to);
			Position at= pMaze->getEnd();
			at =at.move(_from);
			while(1 )
			{
				if( at == pMaze->getStart() )
			    {
					break;
			    }
				pChoices = pMaze->getMoves(at);
				pChoices.remove(came_from);
				if(pChoices.size() == 1) 
				{
					
					go_to = pChoices.begin();
					at = at.move(go_to);
					came_from = reverseDir(go_to); 
					pSolve->push_front(came_from);
				}
				else if(pChoices.size()>1)
				{
					pSolve->push_front(reverseDir(pa->from));
					at = at.move(pa->from);
					came_from = reverseDir(pa->from);
					pa=pa->parent;
				
				}
		    }



}