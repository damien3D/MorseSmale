/* ================== Library Information ================== */
// [Name] 
// MeshLib Library
//
// [Developer]
// Xu Dong
// State Key Lab of CAD&CG, Zhejiang University
// 
// [Date]
// 2005-08-05
//
// [Goal]
// A general, flexible, versatile and easy-to-use mesh library for research purpose.
// Supporting arbitrary polygonal meshes as input, but with a 
// primary focus on triangle meshes.

/* ================== File Information ================== */
// [Name]
// Utility.h
//
// [Developer]
// Xu Dong
// State Key Lab of CAD&CG, Zhejiang University
// 
// [Date]
// 2005-08-05
//
// [Goal]
// Defining various utility functions



#include "BasicDataType.h"
#include "..\Numerical\Matrix.h"
#include "..\Numerical\Rotation.h"
#include <math.h>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

#pragma once

typedef enum
{
        IS_EDGE,  // Intersection is on the edge (anywhere on the edge but at the end-points)
        IS_POINT, // Intersection with one of the two end-points
        IS_NONE
} IntersectionType;

class Utility
{
public:
    // string utilities
    void ResolveFileName(string filename, string& file_path, string& file_title, string& file_ext);
    void MakeLower(string& str);
    void MakeUpper(string& str);

    // Free various vectors
    template <class T>
    void FreeVector(T& arr)
    {
        T tmp;
        arr.clear();
        arr.swap(tmp);
    }

    // Flag utilities
    template <class T>
        void SetFlag(T& flag_ele, T flag) { flag_ele |= flag; }

    template <class T>
        void ClearFlag(T& flag_ele, T flag) { flag_ele &= ~flag; }

    template <class T>
        bool IsSetFlag(T& flag_ele, T flag) { return ((flag_ele&flag) == flag); }

    template <class T>
        void ToggleFlag(T& flag_ele, T flag)
    {
        if(IsSetFlag(flag_ele, flag))
            ClearFlag(flag_ele, flag);
        else
            SetFlag(flag_ele, flag);
    }

    // Debug output utility
    template <class T>  // Make sure class T is a vector
    void PrintArray(T& arr, string arrName)
    {
        int n = arr.size();
        cout << '\n';
        for(int i = 0; i < n; ++ i)
        {
            cout << arrName;
            printf("[%8d] = ", i);
            cout << arr[i] << '\n';
        }
    }
    // Mean value and variance calculation
    template <class T>  // Make sure class T is a vector of basic data type (int, float, double, etc... )
    void CalMeanAndStdVar(vector<T>& arr, T& mean, T& std_var)
    {
        int n = arr.size();
        if(n == 0)
            return;

        double Mean = 0.0, Var = 0.0;
        for(int i = 0; i < n; ++ i)
            Mean += arr[i];
        Mean /= (double)n;

        for(i = 0; i < n; ++ i)
        {
            double tmp = arr[i]-Mean;
            Var += tmp*tmp;
        }
        Var /= (double)n;

        mean = (T)Mean;
        std_var  = (T)sqrt(Var);
    }

    // 
    template <class T>  // Make sure class T is a vector of basic data type (int, float, double, etc... )
    bool IsInVector(vector<T>& arr, T& value)
    {
                size_t n = arr.size();
        if(n == 0)
            return false;

        for(size_t i = 0; i < n; ++ i)
                {
                        if (arr[i] == value)
                        {
                                return true;
                        }
                }

                return false;
        }

        template <class T>  // Make sure class T is a vector of basic data type (int, float, double, etc... )
        int IndexInVector(vector<T>& arr, T value)
        {
                int n = (int) arr.size();
                if(n == 0)
                        return -1;

                for(int i = 0; i < n; ++ i)
                {
                        if (arr[i] == value)
                        {
                                return i;
                        }
                }

                return -1;
        }

    // Data set filtering
    template <class T>  // Make sure class T is a vector of basic data type (int, float, double, etc... )
    int DataClamp(vector<T>& arr, T mean, T std_var, double coef)
    {
        int n = arr.size();
        int nClamp = 0;
        for(int i = 0; i < n; ++ i)
        {
            T& data = arr[i];
            double tmp = (data-mean)/std_var;
            if(tmp > coef)
            {
                data = coef*std_var + mean;
                ++ nClamp;
            }
            else if(tmp < -coef)
            {
                data = -coef*std_var + mean;
                ++ nClamp;
            }
        }
        
        return nClamp;
    }

        double VectorMultiplyVector(vector<double>& vec1, vector<double>& vec2)
        {
                if (vec1.size() != vec2.size())
                {
                        return -1;
                }

                double value = 0;
                for (size_t i = 0; i < vec1.size(); i++)
                {
                        value += vec1[i] * vec2[i];
                }

                return value;
        }

        void VectorMultiplyNumber(vector<double>& vec, double number)
        {
                for (size_t i = 0; i < vec.size(); i++)
                {
                        vec[i] *= number;
                }
        }

        void NoramlizeVector(vector<double>& vec)
        {
                double sum = 0;
                for (size_t i = 0; i < vec.size(); i++)
                {
                        sum += vec[i] * vec[i];
                }

                sum = sqrt(sum);

                for (size_t i = 0; i < vec.size(); i++)
                {
                        vec[i] /= sum;
                }
        }

        double VectorDistance(vector<double>& vec1, vector<double>& vec2)
        {
                //
                double dis = 0;

                vector<double> nvec1(vec1);
                vector<double> nvec2(vec2);
                NoramlizeVector(nvec1);
                NoramlizeVector(nvec2);

                for (size_t i = 0; i < vec1.size(); i++)
                {
                        dis += (nvec1[i] - nvec2[i]) * (nvec1[i] - nvec2[i]);
                }

                return dis;
        }

    //////////////////////////////////////////////////////////////////////////
        bool CalTransMatrix(Coord src_v[3], Coord tar_v[3], CMatrix& TransMtx, bool bNormalScale = false);
    bool CalRigidTransMatrix(Coord src_v[3], Coord tar_v[3], CMatrix& TransMtx);
        bool CalTransMatrixAndSplit(Coord src_v[3], Coord tar_v[3], CMatrix& TransMtx, QUATERNION& quat, double S[3][3]);
        bool MatrixSplit(CMatrix TransMtx, QUATERNION& quat, double S[3][3]);
        Coord VectorTransform(CMatrix& TransMtx, Coord v);

    //////////////////////////////////////////////////////////////////////////
    // Given normal and another vector, compute the local frame
    inline void CalLocalFrame(Coord vector, Coord normal, CoordArray& Axis)
    {
        Axis[1] = (cross(normal, vector)).unit();
        Axis[0] = cross(Axis[1], normal);
        Axis[2] = normal;
    }

    template<class T> // Make sure that T is a index array
    void Unique(T& arr)
    {
        sort(arr.begin(), arr.end());
        arr.erase(unique(arr.begin(), arr.end()), arr.end());
    }

        //////////////////////////////////////////////////////////////////////////
        bool SegmentIntersection(double s1x1, double s1y1, double s1x2, double s1y2,
                                     double s2x1, double s2y1, double s2x2, double s2y2,
                                                         double& r, double &s);
        bool SegmentIntersection(Coord& A, Coord& B, Coord& C, Coord& D, double& r, double& s);
        bool TEdgeIntersection(pair<Coord, Coord>& edge, pair<Coord, Coord>& radial, pair<Coord, IntersectionType>& interSection);

};