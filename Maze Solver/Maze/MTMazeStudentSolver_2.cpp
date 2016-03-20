#include "MTMazeStudentSolver_2.h"
#include "ThreadHelper.h"
int i=0;
void MTMazeStudentSolver_2::worker_thread_forward(std::list<Choice> *pChoiceStack)
{
	
	Choice ch;
	int count = 0;
	pChoiceStack->push_front( firstChoice( pMaze->getStart() ) ) ;
	while (!done)
	{
      
		
		  if(!(pChoiceStack->size() == 0) )
			{
				count++;
				
				ch = pChoiceStack->front();
				if(ch.isDeadend()) 
				{
					// backtrack.

					pChoiceStack->pop_front();

					if(!(pChoiceStack->size() == 0) )
					{
						pChoiceStack->front().pChoices.pop_front();
					}

					continue;
				}
				
				pChoiceStack->push_front( follow_front( ch.at, ch.pChoices.front() ) );
			}
	  }
	  
		 //  printf("<Forward> find Solution \n");
		  // cv.notify_one();
			
	

	
	printf("(%llu)I Finished <Front>\n",GetCurrentID());		
	finished_thread_count++;
}

void MTMazeStudentSolver_2::worker_thread_backward(std::list<Choice> *pChoiceStack)
{

	Choice ch;
	int count = 0;
	pChoiceStack->push_front( firstChoice( pMaze->getEnd()) ) ;

	while (!done)
	{
		
			if(!(pChoiceStack->size() == 0) )
				{
					count++;

					ch = pChoiceStack->front();
				
					if(ch.isDeadend()) 
					{
					// backtrack.
						pChoiceStack->pop_front();

						if(!(pChoiceStack->size() == 0) )
						{
							pChoiceStack->front().pChoices.pop_backward();
						}

					continue;
				}
				
					pChoiceStack->push_front( follow_back( ch.at, ch.pChoices.backWard() ) );
		
	
	         }
		
		  
			// printf("<Backward> find Solution \n");
		   //  cv.notify_one();
			
		
  }
	printf("(%llu)I Finished <BackWard>\n",GetCurrentID());
		
	
	finished_thread_count++;


}

Choice MTMazeStudentSolver_2::follow_front( Position at, Direction dir )  //throws SolutionFoundSkip() 
	{
		ListDirection Choices;
		Direction go_to = dir;
		Direction came_from = reverseDir(dir);
	
		//pMaze->setMazeCellInfo(at);
		Position Parentpos = at;
		at = at.move(go_to);
		pMaze->setParentInfo(at,Parentpos,go_to);
		do 
		{
			Parentpos = at;
			if( at == (pMaze->getEnd()) )
			{
				SolutionFound(at,go_to);
				
			}
		
			
			
			Choices = pMaze->getMoves(at);
			Choices.remove(came_from);
	
			if(Choices.size() == 1) 
			{
				go_to = Choices.begin();
				at = at.move(go_to);
				came_from = reverseDir(go_to);
				pMaze->setParentInfo(at,Parentpos,go_to);
			}

		} while( Choices.size() == 1 );
		
	
		CellCheck(at,go_to,MazeThread::FOWWARD);
		Choice pRet( at, came_from, Choices ) ;

		return pRet;
	}

Choice MTMazeStudentSolver_2::follow_back( Position at, Direction dir )  //throws SolutionFoundSkip() 
	{
		ListDirection Choices;
		Direction go_to = dir;
	
		Choices = pMaze->getMoves(at);
		Direction came_from = reverseDir(dir);
		Direction go = came_from;
		//pMaze->setMazeCellInfo(at);
		Position  Childpos = at;
		at = at.move(go_to);
		pMaze->setParentInfo(Childpos,at,came_from);
		do 
		{
			Childpos = at;
			if( at == (pMaze->getStart()) )
			{
			   SolutionFound(at,go_to);
				
			}
		
			Choices = pMaze->getMoves(at);
			Choices.remove(came_from);
	
			if(Choices.size() == 1) 
			{
				go_to = Choices.begin();
				at = at.move(go_to);
				came_from = reverseDir(go_to);
				pMaze->setParentInfo(Childpos,at,came_from);
			}

		} while( Choices.size() == 1 );


		CellCheck(at,go_to,MazeThread::BACKWARD);
		Choice pRet( at,go, Choices ) ;
		//printf("%d,%d\n" ,at.col,at.row);
		return pRet;
	}

Choice MTMazeStudentSolver_2::firstChoice( Position pos ,Direction from ) //throw SolutionFound 
	{
		ListDirection Moves = pMaze->getMoves(pos);

   if(Moves.size() == 1)
		{
			if(pos ==pMaze->getStart())
			{
				Direction tmp = Moves.begin();
				return follow_front(pos, tmp);
			}
			else if(pos== pMaze->getEnd())
			{
				Direction tmp = Moves.backWard();
				return follow_back(pos, tmp);
			}
	    }
		else
		{
			Choice p(pos, from, Moves);
			return p;		
		}
	}

void MTMazeStudentSolver_2::backtrack()
{
	Position pos= pMaze->getEnd();
	Position start = pMaze->getStart();
	Parent parentInfo;
	while (!(pos==start))
	{
	    parentInfo = pMaze->getParentInfo(pos);
	//	printf(" %s\n",DirectionHelper::getString(parentInfo.path));
		pSolutionPath->push_front(parentInfo.path);
		pos= parentInfo.parentPos;
	}

}
void MTMazeStudentSolver_2::CellCheck(Position at,Direction go_to,MazeThread info)
{
	   //throw_soln_mtx.lock();
	    MazeInfo mInfo = pMaze->getMazeCellInfo(at) ;
	   	
		if(mInfo.isPassed==true)
			{
				if(mInfo.passedThread!=info)
				    SolutionFound(at,go_to);
			}
	
         pMaze->setMazeCellInfo(at,info);
		 //throw_soln_mtx.unlock();
}
void MTMazeStudentSolver_2::SolutionFound(Position at ,Direction go_to)
{
				ListDirection Choices;
				Choices = pMaze->getMoves(at);
			  // throw_soln_mtx.lock();
		       if(done!= true)
				{
					done=true;
					printf("I hit a (%d %d)\n",at.row,at.col);
					cv.notify_one();
					
				}
			 //  throw_soln_mtx.unlock();


}