//////////////////////////////////////////////////////////////////////
// Matrix.h
//
// ²Ù×÷¾ØÕóµÄÀà CMatrix µÄÉùÃ÷½Ó¿Ú
//
// ÖÜ³¤·¢±àÖÆ, 2002/8
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATRIX_H__ACEC32EA_5254_4C23_A8BD_12F9220EF43A__INCLUDED_)
#define AFX_MATRIX_H__ACEC32EA_5254_4C23_A8BD_12F9220EF43A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>
#include "..\MeshLib\BasicDataType.h"
#if !defined(_BITSET_)
#       include <bitset>
#endif // !defined(_BITSET_)

//////////////////////////////////////////////////////////////////////////////////////
//
//(-- class CTokenizer
//
class CTokenizer
{
public:
        CTokenizer(const CString& cs, const CString& csDelim) : m_cs(cs), m_nCurPos(0)
        {
                SetDelimiters(csDelim);
        }
        void SetDelimiters(const CString& csDelim)
        {
                for(int i = 0; i < csDelim.GetLength(); ++i)
                        m_sDelimiter.set(static_cast<BYTE>(csDelim[i]));
        }

        BOOL Next(CString& cs)
        {
                cs.Empty();

                while(m_nCurPos < m_cs.GetLength() && m_sDelimiter[static_cast<BYTE>(m_cs[m_nCurPos])])
                        ++m_nCurPos;

                if(m_nCurPos >= m_cs.GetLength())
                        return FALSE;

                int nStartPos = m_nCurPos;
                while(m_nCurPos < m_cs.GetLength() && !m_sDelimiter[static_cast<BYTE>(m_cs[m_nCurPos])])
                        ++m_nCurPos;
                
                cs = m_cs.Mid(nStartPos, m_nCurPos - nStartPos);

                return TRUE;
        }

        CString Tail() const
        {
                int nCurPos = m_nCurPos;

                while(nCurPos < m_cs.GetLength() && m_sDelimiter[static_cast<BYTE>(m_cs[nCurPos])])
                        ++nCurPos;

                CString csResult;

                if(nCurPos < m_cs.GetLength())
                        csResult = m_cs.Mid(nCurPos);

                return csResult;
        }

private:
        CString m_cs;
        std::bitset<256> m_sDelimiter;
        int m_nCurPos;
};
//
//--) // class CTokenizer
//
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
//
//(-- class CMatrix
//
class CMatrix  
{
        //
        // ¹«ÓÐ½Ó¿Úº¯Êý
        //
public:
        bool SetRotation2Matrix(float angle, Coord axis);
        bool SetScale2Matrix(Coord scale);
        bool SetTrans2Matrix(Coord trans);
        bool TransformVector(Coord& coord);
        void Matrix2Array(double * data);
        void Array2Matrix(double * data);
        void Array2Matrix(double data[3][3]);
        void SetRowCol(int nRows, int nCols);

        //
        // ¹¹ÔìÓëÎö¹¹
        //

        CMatrix();                                                                              // »ù´¡¹¹Ôìº¯Êý
        CMatrix(int nRows, int nCols);                                  // Ö¸¶¨ÐÐÁÐ¹¹Ôìº¯Êý
        CMatrix(int nRows, int nCols, double value[]);  // Ö¸¶¨Êý¾Ý¹¹Ôìº¯Êý
        CMatrix(int nSize);                                                             // ·½Õó¹¹Ôìº¯Êý
        CMatrix(int nSize, double value[]);                             // Ö¸¶¨Êý¾Ý·½Õó¹¹Ôìº¯Êý
        CMatrix(const CMatrix& other);                                  // ¿½±´¹¹Ôìº¯Êý
        BOOL    Init(int nRows, int nCols);                             // ³õÊ¼»¯¾ØÕó   
        BOOL    MakeUnitMatrix(int nSize);                              // ½«·½Õó³õÊ¼»¯Îªµ¥Î»¾ØÕó
        BOOL    MakeZeroMatrix(int nSize);                              // ½«·½Õó³õÊ¼»¯ÎªÁã¾ØÕó
        virtual ~CMatrix();                                                             // Îö¹¹º¯Êý
        void Matrix2Array(double** data);
        void Array2Matrix(double** data);

