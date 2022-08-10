#include"matrix.h"
void matrix_print(Matrix *matrix){
    printf("\n");
    for(int i=0; i <matrix->row_size;i++)
    {
        printf("\t\t");
        for(int j=0;j<matrix->col_size;j++){
            printf("%9.2f",matrix->matrix_entry[i][j]);
        }
        printf("\n");
 
    }
    printf("\n");
}

void matrix_print_part(Matrix *matrix ,int start_row,int start_col){
    for (int i=start_row;i<matrix->row_size;i++){
        for (int j=start_col;j<matrix->col_size;++j){
            printf("\t%9.2f",matrix->matrix_entry[i][j]);
        }
        printf("\n");
    }
}

void matrix_fill(Matrix *matrix){
    printf("Enter the contents of the matrix \n");
    for (int i=0;i<matrix->row_size;i++){
        for(int j=0; j<matrix->col_size;j++){
            scanf("%f",&matrix->matrix_entry[i][j]);
        }
    }
}

/*创建单位矩阵*/
Matrix *matrix_callalloc(int matrix_size){
    Matrix *result =matrix_alloc(matrix_size,matrix_size);
    for (int i=0;i<matrix_size;i++){
        for(int j=0;j<matrix_size;j++){
            if(j==i){
                result->matrix_entry[i][j]=1;
            }
            else{
                result->matrix_entry[i][j]=0;
            }
        }
    }
    return result;

}

Matrix *matrix_alloc(int row_size,int col_size){
    Matrix *new_matrix=malloc(sizeof(Matrix));
    //分配给新的矩阵内存空间
    new_matrix->row_size=row_size;
    new_matrix->col_size=col_size;
    new_matrix->matrix_entry=malloc(new_matrix->row_size*sizeof(float **));
    for(int i=0;i<new_matrix->row_size;i++){
        new_matrix->matrix_entry[i]=malloc(new_matrix->col_size*sizeof(float*));

    }
    return new_matrix;
}

//复制矩阵
void matrix_copy(Matrix *matrix1,Matrix *matrix2){
    for (int i=0;i<matrix1->row_size;i++){
        for (int j=0;j<matrix1->col_size;j++){
            matrix2->matrix_entry[i][j]=matrix1->matrix_entry[i][j];
        }
    }
}

Matrix *matrix_multiply(const Matrix *matrix1,const Matrix *matrix2){
    if(matrix1->col_size!=matrix2->row_size){
        terminate("ERROR: col_size !=row_size ");
    }
    Matrix *result=matrix_alloc(matrix1->row_size,matrix2->col_size);
    for (int i=0;i<matrix1->row_size;i++){
        for (int k=0;k<matrix2->col_size;k++)
        {
            int sum=0;
            for (int j=0;j<matrix1->col_size;j++){
               sum+=matrix1->matrix_entry[i][j]*matrix2->matrix_entry[j][k]; 
            }
            result->matrix_entry[i][k]=sum;
        }
    }
    return result;
}
//矩阵自己相乘
Matrix * matrix_pow(Matrix *matrix ,int index){
    if(index==1){
        Matrix *result=matrix_alloc(matrix->row_size,matrix->col_size);
        matrix_copy(matrix,result);
        return result;
    }
    else{
        Matrix *temp=matrix_alloc(matrix->row_size,matrix->col_size);
        Matrix *result=matrix_alloc(matrix->row_size,matrix->col_size);
        matrix_copy(matrix,temp);
        int count=index/2-1;
        if(count<1){
            matrix_copy(matrix,result);
        }
        else{
            for(int l=0;l<count;l++)
            {
                for(int i=0;i<matrix->row_size;i++){
                    for (int k=0;k<matrix->col_size;k++){
                        int sum=0;
                        for (int j=0;j<matrix->col_size;j++){
                            sum+=(temp->matrix_entry[i][j]*matrix->matrix_entry[j][k]);
                        }
                        result->matrix_entry[i][k]=sum;
                    }
                }
                matrix_copy(result,temp);
            }
            
        }
        /*释放 temp 矩阵*/
        matrix_free(temp);
        Matrix *result_final;
        if(index%2==0){
            result_final=matrix_multiply(result,result);
            matrix_free(result);
        }
        else{
            Matrix *temp=matrix_multiply(matrix,result);
            result_final=matrix_multiply(temp,result);
            matrix_free(temp);
            matrix_free(result);
        }
        return result_final;
    }
}
//释放内存空间
void matrix_free(Matrix *matrix){
    for(int j=0;j<matrix->row_size;j++){
        free(matrix->matrix_entry[j]);
    }
    free(matrix->matrix_entry);
    free(matrix);
}

