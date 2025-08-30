#ifndef FLOOD_H
#define FLOOD_H

#include "myqueue.h"
#include "config.h"

// const int ROW = 24;
// const int COL = 24;   // maze size
// const int CELL = 30;  // pixel size of each cell
// const int endr = 16;
// const int endc = 16;
enum Direction {
	North, East, South, West
};

struct Cell {
	int row, col;
	bool checked, wallN, wallE, wallS, wallW, visitedCell;
	int value;
  //Direction cellDir;
};


void setWall(Cell **maze, int r, int c, Direction dir) {
	switch (dir) {
		case North:
			maze[r][c].wallN = true;
			if (r < ROW-1) maze[r+1][c].wallS = true;
			break;
		case East:
			maze[r][c].wallE = true;
			if (c < COL-1) maze[r][c+1].wallW = true;
			break;
		case South:
			maze[r][c].wallS = true;
			if (r > 0) maze[r-1][c].wallN = true;
			break;
		case West:
			maze[r][c].wallW = true;
			if (c > 0) maze[r][c-1].wallE = true;
			break;
	}
}

void floodFill(MyQueue<Cell*> &queue, Cell **maze){ 
	//queue.enqueue(&maze[r][c]); 
	while(!queue.isEmpty()){ 
		Cell* info = queue.dequeue(); 
		int cost = info->value; 
		int r = info->row; 
		int c = info->col; 
		//North-neighbour 
		if(r<ROW-1 && !info->wallN){ 
			if(!maze[r+1][c].checked){ 
				maze[r+1][c].value = cost+1; 
				maze[r+1][c].checked = true; 
				queue.enqueue(&maze[r+1][c]); 
			} 
		} 
		//East-neighbour 
		if(c<COL-1 && !info->wallE){ 
			if(!maze[r][c+1].checked){ 
				maze[r][c+1].value = cost+1; 
				maze[r][c+1].checked = true; 
				queue.enqueue(&maze[r][c+1]); 
			} 
		} 
		//South-neighbour 
		if(r>0 && !info->wallS){ 
			if(!maze[r-1][c].checked){ 
				maze[r-1][c].value = cost+1; 
				maze[r-1][c].checked = true; 
				queue.enqueue(&maze[r+1][c]); 
			}
		} 
		//West-neighbour 
		if(c>0 && !info->wallW){ 
			if(!maze[r][c-1].checked){ 
				maze[r][c-1].value = cost+1; 
				maze[r][c-1].checked = true; 
				queue.enqueue(&maze[r][c-1]); 
			} 
		} 
	} 
}


void updateFlood(Cell **maze, int startRow, int startCol) {
  for(int r=0;r<ROW;r++)
    for(int c=0;c<COL;c++){
        maze[r][c].checked = false;
        maze[r][c].value = -1;
    }

  MyQueue<Cell*> queue;
  maze[startRow][startCol].value = 0;
  maze[startRow][startCol].checked = true;
  queue.enqueue(&maze[startRow][startCol]);

  floodFill(queue, maze);
}

Direction getLowestNeighbor(Cell **maze, int r, int c) {
    int minValue = 999;
    Direction bestDir = North;  // default

    // North (row + 1)
    if (r < ROW-1 && !maze[r][c].wallN) {
        if (maze[r+1][c].value >= 0 && maze[r+1][c].value < minValue) {
            minValue = maze[r+1][c].value;
            bestDir = North;
        }
    }

    // East (col + 1)
    if (c < COL-1 && !maze[r][c].wallE) {
        if (maze[r][c+1].value >= 0 && maze[r][c+1].value < minValue) {
            minValue = maze[r][c+1].value;
            bestDir = East;
        }
    }

    // South (row - 1)
    if (r > 0 && !maze[r][c].wallS) {
        if (maze[r-1][c].value >= 0 && maze[r-1][c].value < minValue) {
            minValue = maze[r-1][c].value;
            bestDir = South;
        }
    }

    // West (col - 1)
    if (c > 0 && !maze[r][c].wallW) {
        if (maze[r][c-1].value >= 0 && maze[r][c-1].value < minValue) {
            minValue = maze[r][c-1].value;
            bestDir = West;
        }
    }

    return bestDir;
}

// Update walls of current cell based on robot's facing direction and sensor readings
// wallFront, wallRight, wallLeft are booleans from the robot's sensors
void setRelativeWall(Cell **maze, int r, int c, Direction currDir,
                     bool wallFront, bool wallRight, bool wallLeft) {
    // Front wall
    if (wallFront) {
        setWall(maze, r, c, currDir);
    }

    // Right wall
    if (wallRight) {
        Direction rightDir = (Direction)((currDir + 1) % 4);
        setWall(maze, r, c, rightDir);
    }

    // Left wall
    if (wallLeft) {
        Direction leftDir = (Direction)((currDir + 3) % 4);  // +3 is same as -1 mod 4
        setWall(maze, r, c, leftDir);
    }
}

#endif