        //
        // ÊäÈëÓëÏÔÊ¾
        //

        // Êä³öµ½×Ö·ûÖÕ¶Ë
        void Print(CString mtxName);
        // ½«×Ö·û´®×ª»»Îª¾ØÕóÊý¾Ý
        BOOL FromString(CString s, const CString& sDelim = _T(" "), BOOL bLineBreak = TRUE);    
        // ½«¾ØÕó×ª»»Îª×Ö·û´®
        CString ToString(const CString& sDelim = _T(" "), BOOL bLineBreak = TRUE) const;
        // ½«¾ØÕóµÄÖ¸¶¨ÐÐ×ª»»Îª×Ö·û´®
        CString RowToString(int nRow, const CString& sDelim = _T(" ")) const;
        // ½«¾ØÕóµÄÖ¸¶¨ÁÐ×ª»»Îª×Ö·û´®
        CString ColToString(int nCol, const CString& sDelim = _T(" ")) const;

        void Read(string filename);
        void Write(string filename);

        //
        // ÔªËØÓëÖµ²Ù×÷
        //

        BOOL    SetElement(int nRow, int nCol, double value);   // ÉèÖÃÖ¸¶¨ÔªËØµÄÖµ
        double  GetElement(int nRow, int nCol) const;                   // »ñÈ¡Ö¸¶¨ÔªËØµÄÖµ
        void    SetData(double value[]);                                                // ÉèÖÃ¾ØÕóµÄÖµ
        int             GetNumColumns() const;                                                  // »ñÈ¡¾ØÕóµÄÁÐÊý
        int             GetNumRows() const;                                                             // »ñÈ¡¾ØÕóµÄÐÐÊý
        int     GetRowVector(int nRow, double* pVector) const;  // »ñÈ¡¾ØÕóµÄÖ¸¶¨ÐÐ¾ØÕó
        int     GetColVector(int nCol, double* pVector) const;  // »ñÈ¡¾ØÕóµÄÖ¸¶¨ÁÐ¾ØÕó
        double* GetData() const;                                                                // »ñÈ¡¾ØÕóµÄÖµ

        //
        // ÊýÑ§²Ù×÷
        //

        CMatrix& operator=(const CMatrix& other);
        BOOL operator==(const CMatrix& other) const;
        BOOL operator!=(const CMatrix& other) const;
        CMatrix operator+(const CMatrix& other) const;
        CMatrix operator-(const CMatrix& other) const;
        CMatrix operator*(double value) const;
        CMatrix operator*(const CMatrix& other) const;
        // ¸´¾ØÕó³Ë·¨
        BOOL CMul(const CMatrix& AR, const CMatrix& AI, const CMatrix& BR, const CMatrix& BI, CMatrix& CR, CMatrix& CI) const;
        // ¾ØÕóµÄ×ªÖÃ
        CMatrix Transpose() const;

        //
        // Ëã·¨
        //

