#include <stdio.h>
#include <stdlib.h>

//输入结点数，申请内存并获取邻接矩阵，返回矩阵指针
int **get_matrix(int node_num){
    int i, j;

    int **matrix = (int**)calloc(node_num, sizeof(int*));
    for(i=0;i<node_num;i++)
        matrix[i] = (int*)calloc(node_num, sizeof(int));

    printf("Please input the matrix:\n");
    for(i=0;i<node_num;i++)
        for(j=0;j<node_num;j++) {
            scanf("%d", &matrix[i][j]);
        }
    return matrix;
}

//打印矩阵
void print_matrix(int **matrix, int node_num){
    int i, j;

    for(i=0;i<node_num;i++) {
        for (j = 0; j < node_num; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

//输入两个矩阵和维数，返回两者之积
int **multiply_matrix(int **a, int **b, int node_num){
    int i, j, k;
    int **result = (int**)calloc(node_num, sizeof(int*));
    for(i=0;i<node_num;i++)
        result[i] = (int*)calloc(node_num, sizeof(int));

    for(i=0;i<node_num;i++)
        for(j=0;j<node_num;j++)
            for(k=0;k<node_num;k++)
                result[i][j] += a[i][k] * b[k][j];
    return result;
}

//输入两个方阵，返回和的指针
int **add_matrix(int **a, int **b, int node_num){
    int i, j;
    int **result = (int**)calloc(node_num, sizeof(int*));
    for(i=0;i<node_num;i++)
        result[i] = (int*)calloc(node_num, sizeof(int));

    for(i=0;i<node_num;i++)
        for(j=0;j<node_num;j++)
            result[i][j] = a[i][j] + b[i][j];
    return result;
}

//输入一个矩阵，返回它的转置矩阵的指针
int **transposed_matrix(int **matrix, int node_num){
    int i, j;
    int **result = (int**)calloc(node_num, sizeof(int*));
    for(i=0;i<node_num;i++)
        result[i] = (int*)calloc(node_num, sizeof(int));

    for(i=0;i<node_num;i++)
        for(j=0;j<node_num;j++)
            result[j][i] = matrix[i][j];

    return result;
}

//输入一个矩阵，返回它和它的转置矩阵的和的指针
int **a_and_trans_a(int **matrix, int node_num){
    int i, j;
    int **result = transposed_matrix(matrix, node_num);

    for(i=0;i<node_num;i++)
        for(j=0;j<node_num;j++){
            result[i][j] += matrix[i][j];
        }
    return result;
}

//输入邻接矩阵，计算并返回可达矩阵的指针
int **accessibility_matrix(int **matrix, int node_num){
    int i, j, n;
    //result储存最后结果
    int **result = (int**)calloc(node_num, sizeof(int*));
    for(i=0;i<node_num;i++)
        result[i] = (int*)calloc(node_num, sizeof(int));

    //tem_matrix暂时储存乘积
    int **tem_matrix = (int**)calloc(node_num, sizeof(int*));
    for(i=0;i<node_num;i++)
        tem_matrix[i] = (int*)calloc(node_num, sizeof(int));

    //先让tem_matrix的值等于matrix
    for(i=0;i<node_num;i++)
        for(j=0;j<node_num;j++)
            tem_matrix[i][j] = matrix[i][j];

    //A(0)
    for(i=0;i<node_num;i++)
        result[i][i] = 1;

    //A(0) and A(1)
    int **tem = result;
    result = add_matrix(result, matrix, node_num);
    //得到和后释放加数的内存
    for(i=0;i<node_num;i++)
        free(tem[i]);
    free(tem);

    //A(0) and A(1) and A(2) and ... and A(n-1)
    for(n=2;n<node_num;n++){
        tem = tem_matrix;
        tem_matrix = multiply_matrix(tem_matrix, matrix, node_num);
        //每一次得到积后释放乘数的内存
        for(i=0;i<node_num;i++)
            free(tem[i]);
        free(tem);

        result = add_matrix(result, tem_matrix, node_num);
    }

    //释放不再使用的tem_matrix的内存
    for(i=0;i<node_num;i++)
        free(tem_matrix[i]);
    free(tem_matrix);

    //A(0)到A(n-1)的和，矩阵某元素若不为0即代表可达，更改为1
    for(i=0;i<node_num;i++)
        for(j=0;j<node_num;j++){
            if(result[i][j] != 0){
                result[i][j] = 1;
            }
        }
    //返回
    return result;
}

int main(){
    int node_num, i, j;
    //获取结点数
    printf("Please input the number of nodes:");
    scanf("%d", &node_num);

    //获取邻接矩阵
    int **matrix = get_matrix(node_num);
    //获取可达矩阵
    int **result = accessibility_matrix(matrix, node_num);

    int is_strong = 1, is_one_side = 1, is_weak = 1;

    //判断是否为强连通图
    for(i=0;i<node_num;i++)
        for(j=0;j<node_num;j++){
            if(result[i][j] != 1){
                is_strong = 0;
                break;
            }
        }

    if(is_strong)
        puts("This graph is strongly connected.");
    else{
        //判断是否为单向连通图
        for(i=0;i<node_num;i++)
            for(j=i;j<node_num;j++){
                if(result[i][j] + result[j][i] <= 0){
                    is_one_side = 0;
                    break;
                }
            }
        if(is_one_side)
            puts("This graph is one-side connected.");
        else{
            //若不是强连通或单向连通图，取邻接矩阵和它转置的和做新邻接矩阵再次求可达矩阵
            matrix = a_and_trans_a(matrix, node_num);
            result = accessibility_matrix(matrix, node_num);
            //判断是否为弱连通图
            for(i=0;i<node_num;i++)
                for(j=0;j<node_num;j++){
                    if(result[i][j] != 1){
                        is_weak = 0;
                        break;
                    }
                }
            if(is_weak)
                puts("This graph is weakly connected.");
            else
                puts("This graph is not connected."); //否则为非连通图
        }
    }
    return 0;
}
