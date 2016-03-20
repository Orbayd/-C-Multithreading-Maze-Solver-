#ifndef MAZE_H
#define MAZE_H

#include <list>
#include <memory>

#include "Position.h"
#include "Direction.h"
#include "Choice.h"

#define DEBUG_PRINT 0

// Maze cells are (row,col) starting at (0,0)
// Linear storage of integers 0 to (width*height - 1)
// Upper Upper corner (0,0) or index 0
// Lower Left corner (width-1, height-1) or (width*height - 1)
struct Parent
{
	Position parentPos;
	Direction path;
	Parent():parentPos(-1,-1),path(Direction::Uninitialized){}
};
enum class MazeThread
{
	FOWWARD,
	BACKWARD,
	UNINITILAZED
};
struct MazeInfo
{
	bool isPassed;
	MazeThread passedThread;
	MazeInfo()
	{
		isPassed= false;
		passedThread =MazeThread:: UNINITILAZED;

	}
};
class Maze
{
public:

	Maze();
	Maze( int _width, int _height);

	void Load( const char * const fileName );
	void Solve();

	ListDirection getMoves(Position pos);
	bool Maze::canMove( Position pos, Direction dir );
	bool checkSolution( std::list<Direction> *soln );

	Position getStart();
	Position getEnd() ;;

	void setEast(Position pos);
	void setSouth(Position pos);
	int getCell(Position pos);
	void setCell(Position pos, int val);
	
	MazeInfo getMazeCellInfo(Position p);
	void setMazeCellInfo(Position pos,MazeThread thread);

	Parent getParentInfo(Position pos);
	void   setParentInfo(Position pos,Position parent,Direction dir);
// data:
	int *pMazeData;
	int height;
	int width;
	int solvable;
	MazeInfo *pMazeCellInfo;
	Parent *pMazeParentData;
};


#endif