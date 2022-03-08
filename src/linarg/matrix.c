#include "matrix.h"
#include "math.h"
#include "string.h"
#include "util.h"
void matrix_print(Matrix *matrix)
{
	int i, j;
	printf("\n");
	for (i = 0; i < matrix->row_size; i++)
	{
		printf("\t\t");
		for (j = 0; j < matrix->col_size; j++)
		{
			printf("%9.2f", matrix->matrix_entry[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void matrix_print_part(Matrix *matrix, int start_index)
{
	int j, i;
	for (i = 0; i < matrix->row_size; ++i)
	{
		for (j = start_index; j < matrix->col_size; ++j)
		{
			printf("\t\t%9.2f", matrix->matrix_entry[i][j]);
		}
		printf("\n");
	}
}

void matrix_fill(Matrix *matrix)
{
	int i, j;
	printf("Enter the contents of the matrix:\n");
	for (i = 0; i < matrix->row_size; i++)
	{
		for (j = 0; j < matrix->col_size; j++)
		{
			scanf("%f", &matrix->matrix_entry[i][j]);
		}
	}
}

/*Function to create an identity matrix	*/
Matrix *matrix_callalloc(int matrix_size)
{
	Matrix *result = matrix_alloc(matrix_size, matrix_size);
	int i, j;

	for (i = 0; i < matrix_size; i += 1)
	{
		for (j = 0; j < matrix_size; j += 1)
		{
			if (j == i)
			{
				result->matrix_entry[i][j] = 1;
			}

			else
			{
				result->matrix_entry[i][j] = 0;
			}
		}
	}

	return result;
}

Matrix *matrix_alloc(int row_size, int col_size)
{
	int j;
	Matrix *new_matrix = malloc(sizeof(Matrix));

	// Allocating memory for the new matrix structure
	new_matrix->row_size = row_size;
	new_matrix->col_size = col_size;
	new_matrix->matrix_entry = malloc(new_matrix->row_size * sizeof(double *));
	for (j = 0; j < new_matrix->row_size; j++)
	{
		new_matrix->matrix_entry[j] = malloc(new_matrix->col_size * sizeof(double));
	}
	for (int i = 0; i < row_size; i++)
	{
		for (int j = 0; j < col_size; j++)
		{
			new_matrix->matrix_entry[i][j] == 0.0;
		}
	}

	return new_matrix;
}

/*Copies Matrix1 into matrix2 */
void matrix_copy(Matrix *matrix1, Matrix *matrix2)
{
	int i, j;
	for (i = 0; i < matrix1->row_size; i += 1)
	{
		for (j = 0; j < matrix1->col_size; j += 1)
		{
			matrix2->matrix_entry[i][j] = matrix1->matrix_entry[i][j];
		}
	}
}

Matrix *matrix_multiply(const Matrix *matrix1, const Matrix *matrix2)
{
	int i, j, k, sum;
	if (matrix1->col_size != matrix2->row_size)
	{
		terminate("ERROR: The number columns of matrix1  != number of rows in matrix2!");
	}
	Matrix *result = matrix_alloc(matrix1->row_size, matrix2->col_size);
	for (i = 0; i < matrix1->row_size; i += 1)
	{
		for (k = 0; k < matrix2->col_size; k += 1)
		{
			sum = 0;

			for (j = 0; j < matrix1->col_size; j += 1)
			{
				sum += matrix1->matrix_entry[i][j] * matrix2->matrix_entry[j][k];
			}

			result->matrix_entry[i][k] = sum;
		}
	}
	return result;
}

Matrix *matrix_pow(Matrix *matrix, int index)
{
	if (index == 1)
	{
		Matrix *result = matrix_alloc(matrix->row_size, matrix->col_size);
		matrix_copy(matrix, result);
		return result;
	}
	else
	{

		int i, j, k, l, sum, count;

		Matrix *temp = matrix_alloc(matrix->row_size, matrix->col_size);   // Allocating space for a temporal matrix
		Matrix *result = matrix_alloc(matrix->row_size, matrix->col_size); // Allocating space for the result matrix

		matrix_copy(matrix, temp);

		count = index / 2 - 1;
		if (count < 1)
		{
			matrix_copy(matrix, result);
		}

		else
		{
			for (l = 0; l < count; l += 1)
			{
				for (i = 0; i < matrix->row_size; i += 1)
				{
					for (k = 0; k < matrix->col_size; k += 1)
					{
						sum = 0;

						for (j = 0; j < matrix->col_size; j += 1)
						{
							sum += (temp->matrix_entry[i][j] * matrix->matrix_entry[j][k]);
						}

						result->matrix_entry[i][k] = sum;
					}
				}

				/* Copying the result matrix into the temp matrix for further
				 * multiplication */
				matrix_copy(result, temp);
			}
		}

		/*	Freeing the temp matrix		*/
		matrix_free(temp);
		if (index % 2 == 0)
		{
			Matrix *result_final = matrix_multiply(result, result);
			/* Freeing the result Matrix	*/
			matrix_free(result);

			return result_final;
		}

		else
		{
			Matrix *temp = matrix_multiply(matrix, result);
			Matrix *result_final = matrix_multiply(temp, result);

			/* Freeing the temp matrix		*/
			matrix_free(temp);

			/* Freeing the result Matrix	*/
			matrix_free(result);

			return result_final;
		} // End of else statement
	}
}

void matrix_free(Matrix *matrix)
{
	int j;
	for (j = 0; j < matrix->row_size; j++)
	{
		free(matrix->matrix_entry[j]);
	}
	free(matrix->matrix_entry);
	free(matrix);
}

/*Function which divides all row entries by the value of a the diagonal */
void row_divide(Matrix *matrix, int pivot)
{
	int j;
	double divisor = matrix->matrix_entry[pivot][pivot],
		   result;

	for (j = pivot; j < matrix->col_size; j++)
	{
		result = (matrix->matrix_entry[pivot][j] / divisor);
		matrix->matrix_entry[pivot][j] = result;
	}
}

/*Function to carry out row operations*/
void row_operation(Matrix *multiplier_matrix, Matrix *matrix, int pivot, int row_index)
{
	int j;
	double multiplier = (matrix->matrix_entry[row_index][pivot] / matrix->matrix_entry[pivot][pivot]);
	// Loop which checks if matrix is provided to store the multiplier
	if (multiplier_matrix != NULL)
	{
		multiplier_matrix->matrix_entry[row_index][pivot] = multiplier;
	}

	for (j = 0; j < matrix->col_size; j++)
	{
		matrix->matrix_entry[row_index][j] -= multiplier * matrix->matrix_entry[pivot][j];
	}
}

void matrix_row_reduce(Matrix *matrix, int zero_control)
{
	int pivot, row_index;
	double multiplier;
	for (pivot = 0; pivot < matrix->row_size; pivot++)
	{

		error_zeros(matrix, zero_control); // Function checks if there are too many zeros in a single row
		if ((matrix->matrix_entry[pivot][pivot] != 1) && (matrix->matrix_entry[pivot][pivot] != 0))
		{
			row_divide(matrix, pivot);
		}

		for (row_index = pivot + 1; row_index < matrix->row_size; row_index++)
		{
			if (matrix->matrix_entry[pivot][pivot] != 0)
			{
				row_operation(NULL, matrix, pivot, row_index);
			}
		}

		for (row_index = pivot - 1; row_index >= 0; row_index--)
		{
			if (matrix->matrix_entry[pivot][pivot] != 0)
			{
				row_operation(NULL, matrix, pivot, row_index);
			}
		}
	}
}

void LU_decompose(Matrix *upper_triangular, Matrix *lower_triangular)
{
	int pivot, row_index;
	double multiplier;
	for (pivot = 0; pivot < upper_triangular->row_size; pivot++)
	{

		error_zeros(upper_triangular, upper_triangular->col_size); // Function checks if there are too many zeros in a single row
		for (row_index = pivot + 1; row_index < upper_triangular->row_size; row_index++)
		{
			if (upper_triangular->matrix_entry[pivot][pivot] != 0)
			{

				row_operation(lower_triangular, upper_triangular, pivot, row_index);
			}
		}
	}
}

void matrix_subtract(Matrix *result, Matrix *matrix1, Matrix *matrix2)
{
	int i, j;

	if (!(matrix_equal_size(matrix1, matrix2)) ||
		!(matrix_equal_size(matrix2, result)))
	{
		terminate("ERROR: The matrices you are trying to subtract have different sizes");
	}

	for (i = 0; i < matrix1->row_size; i += 1)
	{
		for (j = 0; j < matrix1->col_size; j += 1)
		{
			result->matrix_entry[i][j] = matrix1->matrix_entry[i][j] - matrix2->matrix_entry[i][j];
		}
	}
}

void matrix_add(Matrix *result, Matrix *matrix1, Matrix *matrix2)
{
	int i, j;
	if (!(matrix_equal_size(matrix1, matrix2)) ||
		!(matrix_equal_size(matrix2, result)))
	{
		terminate("ERROR: The matrices you are trying to add  have different sizes");
	}
	for (i = 0; i < matrix1->row_size; i += 1)
	{
		for (j = 0; j < matrix1->col_size; j += 1)
		{
			result->matrix_entry[i][j] = matrix1->matrix_entry[i][j] + matrix2->matrix_entry[i][j];
		}
	}
}

void matrix_invert(Matrix *inverse_matrix)
{
	int j, k;
	/*Temporal matrix used in this function */
	Matrix *temp_matrix = matrix_alloc(inverse_matrix->row_size, inverse_matrix->col_size * 2);

	matrix_copy(inverse_matrix, temp_matrix);

	/* Adding an identity matrix at the end of the temporal matrix */
	for (j = 0; j < temp_matrix->row_size; j++)
	{
		for (k = 3; k < temp_matrix->col_size; k++)
		{
			if (j + 3 == k)
			{
				temp_matrix->matrix_entry[j][k] = 1;
			}
			else
			{
				temp_matrix->matrix_entry[j][k] = 0;
			}
		}
	}

	matrix_row_reduce(temp_matrix, temp_matrix->row_size);

	/* Copying the inverse matrix from the temp_matrix to the  invse_matrix */
	for (j = 0; j < temp_matrix->row_size; j++)
	{
		for (k = 3; k < temp_matrix->col_size; k++)
		{
			inverse_matrix->matrix_entry[j][k - 3] = temp_matrix->matrix_entry[j][k];
		}
	}

	matrix_free(temp_matrix);
}

void matrix_inverse(Matrix *mat, Matrix *inv)
{
	if (mat->col_size != mat->row_size)
	{
		terminate("ERROR: The matrix to inverse must have same colsize and rowsize");
	}
	int size = mat->row_size;
	Matrix *L = matrix_alloc(size, size);
	Matrix *U = matrix_alloc(size, size);
	Matrix *Lni = matrix_alloc(size, size);
	Matrix *Uni = matrix_alloc(size, size);
	double s;
	for (size_t i = 0; i < size; i++)
	{
		L->matrix_entry[i][i] = 1.0;
	}
	for (size_t i = 0; i < size; i++)
	{
		U->matrix_entry[0][i] = mat->matrix_entry[0][i];
	}
	for (size_t i = 1; i < size; i++)
	{
		L->matrix_entry[i][0] = mat->matrix_entry[i][0] / U->matrix_entry[0][0];
	}
	for (size_t k = 1; k < size; k++)
	{
		for (size_t j = k; j < size; j++)
		{
			s = 0.0;
			for (size_t t = 0; t < k; t++)
			{
				s += L->matrix_entry[k][t] * U->matrix_entry[t][j];
			}
			U->matrix_entry[k][j] = mat->matrix_entry[k][j] - s;
		}
		for (size_t i = k; i < size; i++)
		{
			s = 0.0;
			for (size_t t = 0; t < k; t++)
			{
				s += L->matrix_entry[i][t] * U->matrix_entry[t][k];
			}
			L->matrix_entry[i][k] = (mat->matrix_entry[i][k] - s) / U->matrix_entry[k][k];
		}
	}
	for (size_t j = 0; j < size; j++)
	{
		for (size_t i = j; i < size; i++)
		{
			if (i == j)
				Lni->matrix_entry[i][j] = 1 / L->matrix_entry[i][j];
			else if (i < j)
				Lni->matrix_entry[i][j] = 0;
			else
			{
				s = 0.0;
				for (int k = 0; k < i; k++)
				{
					s = s + L->matrix_entry[i][k] + Lni->matrix_entry[k][j];
				}
				Lni->matrix_entry[i][j] = -Lni->matrix_entry[j][j] * s;
			}
		}
	}
	for (size_t j = 0; j < size; j++)
	{
		for (int i = j; i >= 0; i--)
		{
			if (i == j)
				Uni->matrix_entry[i][j] = 1. / U->matrix_entry[i][j];
			else if (i > j)
				Uni->matrix_entry[i][j] = 0.;
			else
			{
				s = 0.0;
				for (size_t k = i + 1; k <= j; k++)
				{
					s = s + U->matrix_entry[i][k] * Uni->matrix_entry[k][j];
				}
				Uni->matrix_entry[i][j] = -1. / U->matrix_entry[i][i] * s;
			}
		}
	}
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			for (size_t k = 0; k < size; k++)
			{
				inv->matrix_entry[i][j] = Uni->matrix_entry[i][k] + Lni->matrix_entry[k][j];
			}
		}
	}
	return;
}

int matrix_equal_size(Matrix *matrix1, Matrix *matrix2)
{

	return (matrix1->row_size == matrix2->row_size &&
			matrix1->col_size == matrix2->col_size);
}

/*
  This function checks if there is a line containing too many zero's and it exits
  if such a line is found
*/
void error_zeros(Matrix *matrix, int control_index)
{
	int i, j, count;
	for (i = 0; i < matrix->row_size; i++)
	{
		count = 0;
		for (j = 0; j < matrix->col_size; j++)
		{
			if (matrix->matrix_entry[i][j] == 0)
			{
				count++;
			}
			else
			{
				return;
			}
			if (count == control_index)
			{
				fprintf(stdout, "\nProcess fail because row %d contains %d  zeros\n", i + 1, control_index);
				matrix_print(matrix);
				exit(1);
			}
		}
	}
}

double vector_2norm(Matrix *matrix)
{
	if (matrix->col_size != 1)
	{
		terminate("vector_2norm must input a column vector");
	}
	double norm = 0.;
	for (int i = 0; i < matrix->row_size; i++)
	{
		norm += pow(matrix->matrix_entry[i][0], 2.);
	}
	return sqrt(norm);
}

double matrix_F2norm(Matrix *matrix)
{
	double norm = 0.;
	for (int i = 0; i < matrix->row_size; i++)
	{
		for (int j = 0; j < matrix->col_size; j++)
		{
			norm += pow(matrix->matrix_entry[i][j], 2.);
		}
	}
	return sqrt(norm);
}