//函数，它将所有行条目除以对角线的值
void row_divide(Matrix *matrix,int pivot){
    float divisor=matrix->matrix_entry[pivot][pivot];
    float result=0;
    for(int j=pivot;j<matrix->col_size;j++){
        result+=(matrix->matrix_entry[pivot][j])/divisor;
        matrix->matrix_entry[pivot][j]=result;
    }
}
//行操作，从代码分析是不太清楚是什么效果，
void row_operation(Matrix *multiplier_matrix,Matrix *matrix,int pivot,int row_index)
{
    float multiplier=(matrix->matrix_entry[row_index][pivot]/matrix->matrix_entry[pivot][pivot]);
    if(multiplier_matrix!=NULL){
        multiplier_matrix->matrix_entry[row_index][pivot]=multiplier;
    }
    for(int j=0;j<matrix->col_size;j++){
        matrix->matrix_entry[row_index][j]-=multiplier*multiplier_matrix->matrix_entry[pivot][j];
    }
}
//和row_operation联合在使用
void matrix_row_reduce(Matrix *matrix,int zero_control){
    int pivot,row_index;
    float multiplier;
    for(pivot=0;pivot<matrix->row_size;pivot++){
        error_zeros(matrix,zero_control);
        if((matrix->matrix_entry[pivot][pivot]!=1)&&(matrix->matrix_entry[pivot][pivot]!=0))
        {
            row_divide(matrix,pivot);
        }
        for(row_index=pivot+1;row_index<matrix->row_size;row_index++)
        {
            if(matrix->matrix_entry[pivot][pivot]!=0){
                row_operation(NULL,matrix,pivot,row_index);
            }
        }
        for(row_index=pivot-1;row_index>=0;row_index--){
            if(matrix->matrix_entry[pivot][pivot]!=0){
                row_operation(NULL,matrix,pivot,row_index);
            }
        }
    }
}
//LU分解，是矩阵分解的一种，将一个矩阵分成一个下三角矩阵和一个上三角矩阵的乘积
void LU_decompose(Matrix *upper_triangular,Matrix *lower_triangular){
    int pivot,row_index;
    float multiplier;
    for(pivot=0; pivot<upper_triangular->row_size;pivot++){
        error_zeros(upper_triangular,upper_triangular->col_size);
        for(row_index=pivot+1;row_index<upper_triangular->row_size;row_index++){
            if(upper_triangular->matrix_entry[pivot][pivot]){
                row_operation(lower_triangular,upper_triangular,pivot,row_index);
            }
        }
    }
}
//矩阵相减
void matrix_substract(Matrix *result,Matrix *matrix1,Matrix *matrix2){
    int i,j;
    if(!(matrix_equal_size(matrix1,matrix2))||!(matrix_equal_size(matrix2,result))){
        terminate("ERROR:The matrices you are trying to substract have different sizes");
    }
    for(i=0;i<matrix1->row_size;i++){
        for(j=0;j<matrix1->col_size;j++){
            result->matrix_entry[i][j]=matrix1->matrix_entry[i][j]-matrix2->matrix_entry[j][j];
        }
    }
}

//矩阵相加
void matrix_add(Matrix *result,Matrix *matrix1,Matrix *matrix2){
    int i,j;
    if(!(matrix_equal_size(matrix1,matrix2))||!(matrix_equal_size(matrix2,result))){
        terminate("ERROR:The matrices you are trying to add have different sizes");
    }
    for(i=0;i<matrix1->row_size;i++){
        for(j=0;j<matrix1->col_size;j++){
            result->matrix_entry[i][j]=matrix1->matrix_entry[i][j]+matrix2->matrix_entry[i][j];
        }
    }
}
//矩阵反转
void matrix_invert(Matrix *inverse_matrix)
{
    int j,k;
    Matrix *temp_matrix=matrix_alloc(inverse_matrix->row_size,inverse_matrix->col_size*2);
    matrix_copy(inverse_matrix,temp_matrix);
    //在临时矩阵的后面加上单位矩阵
    for(j=0;j<temp_matrix->row_size;j++){
        for(k=3;k<temp_matrix->col_size;k++)
        {
            if(j+3==k){
                temp_matrix->matrix_entry[j][k]=1;
            }
            else{
                temp_matrix->matrix_entry[j][k]=0;
            }
        }
    }
    //这个函数进行行变换乘单位阵
    matrix_row_reduce(temp_matrix,temp_matrix->row_size);

    //复制这个临时矩阵中的反转矩阵到反转矩阵中
    for(j=0;j<temp_matrix->row_size;j++)
    {
        for(k=3;k<temp_matrix->col_size;k++){
            inverse_matrix->matrix_entry[j][k-3]=temp_matrix->matrix_entry[j][k];
        }
    }
    matrix_free(temp_matrix);
}

//判断矩阵的形状是否相等
int matrix_equal_size(Matrix *matrix1,Matrix *matrix2){
    return (matrix1->row_size==matrix2->row_size&&matrix1->col_size==matrix2->col_size);
}

//检查一行是否包含太多的零或者这个样的一行是否存在
void error_zeros(Matrix *matrix,int control_index){
    int i,j,count;
    for(i=0;i<matrix->row_size;i++){
        count=0;
        for(j=0;j<matrix->col_size;j++){
            if(matrix->matrix_entry[i][j]==0){
                count++;
            }
            else{
                return ;
            }
            if(count==control_index){
                fprintf(stdout,"\nProcess fail Because row %d contains %d zeros\n",i+1,control_index);
                matrix_print(matrix);
                exit(1);
            }
        }
    }
}

//终止程序运行
void terminate(char *string){
    fprintf(stdout,"\n%s\n",string);
    fprintf(stdout,"The program is exiting now . . . \n");
    exit(-1);
}

