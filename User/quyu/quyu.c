#include "quyu.h"
#include "math.h"
static int NeighborDirection[8][2]={{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};
typedef struct QNode //�������
{
 uint16_t data;
 struct QNode *next;
}QNode;

typedef struct Queue  //������ʽ���нṹ
{
 struct QNode* first;
 struct QNode* last;
}Queue;















