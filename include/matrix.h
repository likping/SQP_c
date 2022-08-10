#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
//这是C的宏定义表示，没有定义C_Matrix就包含下面的代码否则不包含
#ifndef C_Matrix
struct _Matrix{
    int row_size;
    int col_size;
    float* *matrix_entry; //矩阵入口
};
typedef struct _Matrix Matrix; //矩阵类型
void matrix_print(Matrix *matrix); //打印整个矩阵
void matrix_print_part(Matrix *matrix,int start_row,int start_cole);//以star_index开始的列开始打印
void matrix_fill(Matrix *matrix);//使用从标准输入得到的数值填充整个矩阵
Matrix * matrix_callalloc(int matrix_size);//初始化一个单位阵
Matrix * matrix_alloc(int row_size,int col_size);//分配给矩阵内存,返回的是内存块的地址
void matrix_copy(Matrix *matrix1,Matrix *matrix2);//将矩阵1复杂内容给矩阵2，两个矩阵在不同的地址内存块中
Matrix * matrix_multiply(const Matrix *matrix1,const Matrix *matrix2);//矩阵1乘矩阵2，返回一个新的矩阵
Matrix * matrix_pow(Matrix *matrix,int n);//矩阵自乘n次
void matrix_free(Matrix *matrix);//释放矩阵
void row_divide(Matrix *matrix,int pivot);//将矩阵从pivot行处分割开
/**
 * 矩阵行操作
 * @param multiplier_matrix  保存用于行reducation的各种乘数矩阵
 * @param matrix 用于行操作的矩阵
 * @param pivot 该矩阵从pivot位置开始被使用
 * @param row_index 在row_index处进行行操作的
 */
void row_operation(Matrix *multiplier_matrix,Matrix *matrix,int pivot,int row_index);//
/**
 * 行阶梯reduction矩阵
 * @param matrix 操作的矩阵
 * @param zero_control  控制一行零的最大个数
 */
void matrix_row_reduce(Matrix *matrix,int zero_control);
/**
 * @brief 从一个矩阵中减去另一的矩阵
 * @param result 执行减法操作后的矩阵
 * @param matrix1 减矩阵
 * @param matrix2 被减矩阵
 */
void matrix_subtract(Matrix *result,Matrix matrix1,Matrix matrix2);
/**
 * @brief  对一个矩阵执行LU decomposition
 * @param  upper_triangular  指向在哪个矩阵进行操作
 * @param  lower_triangular  指向低的triangular矩阵
 * @note   在调用该函数之前应该使用matrix_callalloc分配内存给低triangular矩
 */
void LU_decompose(Matrix *upper_triangular ,Matrix * lower_triangular);
/**
 * @brief 矩阵相加
 * @param result  矩阵相加后得到的结果
 * @param matrix1 第一个矩阵
 * @param matrix2 第二个矩阵 
 * 
 */
void matrix_add(Matrix *result , Matrix *matrix1,Matrix *matrix2);

/**
 * @brief 对矩阵进行反转操作
 * @param inverse_matrix 将要被反转的矩阵
 */
void matrix_invert(Matrix *inverse_matrix);

/**
 * @brief 检查两个矩阵有相同的行与列
 * @param matrix1 矩阵1
 * @param matrix2 矩阵2 
 * @return   0表示不相等，非零表示行与列是相等的
 */
int matrix_equal_size(Matrix *matrix1,Matrix *matrix2);
/**
 * @brief 检查单独一行是否存在很多零
 * @param matrix 被检查的矩阵
 * @param control_index 单独一行可以被包含的零的个数 
 * 
 */
void error_zeros(Matrix *matrix,int control_index);
/**
 * @brief 当错误出现时终结一个应用
 * @param string 报错信息 
 * 
 */
void terminate(char *string);
#endif