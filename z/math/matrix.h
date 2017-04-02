/**
 * File:            matrix.h
 * Namespace:       z::math
 *
 * Description:     Template class for square matrices
 *                  of size at least 2x2.
 *                  Includes methods for calculating determinants.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   1 Mar. 2017
**/

#pragma once
#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

namespace z
{
    namespace math
    {
        ///square matrix is of constant size NxN
        template <unsigned int N>
        class matrix
        {
        public:
            double value[N][N];

            ///Method for calculating determinant
            double det()
            {
                //the determinant of an NxN matrix requires
                //the determinant of an (N-1)x(N-1) matrix.
                matrix<N-1> mat;

                double result=0;


                for (unsigned int col=0; col<N; col++)
                {
                    for (unsigned int y=0; y<N-1; y++)
                    {
                        int xoffs = 0;

                        for(unsigned int x=0; x<N-1; x++)
                        {
                            if (xoffs == col)
                                xoffs++;

                            mat.value[y][x] = value[y+1][xoffs];

                            xoffs++;
                        }
                    }

                    if(col % 2)
                        result -= value[0][col] * mat.det();
                    else
                        result += value[0][col] * mat.det();
                }


                return result;
            }
        };


        ///quickly calculate the determinant of a 2x2 matrix.
        template<>
        double matrix<2>::det()
        {
            return (value[0][0]*value[1][1]-value[1][0]*value[0][1]);
        }
    }
}

#endif // MATRIX_H_INCLUDED