        // Êµ¾ØÕóÇóÄæµÄÈ«Ñ¡Ö÷Ôª¸ßË¹£­Ô¼µ±·¨
        BOOL InvertGaussJordan();                                               
        // ¸´¾ØÕóÇóÄæµÄÈ«Ñ¡Ö÷Ôª¸ßË¹£­Ô¼µ±·¨
        BOOL InvertGaussJordan(CMatrix& mtxImag);                                 
        // ¶Ô³ÆÕý¶¨¾ØÕóµÄÇóÄæ
        BOOL InvertSsgj();                                              
        // ÍÐ²®Àû×È¾ØÕóÇóÄæµÄ°£À¼ÌØ·½·¨
        BOOL InvertTrench();                                                    
        // ÇóÐÐÁÐÊ½ÖµµÄÈ«Ñ¡Ö÷Ôª¸ßË¹ÏûÈ¥·¨
        double DetGauss();                                                              
        // Çó¾ØÕóÖÈµÄÈ«Ñ¡Ö÷Ôª¸ßË¹ÏûÈ¥·¨
        int RankGauss();
        // ¶Ô³ÆÕý¶¨¾ØÕóµÄÇÇÀïË¹»ù·Ö½âÓëÐÐÁÐÊ½µÄÇóÖµ
        BOOL DetCholesky(double* dblDet);                                                               
        // ¾ØÕóµÄÈý½Ç·Ö½â
        BOOL SplitLU(CMatrix& mtxL, CMatrix& mtxU);                                     
        // Ò»°ãÊµ¾ØÕóµÄQR·Ö½â
        BOOL SplitQR(CMatrix& mtxQ);                                                      
        // Ò»°ãÊµ¾ØÕóµÄÆæÒìÖµ·Ö½â
        BOOL SplitUV(CMatrix& mtxU, CMatrix& mtxV, double eps = 0.000001);                                       
        // Çó¹ãÒåÄæµÄÆæÒìÖµ·Ö½â·¨
        BOOL GInvertUV(CMatrix& mtxAP, CMatrix& mtxU, CMatrix& mtxV, double eps = 0.000001);
        // Ô¼»¯¶Ô³Æ¾ØÕóÎª¶Ô³ÆÈý¶Ô½ÇÕóµÄºÀË¹ºÉ¶ûµÂ±ä»»·¨
        BOOL MakeSymTri(CMatrix& mtxQ, CMatrix& mtxT, double dblB[], double dblC[]);
        // Êµ¶Ô³ÆÈý¶Ô½ÇÕóµÄÈ«²¿ÌØÕ÷ÖµÓëÌØÕ÷ÏòÁ¿µÄ¼ÆËã
        BOOL SymTriEigenv(double dblB[], double dblC[], CMatrix& mtxQ, int nMaxIt = 60, double eps = 0.000001);
        // Ô¼»¯Ò»°ãÊµ¾ØÕóÎªºÕÉê²®¸ñ¾ØÕóµÄ³õµÈÏàËÆ±ä»»·¨
        void MakeHberg();
        // ÇóºÕÉê²®¸ñ¾ØÕóÈ«²¿ÌØÕ÷ÖµµÄQR·½·¨
        BOOL HBergEigenv(double dblU[], double dblV[], int nMaxIt = 60, double eps = 0.000001);
        // ÇóÊµ¶Ô³Æ¾ØÕóÌØÕ÷ÖµÓëÌØÕ÷ÏòÁ¿µÄÑÅ¿É±È·¨
        BOOL JacobiEigenv(double dblEigenValue[], CMatrix& mtxEigenVector, int nMaxIt = 60, double eps = 0.000001);
        // ÇóÊµ¶Ô³Æ¾ØÕóÌØÕ÷ÖµÓëÌØÕ÷ÏòÁ¿µÄÑÅ¿É±È¹ý¹Ø·¨
        BOOL JacobiEigenv2(double dblEigenValue[], CMatrix& mtxEigenVector, double eps = 0.000001);

        //
        // ±£»¤ÐÔÊý¾Ý³ÉÔ±
        //
protected:
        int     m_nNumColumns;                  // ¾ØÕóÁÐÊý
        int     m_nNumRows;                             // ¾ØÕóÐÐÊý
        double* m_pData;                        // ¾ØÕóÊý¾Ý»º³åÇø

        //
        // ÄÚ²¿º¯Êý
        //
private:
        void ppp(double a[], double e[], double s[], double v[], int m, int n);
        void sss(double fg[2], double cs[2]);

};
//
//--) // class CMatrix
//
//////////////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_MATRIX_H__ACEC32EA_5254_4C23_A8BD_12F9220EF43A__INCLUDED_)