#include "matrix.h"
#include "linearequations.h"
#include "f2c.h"
#include "slsqp.h"

#define M_alloc_variable(x, type, value)    \
    type *x = (type *)malloc(sizeof(type)); \
    *x = value

int main(void)
{
    Matrix *A = matrix_alloc(3, 3);
    Matrix *B = matrix_alloc(3, 3);
    Matrix *C = matrix_callalloc(3);
    A->matrix_entry[0][0] = 4.;
    A->matrix_entry[0][1] = 2.;
    A->matrix_entry[0][2] = 1.;
    A->matrix_entry[1][0] = 12.;
    A->matrix_entry[1][1] = 3.;
    A->matrix_entry[1][2] = 2.;
    A->matrix_entry[2][0] = 4.;
    A->matrix_entry[2][1] = 5.;
    A->matrix_entry[2][2] = 2.;

    // matrix_copy(A, B); // B = A
    // printf("MatrixA:\n");
    // matrix_print(A);
    // printf("MatrixA^-1:\n");
    // matrix_invert(B); // B = B^-1
    // matrix_print(B);
    // matrix_print(matrix_multiply(A, B));
    // matrix_print(matrix_multiply(B, A));

    //! F,C,G,A        MUST ALL BE SET BY THE USER BEFORE EACH CALL.
    M_alloc_variable(m, integer, 0);
    // IS THE TOTAL NUMBER OF CONSTRAINTS, M .GE. 0
    M_alloc_variable(meq, integer, 0);
    // IS THE NUMBER OF EQUALITY CONSTRAINTS, MEQ .GE. 0
    M_alloc_variable(la, integer, 0);
    // SEE A, LA .GE. MAX(M,1)
    M_alloc_variable(n, integer, 0);
    // IS THE NUMBER OF VARIBLES, N .GE. 1
    M_alloc_variable(x, doublereal, 0);
    // X() STORES THE CURRENT ITERATE OF THE N VECTOR X  *
    // ON ENTRY X() MUST BE INITIALIZED. ON EXIT X()
    // STORES THE SOLUTION VECTOR X IF MODE = 0
    M_alloc_variable(xl, doublereal, 0);
    // XL() STORES AN N VECTOR OF LOWER BOUNDS XL TO X.
    // ELEMENTS MAY BE NAN TO INDICATE NO LOWER BOUND.
    M_alloc_variable(xu, doublereal, 0);
    // XU() STORES AN N VECTOR OF UPPER BOUNDS XU TO X.
    // ELEMENTS MAY BE NAN TO INDICATE NO UPPER BOUND.
    M_alloc_variable(f, doublereal, 0);
    // IS THE VALUE OF THE OBJECTIVE FUNCTION.
    M_alloc_variable(c__, doublereal, 0);
    // C() STORES THE M VECTOR C OF CONSTRAINTS,
    // EQUALITY CONSTRAINTS (IF ANY) FIRST.
    // DIMENSION OF C MUST BE GREATER OR EQUAL LA,
    // which must be GREATER OR EQUAL MAX(1,M).
    M_alloc_variable(g, doublereal, 0);
    // G() STORES THE N VECTOR G OF PARTIALS OF THE
    // OBJECTIVE FUNCTION; DIMENSION OF G MUST BE GREATER OR EQUAL N+1.
    M_alloc_variable(a, doublereal, 0);
    // THE LA BY N + 1 ARRAY A() STORES
    // THE M BY N MATRIX A OF CONSTRAINT NORMALS.
    // A() HAS FIRST DIMENSIONING PARAMETER LA,
    // WHICH MUST BE GREATER OR EQUAL MAX(1,M).
    M_alloc_variable(acc, doublereal, 0);
    // ABS(ACC) CONTROLS THE FINAL ACCURACY.             *
    // IF ACC .LT. ZERO AN EXACT LINESEARCH IS PERFORMED,*
    // OTHERWISE AN ARMIJO-TYPE LINESEARCH IS USED.      *
    M_alloc_variable(iter, integer, 0);
    // PRESCRIBES THE MAXIMUM NUMBER OF ITERATIONS.      *
    // ON EXIT ITER INDICATES THE NUMBER OF ITERATIONS.
    M_alloc_variable(mode, integer, 0);
    
    M_alloc_variable(w, doublereal, 0);
    M_alloc_variable(l_w__, integer, 0);
    M_alloc_variable(jw, integer, 0);
    M_alloc_variable(l_jw__, integer, 0);
    M_alloc_variable(alpha, doublereal, 0);
    M_alloc_variable(f0, doublereal, 0);
    M_alloc_variable(gs, doublereal, 0);
    M_alloc_variable(h1, doublereal, 0);
    M_alloc_variable(h2, doublereal, 0);
    M_alloc_variable(h3, doublereal, 0);
    M_alloc_variable(h4, doublereal, 0);
    M_alloc_variable(t, doublereal, 0);
    M_alloc_variable(t0, doublereal, 0);
    M_alloc_variable(tol, doublereal, 0);
    M_alloc_variable(iexact, integer, 0);
    M_alloc_variable(incons, integer, 0);
    M_alloc_variable(ireset, integer, 0);
    M_alloc_variable(itermx, integer, 0);
    M_alloc_variable(line, integer, 0);
    M_alloc_variable(n1, integer, 0);
    M_alloc_variable(n2, integer, 0);
    M_alloc_variable(n3, integer, 0);
    int state = slsqp_(m,
                       meq,
                       la,
                       n,
                       x,
                       xl,
                       xu,
                       f,
                       c__,
                       g,
                       a,
                       acc,
                       iter,
                       mode,
                       w,
                       l_w__,
                       jw,
                       l_jw__,
                       alpha,
                       f0,
                       gs,
                       h1,
                       h2,
                       h3,
                       h4,
                       t,
                       t0,
                       tol,
                       iexact,
                       incons,
                       ireset,
                       itermx,
                       line,
                       n1,
                       n2,
                       n3);
    printf("state%d", state);
}