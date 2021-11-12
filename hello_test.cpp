#include <gtest/gtest.h>
#include <vector>

using namespace std;

class Cell 
{
public:
  virtual ~Cell(){}
  virtual bool isAlive() = 0;
  virtual void print() = 0;
private:
};

class DeadCell : public Cell
{
  public:
    DeadCell()    {    }

    bool isAlive() override
    {
      return false;
    }
    void print() override
    {
      std::cout<<" X";
    }
};

class AliveCell : public Cell
{
  public:
    AliveCell(){    }
    bool isAlive() override
    {
      return true;
    }
    void print() override
    {
      std::cout<<" O";
    }
};

void addAliveCounter(Cell * c, int & aliveNeighbours)
{
    if (c->isAlive())
    {
      aliveNeighbours++;
    }
}

void checkUp(vector<vector<Cell *>> colony, int x, int y, int &aliveNeighbours)
{
  if (x > 0)
  {
    addAliveCounter(colony[x-1][y], aliveNeighbours);
  }
}

void checkDown(vector<vector<Cell *>> colony, int x, int y, int XMax, int & aliveNeighbours)
{
    if (x < XMax)//check down
    {            
        addAliveCounter(colony[x+1][y], aliveNeighbours);
    }
}

void checkLeft(vector<vector<Cell *>> colony, int x, int y, int & aliveNeighbours)
{
    if (y > 0)//check left
    {            
        addAliveCounter(colony[x][y-1], aliveNeighbours);
    }
}

void checkRight(vector<vector<Cell *>> colony, int x, int y, int YMax, int & aliveNeighbours)
{
    if (y < YMax) //check right
    {            
        addAliveCounter(colony[x][y+1], aliveNeighbours);
    }
}

void checkVertical(vector<vector<Cell *>> colony, int x, int y, int XMax, int & aliveNeighbours)
{
    checkUp(colony, x, y, aliveNeighbours);
    checkDown(colony, x, y, XMax, aliveNeighbours);
}
void checkHorizontal(vector<vector<Cell *>> colony, int x, int y, int YMax, int & aliveNeighbours)
{
    checkLeft(colony, x, y, aliveNeighbours);
    checkRight(colony, x, y, YMax, aliveNeighbours);
}

int aliveNeighbourCounter(vector<vector<Cell *>> colony, int x, int y, int XMax, int YMax)
{
    int aliveNeighbours = 0;
    checkVertical(colony, x, y, XMax, aliveNeighbours);
    checkHorizontal(colony, x, y, YMax, aliveNeighbours);
    return aliveNeighbours;
}

void printSingleCell(Cell* cell)
{
  if (cell)
  {
    cell->print();
  }
  else
  {
      std::cout<<endl<<"ERRORORRORORORRO: printColony"<<endl;
  }
}

void printSingleRow(vector<Cell *> line)
{
      for (Cell* cell : line)
      {
          printSingleCell(cell);
      }
}

void printColony (vector<vector<Cell *>> colony) 
{
  for(vector<Cell *> line : colony)
  {
    printSingleRow(line);
    std::cout<<endl;
  }
}

void ruleFewerThan2Dies(vector<vector<Cell *>> &colony, int x, int y)
{
    if (colony[x][y]->isAlive() && aliveNeighbourCounter(colony, x, y, colony.size() - 1, colony[0].size() - 1) < 2)
    {
      delete colony[x][y];
      colony[x][y] = new DeadCell();
    }
}

void gameOfLife(vector<vector<Cell *>> &colony) 
{
  for(int x = 0; x < colony.size(); x++)
  {
    for (int y = 0; y < colony[x].size(); y++)
    {
      //apply rules 
      //Any live cell with fewer than two live neighbours dies, as if by underpopulation.
      ruleFewerThan2Dies(colony, x, y);
    }
  }
}
// Any live cell with fewer than two live neighbours dies, as if by underpopulation.
TEST(HelloTest, FewerThan2Neighbours_Die) 
{

  vector<vector<Cell *>>
      Colony1={{new DeadCell(), new AliveCell(), new DeadCell(), new AliveCell(), new AliveCell()}, 
              {new AliveCell(), new AliveCell(), new AliveCell(), new AliveCell(), new AliveCell()},
              {new DeadCell(), new DeadCell(), new DeadCell(), new DeadCell(), new DeadCell()},
              {new AliveCell(), new DeadCell(), new DeadCell(), new DeadCell(), new DeadCell()},
              {new DeadCell(), new AliveCell(), new AliveCell(), new AliveCell(), new AliveCell()}};
//Check
//Run cycle
//Expect cell to die or not.
std::cout<<endl<<endl<<"Colony before:"<<endl;
printColony(Colony1);

gameOfLife(Colony1);

std::cout<<endl<<endl<<"Colony after:"<<endl;
printColony(Colony1);

  vector<vector<Cell *>>
      Colony2={{new DeadCell(), new DeadCell(), new DeadCell(), new AliveCell(), new AliveCell()}, 
              {new DeadCell(), new DeadCell(), new DeadCell(), new AliveCell(), new AliveCell()},
              {new DeadCell(), new DeadCell(), new DeadCell(), new DeadCell(), new DeadCell()},
              {new DeadCell(), new DeadCell(), new DeadCell(), new DeadCell(), new DeadCell()},
              {new DeadCell(), new DeadCell(), new DeadCell(), new DeadCell(), new DeadCell()}};

  // Expect equality.

  for(int x = 0; x < Colony1.size(); x++)
  {
    for (int y = 0; y < Colony1[x].size(); y++)
    {
      EXPECT_EQ(Colony1[x][y]->isAlive(), Colony2[x][y]->isAlive());
    }
  }

  //TODO compare colonies
}
/*
Any live cell with fewer than two live neighbours dies, as if by underpopulation.
Any live cell with two or three live neighbours lives on to the next generation.
Any live cell with more than three live neighbours dies, as if by overpopulation.
Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
These rules, which compare the behavior of the automaton to real life, can be condensed into the following:

Any live cell with two or three live neighbours survives.
Any dead cell with three live neighbours becomes a live cell.
All other live cells die in the next generation. Similarly, all other dead cells stay dead.
*/