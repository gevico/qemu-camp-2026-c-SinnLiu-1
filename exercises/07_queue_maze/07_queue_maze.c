#include <stdio.h>

#define MAX_ROW 5
#define MAX_COL 5
#define QUEUE_SIZE (MAX_ROW * MAX_COL)

// 迷宫定义：0表示通路，1表示墙壁
int maze[MAX_ROW][MAX_COL] = {
	0, 1, 0, 0, 0,
	0, 1, 0, 1, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 1, 0,
};

// 位置结构体：表示迷宫中的一个坐标点
typedef struct {
    int row;
    int col;
} Position;

// 队列实现（BFS核心数据结构）
Position queue[QUEUE_SIZE];
int front = 0; // 队头指针
int rear = 0;  // 队尾指针

// 前驱数组：记录每个位置的上一个位置，用于回溯路径
Position prev[MAX_ROW][MAX_COL];
// 访问标记数组：避免重复访问
int visited[MAX_ROW][MAX_COL] = {0};

// 入队操作
void enqueue(Position pos) {
    if (rear < QUEUE_SIZE) {
        queue[rear++] = pos;
    }
}

// 出队操作
Position dequeue() {
    Position empty = {-1, -1};
    if (front < rear) {
        return queue[front++];
    }
    return empty;
}

// 检查队列是否为空
int is_queue_empty() {
    return front == rear;
}

// 检查位置是否有效（在迷宫范围内、是通路、未被访问）
int is_valid(int row, int col) {
    return (row >= 0 && row < MAX_ROW &&
            col >= 0 && col < MAX_COL &&
            maze[row][col] == 0 &&
            visited[row][col] == 0);
}

int main(void)
{
    // 方向数组：上、下、左、右（BFS方向顺序不影响最短路径结果）
    int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    
    // 起点和终点定义
    Position start = {0, 0};
    Position end = {4, 4};
    
    // 初始化前驱数组为(-1,-1)，表示无前置节点
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            prev[i][j].row = -1;
            prev[i][j].col = -1;
        }
    }
    
    // BFS初始化：起点入队并标记为已访问
    enqueue(start);
    visited[start.row][start.col] = 1;
    
    // BFS主循环
    while (!is_queue_empty()) {
        Position current = dequeue();
        
        // 检查是否到达终点
        if (current.row == end.row && current.col == end.col) {
            // 回溯路径：从终点反向追溯到起点
            Position path[QUEUE_SIZE];
            int path_length = 0;
            Position p = end;
            
            // 通过前驱数组回溯完整路径
            while (p.row != -1 && p.col != -1) {
                path[path_length++] = p;
                p = prev[p.row][p.col];
            }
            
            // 输出路径（从终点到起点，与测试预期顺序完全一致）
            for (int i = 0; i < path_length; i++) {
                printf("(%d, %d)\n", path[i].row, path[i].col);
            }
            
            return 0;
        }
        
        // 探索当前位置的四个相邻方向
        for (int i = 0; i < 4; i++) {
            int new_row = current.row + dirs[i][0];
            int new_col = current.col + dirs[i][1];
            
            if (is_valid(new_row, new_col)) {
                Position next = {new_row, new_col};
                visited[new_row][new_col] = 1;       // 标记为已访问
                prev[new_row][new_col] = current;    // 记录前驱节点
                enqueue(next);                       // 入队等待探索
            }
        }
    }
    
    // 队列空仍未找到终点，表示无路径
    printf("No path!\n");
    return 0;
}