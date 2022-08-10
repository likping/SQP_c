#include"matrix.h"
#include <stdio.h>
int main(){
    /**
     * @brief 
     * 创建矩阵
     */
    // 创建单位矩阵
    Matrix *mat=matrix_callalloc(4);
    // 打印单位矩阵
    matrix_print(mat);
    //填充矩阵
    matrix_fill(mat);
    // 打印部分矩阵
    matrix_print_part(mat,2,2);
    //分配矩阵空间,默认赋值0
    Matrix *empty_mat=matrix_alloc(4,4);
    matrix_print(empty_mat);
    matrix_copy(mat,empty_mat);
    matrix_print(empty_mat);
    // printf("Hello World\n");
    /**
     * @brief 
     * 矩阵运算 
     */
    
    return 0;
}
