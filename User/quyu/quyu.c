#include "quyu.h"
#include "math.h"
static int NeighborDirection[8][2]={{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};
typedef struct QNode //定义队列
{
 uint16_t data;
 struct QNode *next;
}QNode;

typedef struct Queue  //采用链式队列结构
{
 struct QNode* first;
 struct QNode* last;
}Queue;















