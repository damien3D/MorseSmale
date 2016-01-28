//////////////////////////////////////////////////////////////////////
// Matrix.cpp
//
// ²Ù×÷¾ØÕóµÄÀà CMatrix µÄÊµÏÖÎÄ¼þ
//
// ÖÜ³¤·¢±àÖÆ, 2002/8
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Matrix.h"
#include "Rotation.h"
#include  <iostream>   
#include  <string>   
#include  <fstream>   
using namespace std;   



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// »ù±¾¹¹Ôìº¯Êý
//////////////////////////////////////////////////////////////////////
CMatrix::CMatrix()
{
	m_nNumColumns = 1;
	m_nNumRows = 1;
	m_pData = NULL;
	BOOL bSuccess = Init(m_nNumRows, m_nNumColumns);
	ASSERT(bSuccess);
}

//////////////////////////////////////////////////////////////////////
// Ö¸¶¨ÐÐÁÐ¹¹Ôìº¯Êý
//
// ²ÎÊý£º
// 1. int nRows - Ö¸¶¨µÄ¾ØÕóÐÐÊý
// 2. int nCols - Ö¸¶¨µÄ¾ØÕóÁÐÊý
//////////////////////////////////////////////////////////////////////
CMatrix::CMatrix(int nRows, int nCols)
{
	m_nNumRows = nRows;
	m_nNumColumns = nCols;
	m_pData = NULL;
	BOOL bSuccess = Init(m_nNumRows, m_nNumColumns);
	ASSERT(bSuccess);
}

//////////////////////////////////////////////////////////////////////
// Ö¸¶¨Öµ¹¹Ôìº¯Êý
//
// ²ÎÊý£º
// 1. int nRows - Ö¸¶¨µÄ¾ØÕóÐÐÊý
// 2. int nCols - Ö¸¶¨µÄ¾ØÕóÁÐÊý
// 3. double value[] - Ò»Î¬Êý×é£¬³¤¶ÈÎªnRows*nCols£¬´æ´¢¾ØÕó¸÷ÔªËØµÄÖµ
//////////////////////////////////////////////////////////////////////
CMatrix::CMatrix(int nRows, int nCols, double value[])
{
	m_nNumRows = nRows;
	m_nNumColumns = nCols;
	m_pData = NULL;
	BOOL bSuccess = Init(m_nNumRows, m_nNumColumns);
	ASSERT(bSuccess);

	SetData(value);
}

//////////////////////////////////////////////////////////////////////
// ·½Õó¹¹Ôìº¯Êý
//
// ²ÎÊý£º
// 1. int nSize - ·½ÕóÐÐÁÐÊý
//////////////////////////////////////////////////////////////////////
CMatrix::CMatrix(int nSize)
{
	m_nNumRows = nSize;
	m_nNumColumns = nSize;
	m_pData = NULL;
	BOOL bSuccess = Init(nSize, nSize);
	ASSERT (bSuccess);
}

//////////////////////////////////////////////////////////////////////
// ·½Õó¹¹Ôìº¯Êý
//
// ²ÎÊý£º
// 1. int nSize - ·½ÕóÐÐÁÐÊý
// 2. double value[] - Ò»Î¬Êý×é£¬³¤¶ÈÎªnRows*nRows£¬´æ´¢·½Õó¸÷ÔªËØµÄÖµ
//////////////////////////////////////////////////////////////////////
CMatrix::CMatrix(int nSize, double value[])
{
	m_nNumRows = nSize;
	m_nNumColumns = nSize;
	m_pData = NULL;
	BOOL bSuccess = Init(nSize, nSize);
	ASSERT (bSuccess);

	SetData(value);
}

//////////////////////////////////////////////////////////////////////
// ¿½±´¹¹Ôìº¯Êý
//
// ²ÎÊý£º
// 1. const CMatrix& other - Ô´¾ØÕó
//////////////////////////////////////////////////////////////////////
CMatrix::CMatrix(const CMatrix& other)
{
	m_nNumColumns = other.GetNumColumns();
	m_nNumRows = other.GetNumRows();
	m_pData = NULL;
	BOOL bSuccess = Init(m_nNumRows, m_nNumColumns);
	ASSERT(bSuccess);

	// copy the pointer
	memcpy(m_pData, other.m_pData, sizeof(double)*m_nNumColumns*m_nNumRows);
}

//////////////////////////////////////////////////////////////////////
// Îö¹¹º¯Êý
//////////////////////////////////////////////////////////////////////
CMatrix::~CMatrix()
{
	if (m_pData)
	{
		delete[] m_pData;
		m_pData = NULL;
	}
}

//////////////////////////////////////////////////////////////////////
// ³õÊ¼»¯º¯Êý
//
// ²ÎÊý£º
// 1. int nRows - Ö¸¶¨µÄ¾ØÕóÐÐÊý
// 2. int nCols - Ö¸¶¨µÄ¾ØÕóÁÐÊý
//
// ·µ»ØÖµ£ºBOOL ÐÍ£¬³õÊ¼»¯ÊÇ·ñ³É¹¦
//////////////////////////////////////////////////////////////////////
BOOL CMatrix::Init(int nRows, int nCols)
{
	if (m_pData)
	{
		delete[] m_pData;
		m_pData = NULL;
	}

	m_nNumRows = nRows;
	m_nNumColumns = nCols;
	int nSize = nCols*nRows;
	if (nSize < 0)
		return FALSE;

	// ·ÖÅäÄÚ´æ
	m_pData = new double[nSize];
	
	if (m_pData == NULL)
		return FALSE;					// ÄÚ´æ·ÖÅäÊ§°Ü
	if (IsBadReadPtr(m_pData, sizeof(double) * nSize))
		return FALSE;

	// ½«¸÷ÔªËØÖµÖÃ0
	memset(m_pData, 0, sizeof(double) * nSize);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// ½«·½Õó³õÊ¼»¯Îªµ¥Î»¾ØÕó
//
// ²ÎÊý£º
// 1. int nSize - ·½ÕóÐÐÁÐÊý
//
// ·µ»ØÖµ£ºBOOL ÐÍ£¬³õÊ¼»¯ÊÇ·ñ³É¹¦
//////////////////////////////////////////////////////////////////////
BOOL CMatrix::MakeUnitMatrix(int nSize)
{
	if (! Init(nSize, nSize))
		return FALSE;

	for (int i=0; i<nSize; ++i)
		for (int j=0; j<nSize; ++j)
			if (i == j)
				SetElement(i, j, 1);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// ½«·½Õó³õÊ¼»¯ÎªÁã¾ØÕó
//
// ²ÎÊý£º
// 1. int nSize - ·½ÕóÐÐÁÐÊý
//
// ·µ»ØÖµ£ºBOOL ÐÍ£¬³õÊ¼»¯ÊÇ·ñ³É¹¦
//////////////////////////////////////////////////////////////////////
BOOL CMatrix::MakeZeroMatrix(int nSize)
{
	if (! Init(nSize, nSize))
		return FALSE;

	for (int i=0; i<nSize; ++i)
		for (int j=0; j<nSize; ++j)
			SetElement(i, j, 0);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// ½«×Ö·û´®×ª»¯Îª¾ØÕóµÄÖµ
//
// ²ÎÊý£º
// 1. CString s - Êý×ÖºÍ·Ö¸ô·û¹¹³ÉµÄ×Ö·û´®
// 2. const CString& sDelim - Êý×ÖÖ®¼äµÄ·Ö¸ô·û£¬Ä¬ÈÏÎª¿Õ¸ñ
// 3. BOOL bLineBreak - ÐÐÓëÐÐÖ®¼äÊÇ·ñÓÐ»Ø³µ»»ÐÐ·û£¬Ä¬ÈÏÎªÕæ(ÓÐ»»ÐÐ·û)
//         µ±¸Ã²ÎÊýÎªFALSEÊ±£¬ËùÓÐÔªËØÖµ¶¼ÔÚÒ»ÐÐÖÐÊäÈë£¬×Ö·û´®µÄµÚÒ»¸ö
//         ÊýÖµÓ¦Îª¾ØÕóµÄÐÐÊý£¬µÚ¶þ¸öÊýÖµÓ¦Îª¾ØÕóµÄÁÐÊý
//
// ·µ»ØÖµ£ºBOOL ÐÍ£¬×ª»»ÊÇ·ñ³É¹¦
//////////////////////////////////////////////////////////////////////
BOOL CMatrix::FromString(CString s, const CString& sDelim /*= " "*/, BOOL bLineBreak /*= TRUE*/)
{
	if (s.IsEmpty())
		return FALSE;

	// ·ÖÐÐ´¦Àí
	if (bLineBreak)
	{
		CTokenizer tk(s, _T("\r\n"));

		CStringList ListRow;
		CString sRow;
		while (tk.Next(sRow))
		{
			sRow.TrimLeft();
			sRow.TrimRight();
			if (sRow.IsEmpty())
				break;

			ListRow.AddTail(sRow);
		}

		// ÐÐÊý
		m_nNumRows = (int) ListRow.GetCount();

		sRow = ListRow.GetHead();
		CTokenizer tkRow(sRow, sDelim);
		CString sElement;
		// ÁÐÊý
		m_nNumColumns = 0;
		while (tkRow.Next(sElement))
		{
			m_nNumColumns++;
		}

		// ³õÊ¼»¯¾ØÕó
		if (! Init(m_nNumRows, m_nNumColumns))
			return FALSE;

		// ÉèÖÃÖµ
		POSITION pos = ListRow.GetHeadPosition();
		for (int i=0; i<m_nNumRows; i++)
		{
			sRow = ListRow.GetNext(pos);
			int j = 0;
			CTokenizer tkRow(sRow, sDelim);
			while (tkRow.Next(sElement))
			{
				sElement.TrimLeft();
				sElement.TrimRight();
				double v = atof((const char*) sElement.GetBuffer());
				SetElement(i, j++, v);
			}
		}

		return TRUE;
	}
	
	// ²»·ÖÐÐ£¨µ¥ÐÐ£©´¦Àí

	CTokenizer tk(s, sDelim);

	CString sElement;
	
	// ÐÐÊý
	tk.Next(sElement);
	sElement.TrimLeft();
	sElement.TrimRight();
	m_nNumRows = atoi((const char*) sElement.GetBuffer());

	// ÁÐÊý
	tk.Next(sElement);
	sElement.TrimLeft();
	sElement.TrimRight();
	m_nNumColumns = atoi((const char*) sElement.GetBuffer());

	// ³õÊ¼»¯¾ØÕó
	if (! Init(m_nNumRows, m_nNumColumns))
		return FALSE;

	// ÉèÖÃÖµ
	int i = 0, j = 0;
	while (tk.Next(sElement))
	{
		sElement.TrimLeft();
		sElement.TrimRight();
		double v = atof((const char*) sElement.GetBuffer());
		SetElement(i, j++, v);

		if (j == m_nNumColumns)
		{
			j = 0;
			i++;
			if (i == m_nNumRows)
				break;
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// ½«¾ØÕó¸÷ÔªËØµÄÖµ×ª»¯Îª×Ö·û´®
//
// ²ÎÊý£º
// 1. const CString& sDelim - Êý×ÖÖ®¼äµÄ·Ö¸ô·û£¬Ä¬ÈÏÎª¿Õ¸ñ
// 2 BOOL bLineBreak - ÐÐÓëÐÐÖ®¼äÊÇ·ñÓÐ»Ø³µ»»ÐÐ·û£¬Ä¬ÈÏÎªÕæ(ÓÐ»»ÐÐ·û)
//
// ·µ»ØÖµ£ºCString ÐÍ£¬×ª»»µÃµ½µÄ×Ö·û´®
//////////////////////////////////////////////////////////////////////
CString CMatrix::ToString(const CString& sDelim /*= " "*/, BOOL bLineBreak /*= TRUE*/) const
{
	CString s= _T("");

	for (int i=0; i<m_nNumRows; ++i)
	{
		for (int j=0; j<m_nNumColumns; ++j)
		{
			CString ss;
			ss.Format(_T("%f"), GetElement(i, j));
			s += ss;

			if (bLineBreak)
			{
				if (j != m_nNumColumns-1)
					s += sDelim;
			}
			else
			{
				if (i != m_nNumRows-1 || j != m_nNumColumns-1)
					s += sDelim;
			}
		}
		if (bLineBreak)
			if (i != m_nNumRows-1)
				s += "\r\n";
	}

	return s;
}
void CMatrix::Read(string filename)
{
	ifstream file(filename.c_str());
    if(!file)
        return;
	
	string s;
    CString sss = _T("");
	while (getline(file,s)) {
		sss += CString(s.c_str());
	}
	
	file.close();
	FromString(sss);
}
void CMatrix::Write(string filename)
{
	ofstream file(filename.c_str());
    if(!file)
        return;

    CString s = ToString();
	file << s.GetBuffer(s.GetLength());

	file.close();
}

//////////////////////////////////////////////////////////////////////
// ½«¾ØÕóÖ¸¶¨ÐÐÖÐ¸÷ÔªËØµÄÖµ×ª»¯Îª×Ö·û´®
//
// ²ÎÊý£º
// 1. int nRow - Ö¸¶¨µÄ¾ØÕóÐÐ£¬nRow = 0±íÊ¾µÚÒ»ÐÐ
// 2. const CString& sDelim - Êý×ÖÖ®¼äµÄ·Ö¸ô·û£¬Ä¬ÈÏÎª¿Õ¸ñ
//
// ·µ»ØÖµ£ºCString ÐÍ£¬×ª»»µÃµ½µÄ×Ö·û´®
//////////////////////////////////////////////////////////////////////
CString CMatrix::RowToString(int nRow, const CString& sDelim /*= " "*/) const
{
	CString s = _T("");

	if (nRow >= m_nNumRows)
		return s;

	for (int j=0; j<m_nNumColumns; ++j)
	{
		CString ss;
		ss.Format(_T("%f"), GetElement(nRow, j));
		s += ss;
		if (j != m_nNumColumns-1)
			s += sDelim;
	}

	return s;
}

//////////////////////////////////////////////////////////////////////
// ½«¾ØÕóÖ¸¶¨ÁÐÖÐ¸÷ÔªËØµÄÖµ×ª»¯Îª×Ö·û´®
//
// ²ÎÊý£º
// 1. int nCol - Ö¸¶¨µÄ¾ØÕóÐÐ£¬nCol = 0±íÊ¾µÚÒ»ÁÐ
// 2. const CString& sDelim - Êý×ÖÖ®¼äµÄ·Ö¸ô·û£¬Ä¬ÈÏÎª¿Õ¸ñ
//
// ·µ»ØÖµ£ºCString ÐÍ£¬×ª»»µÃµ½µÄ×Ö·û´®
//////////////////////////////////////////////////////////////////////
CString CMatrix::ColToString(int nCol, const CString& sDelim /*= " "*/) const
{
	CString s = _T("");

	if (nCol >= m_nNumColumns)
		return s;

	for (int i=0; i<m_nNumRows; ++i)
	{
		CString ss;
		ss.Format(_T("%f"), GetElement(i, nCol));
		s += ss;
		if (i != m_nNumRows-1)
			s += sDelim;
	}

	return s;
}

//////////////////////////////////////////////////////////////////////
// ÉèÖÃ¾ØÕó¸÷ÔªËØµÄÖµ
//
// ²ÎÊý£º
// 1. double value[] - Ò»Î¬Êý×é£¬³¤¶ÈÎªm_nNumColumns*m_nNumRows£¬´æ´¢
//                     ¾ØÕó¸÷ÔªËØµÄÖµ
//
// ·µ»ØÖµ£ºÎÞ
//////////////////////////////////////////////////////////////////////
void CMatrix::SetData(double value[])
{
	// empty the memory
	memset(m_pData, 0, sizeof(double) * m_nNumColumns*m_nNumRows);
	// copy data
	memcpy(m_pData, value, sizeof(double)*m_nNumColumns*m_nNumRows);
}

//////////////////////////////////////////////////////////////////////
// ÉèÖÃÖ¸¶¨ÔªËØµÄÖµ
//
// ²ÎÊý£º
// 1. int nRows - Ö¸¶¨µÄ¾ØÕóÐÐÊý
// 2. int nCols - Ö¸¶¨µÄ¾ØÕóÁÐÊý
// 3. double value - Ö¸¶¨ÔªËØµÄÖµ
//
// ·µ»ØÖµ£ºBOOL ÐÍ£¬ËµÃ÷ÉèÖÃÊÇ·ñ³É¹¦
//////////////////////////////////////////////////////////////////////
BOOL CMatrix::SetElement(int nRow, int nCol, double value)
{
	if (nCol < 0 || nCol >= m_nNumColumns || nRow < 0 || nRow >= m_nNumRows)
		return FALSE;						// array bounds error
	if (m_pData == NULL)
		return FALSE;							// bad pointer error
	
	m_pData[nCol + nRow * m_nNumColumns] = value;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// ÉèÖÃÖ¸¶¨ÔªËØµÄÖµ
//
// ²ÎÊý£º
// 1. int nRows - Ö¸¶¨µÄ¾ØÕóÐÐÊý
// 2. int nCols - Ö¸¶¨µÄ¾ØÕóÁÐÊý
//
// ·µ»ØÖµ£ºdouble ÐÍ£¬Ö¸¶¨ÔªËØµÄÖµ
//////////////////////////////////////////////////////////////////////
double CMatrix::GetElement(int nRow, int nCol) const
{
	ASSERT(nCol >= 0 && nCol < m_nNumColumns && nRow >= 0 && nRow < m_nNumRows); // array bounds error
	ASSERT(m_pData);							// bad pointer error
	return m_pData[nCol + nRow * m_nNumColumns] ;
}

//////////////////////////////////////////////////////////////////////
// »ñÈ¡¾ØÕóµÄÁÐÊý
//
// ²ÎÊý£ºÎÞ
//
// ·µ»ØÖµ£ºint ÐÍ£¬¾ØÕóµÄÁÐÊý
//////////////////////////////////////////////////////////////////////
int	CMatrix::GetNumColumns() const
{
	return m_nNumColumns;
}

//////////////////////////////////////////////////////////////////////
// »ñÈ¡¾ØÕóµÄÐÐÊý
//
// ²ÎÊý£ºÎÞ
//
// ·µ»ØÖµ£ºint ÐÍ£¬¾ØÕóµÄÐÐÊý
//////////////////////////////////////////////////////////////////////
int	CMatrix::GetNumRows() const
{
	return m_nNumRows;
}

//////////////////////////////////////////////////////////////////////
// »ñÈ¡¾ØÕóµÄÊý¾Ý
//
// ²ÎÊý£ºÎÞ
//
// ·µ»ØÖµ£ºdoubleÐÍÖ¸Õë£¬Ö¸Ïò¾ØÕó¸÷ÔªËØµÄÊý¾Ý»º³åÇø
//////////////////////////////////////////////////////////////////////
double* CMatrix::GetData() const
{
	return m_pData;
}

//////////////////////////////////////////////////////////////////////
// »ñÈ¡Ö¸¶¨ÐÐµÄÏòÁ¿
//
// ²ÎÊý£º
// 1. int nRows - Ö¸¶¨µÄ¾ØÕóÐÐÊý
// 2.  double* pVector - Ö¸ÏòÏòÁ¿ÖÐ¸÷ÔªËØµÄ»º³åÇø
//
// ·µ»ØÖµ£ºint ÐÍ£¬ÏòÁ¿ÖÐÔªËØµÄ¸öÊý£¬¼´¾ØÕóµÄÁÐÊý
//////////////////////////////////////////////////////////////////////
int CMatrix::GetRowVector(int nRow, double* pVector) const
{
	if (pVector == NULL)
		delete pVector;

	pVector = new double[m_nNumColumns];
	ASSERT(pVector != NULL);

	for (int j=0; j<m_nNumColumns; ++j)
		pVector[j] = GetElement(nRow, j);

	return m_nNumColumns;
}

//////////////////////////////////////////////////////////////////////
// »ñÈ¡Ö¸¶¨ÁÐµÄÏòÁ¿
//
// ²ÎÊý£º
// 1. int nCols - Ö¸¶¨µÄ¾ØÕóÁÐÊý
// 2.  double* pVector - Ö¸ÏòÏòÁ¿ÖÐ¸÷ÔªËØµÄ»º³åÇø
//
// ·µ»ØÖµ£ºint ÐÍ£¬ÏòÁ¿ÖÐÔªËØµÄ¸öÊý£¬¼´¾ØÕóµÄÐÐÊý
//////////////////////////////////////////////////////////////////////
int CMatrix::GetColVector(int nCol, double* pVector) const
{
	if (pVector == NULL)
		delete pVector;

	pVector = new double[m_nNumRows];
	ASSERT(pVector != NULL);

	for (int i=0; i<m_nNumRows; ++i)
		pVector[i] = GetElement(i, nCol);

	return m_nNumRows;
}

//////////////////////////////////////////////////////////////////////
// ÖØÔØÔËËã·û=£¬¸ø¾ØÕó¸³Öµ
//
// ²ÎÊý£º
// 1. const CMatrix& other - ÓÃÓÚ¸ø¾ØÕó¸³ÖµµÄÔ´¾ØÕó
//
// ·µ»ØÖµ£ºCMatrixÐÍµÄÒýÓÃ£¬ËùÒýÓÃµÄ¾ØÕóÓëotherÏàµÈ
//////////////////////////////////////////////////////////////////////
CMatrix& CMatrix::operator=(const CMatrix& other)
{
	if (&other != this)
	{
		BOOL bSuccess = Init(other.GetNumRows(), other.GetNumColumns());
		ASSERT(bSuccess);

		// copy the pointer
		memcpy(m_pData, other.m_pData, sizeof(double)*m_nNumColumns*m_nNumRows);
	}

	// finally return a reference to ourselves
	return *this ;
}

//////////////////////////////////////////////////////////////////////
// ÖØÔØÔËËã·û==£¬ÅÐ¶Ï¾ØÕóÊÇ·ñÏàµÈ
//
// ²ÎÊý£º
// 1. const CMatrix& other - ÓÃÓÚ±È½ÏµÄ¾ØÕó
//
// ·µ»ØÖµ£ºBOOL ÐÍ£¬Á½¸ö¾ØÕóÏàµÈÔòÎªTRUE£¬·ñÔòÎªFALSE
//////////////////////////////////////////////////////////////////////
BOOL CMatrix::operator==(const CMatrix& other) const
{
	// Ê×ÏÈ¼ì²éÐÐÁÐÊýÊÇ·ñÏàµÈ
	if (m_nNumColumns != other.GetNumColumns() || m_nNumRows != other.GetNumRows())
		return FALSE;

	for (int i=0; i<m_nNumRows; ++i)
	{
		for (int j=0; j<m_nNumColumns; ++j)
		{
			if (GetElement(i, j) != other.GetElement(i, j))
				return FALSE;
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// ÖØÔØÔËËã·û!=£¬ÅÐ¶Ï¾ØÕóÊÇ·ñ²»ÏàµÈ
//
// ²ÎÊý£º
// 1. const CMatrix& other - ÓÃÓÚ±È½ÏµÄ¾ØÕó
//
// ·µ»ØÖµ£ºBOOL ÐÍ£¬Á½¸ö²»¾ØÕóÏàµÈÔòÎªTRUE£¬·ñÔòÎªFALSE
//////////////////////////////////////////////////////////////////////
BOOL CMatrix::operator!=(const CMatrix& other) const
{
	return !(*this == other);
}

//////////////////////////////////////////////////////////////////////
// ÖØÔØÔËËã·û+£¬ÊµÏÖ¾ØÕóµÄ¼Ó·¨
//
// ²ÎÊý£º
// 1. const CMatrix& other - ÓëÖ¸¶¨¾ØÕóÏà¼ÓµÄ¾ØÕó
//
// ·µ»ØÖµ£ºCMatrixÐÍ£¬Ö¸¶¨¾ØÕóÓëotherÏà¼ÓÖ®ºÍ
//////////////////////////////////////////////////////////////////////
CMatrix	CMatrix::operator+(const CMatrix& other) const
{
	// Ê×ÏÈ¼ì²éÐÐÁÐÊýÊÇ·ñÏàµÈ
	ASSERT (m_nNumColumns == other.GetNumColumns() && m_nNumRows == other.GetNumRows());

	// ¹¹Ôì½á¹û¾ØÕó
	CMatrix	result(*this) ;		// ¿½±´¹¹Ôì
	// ¾ØÕó¼Ó·¨
	for (int i = 0 ; i < m_nNumRows ; ++i)
	{
		for (int j = 0 ; j <  m_nNumColumns; ++j)
			result.SetElement(i, j, result.GetElement(i, j) + other.GetElement(i, j)) ;
	}

	return result ;
}

//////////////////////////////////////////////////////////////////////
// ÖØÔØÔËËã·û-£¬ÊµÏÖ¾ØÕóµÄ¼õ·¨
//
// ²ÎÊý£º
// 1. const CMatrix& other - ÓëÖ¸¶¨¾ØÕóÏà¼õµÄ¾ØÕó
//
// ·µ»ØÖµ£ºCMatrixÐÍ£¬Ö¸¶¨¾ØÕóÓëotherÏà¼õÖ®²î
//////////////////////////////////////////////////////////////////////
CMatrix	CMatrix::operator-(const CMatrix& other) const
{
	// Ê×ÏÈ¼ì²éÐÐÁÐÊýÊÇ·ñÏàµÈ
	ASSERT (m_nNumColumns == other.GetNumColumns() && m_nNumRows == other.GetNumRows());

	// ¹¹ÔìÄ¿±ê¾ØÕó
	CMatrix	result(*this) ;		// copy ourselves
	// ½øÐÐ¼õ·¨²Ù×÷
	for (int i = 0 ; i < m_nNumRows ; ++i)
	{
		for (int j = 0 ; j <  m_nNumColumns; ++j)
			result.SetElement(i, j, result.GetElement(i, j) - other.GetElement(i, j)) ;
	}

	return result ;
}

//////////////////////////////////////////////////////////////////////
// ÖØÔØÔËËã·û*£¬ÊµÏÖ¾ØÕóµÄÊý³Ë
//
// ²ÎÊý£º
// 1. double value - ÓëÖ¸¶¨¾ØÕóÏà³ËµÄÊµÊý
//
// ·µ»ØÖµ£ºCMatrixÐÍ£¬Ö¸¶¨¾ØÕóÓëvalueÏà³ËÖ®»ý
//////////////////////////////////////////////////////////////////////
CMatrix	CMatrix::operator*(double value) const
{
	// ¹¹ÔìÄ¿±ê¾ØÕó
	CMatrix	result(*this) ;		// copy ourselves
	// ½øÐÐÊý³Ë
	for (int i = 0 ; i < m_nNumRows ; ++i)
	{
		for (int j = 0 ; j <  m_nNumColumns; ++j)
			result.SetElement(i, j, result.GetElement(i, j) * value) ;
	}

	return result ;
}

//////////////////////////////////////////////////////////////////////
// ÖØÔØÔËËã·û*£¬ÊµÏÖ¾ØÕóµÄ³Ë·¨
//
// ²ÎÊý£º
// 1. const CMatrix& other - ÓëÖ¸¶¨¾ØÕóÏà³ËµÄ¾ØÕó
//
// ·µ»ØÖµ£ºCMatrixÐÍ£¬Ö¸¶¨¾ØÕóÓëotherÏà³ËÖ®»ý
//////////////////////////////////////////////////////////////////////
CMatrix	CMatrix::operator*(const CMatrix& other) const
{
	// Ê×ÏÈ¼ì²éÐÐÁÐÊýÊÇ·ñ·ûºÏÒªÇó
	ASSERT (m_nNumColumns == other.GetNumRows());

	// construct the object we are going to return
	CMatrix	result(m_nNumRows, other.GetNumColumns()) ;

	// ¾ØÕó³Ë·¨£¬¼´
	//
	// [A][B][C]   [G][H]     [A*G + B*I + C*K][A*H + B*J + C*L]
	// [D][E][F] * [I][J] =   [D*G + E*I + F*K][D*H + E*J + F*L]
	//             [K][L]
	//
	double	value ;
	for (int i = 0 ; i < result.GetNumRows() ; ++i)
	{
		for (int j = 0 ; j < other.GetNumColumns() ; ++j)
		{
			value = 0.0 ;
			for (int k = 0 ; k < m_nNumColumns ; ++k)
			{
				value += GetElement(i, k) * other.GetElement(k, j) ;
			}

			result.SetElement(i, j, value) ;
		}
//		fprintf(stdout,"Multiply Step %d\n",i);
	}

	return result ;
}

//////////////////////////////////////////////////////////////////////
// ¸´¾ØÕóµÄ³Ë·¨
//
// ²ÎÊý£º
// 1. const CMatrix& AR - ×ó±ß¸´¾ØÕóµÄÊµ²¿¾ØÕó
// 2. const CMatrix& AI - ×ó±ß¸´¾ØÕóµÄÐé²¿¾ØÕó
// 3. const CMatrix& BR - ÓÒ±ß¸´¾ØÕóµÄÊµ²¿¾ØÕó
// 4. const CMatrix& BI - ÓÒ±ß¸´¾ØÕóµÄÐé²¿¾ØÕó
// 5. CMatrix& CR - ³Ë»ý¸´¾ØÕóµÄÊµ²¿¾ØÕó
// 6. CMatrix& CI - ³Ë»ý¸´¾ØÕóµÄÐé²¿¾ØÕó
//
// ·µ»ØÖµ£ºBOOLÐÍ£¬¸´¾ØÕó³Ë·¨ÊÇ·ñ³É¹¦
//////////////////////////////////////////////////////////////////////
BOOL CMatrix::CMul(const CMatrix& AR, const CMatrix& AI, const CMatrix& BR, const CMatrix& BI, CMatrix& CR, CMatrix& CI) const
{
	// Ê×ÏÈ¼ì²éÐÐÁÐÊýÊÇ·ñ·ûºÏÒªÇó
	if (AR.GetNumColumns() != AI.GetNumColumns() ||
		AR.GetNumRows() != AI.GetNumRows() ||
		BR.GetNumColumns() != BI.GetNumColumns() ||
		BR.GetNumRows() != BI.GetNumRows() ||
		AR.GetNumColumns() != BR.GetNumRows())
		return FALSE;

	// ¹¹Ôì³Ë»ý¾ØÕóÊµ²¿¾ØÕóºÍÐé²¿¾ØÕó
	CMatrix mtxCR(AR.GetNumRows(), BR.GetNumColumns()), mtxCI(AR.GetNumRows(), BR.GetNumColumns());
	// ¸´¾ØÕóÏà³Ë
    for (int i=0; i<AR.GetNumRows(); ++i)
	{
	    for (int j=0; j<BR.GetNumColumns(); ++j)
		{
			double vr = 0;
			double vi = 0;
            for (int k =0; k<AR.GetNumColumns(); ++k)
			{
                double p = AR.GetElement(i, k) * BR.GetElement(k, j);
                double q = AI.GetElement(i, k) * BI.GetElement(k, j);
                double s = (AR.GetElement(i, k) + AI.GetElement(i, k)) * (BR.GetElement(k, j) + BI.GetElement(k, j));
                vr += p - q;
                vi += s - p - q;
			}
            mtxCR.SetElement(i, j, vr);
            mtxCI.SetElement(i, j, vi);
        }
	}

	CR = mtxCR;
	CI = mtxCI;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// ¾ØÕóµÄ×ªÖÃ
//
// ²ÎÊý£ºÎÞ
//
// ·µ»ØÖµ£ºCMatrixÐÍ£¬Ö¸¶¨¾ØÕó×ªÖÃ¾ØÕó
//////////////////////////////////////////////////////////////////////
CMatrix CMatrix::Transpose() const
{
	// ¹¹ÔìÄ¿±ê¾ØÕó
	CMatrix	Trans(m_nNumColumns, m_nNumRows);

	// ×ªÖÃ¸÷ÔªËØ
	for (int i = 0 ; i < m_nNumRows ; ++i)
	{
		for (int j = 0 ; j < m_nNumColumns ; ++j)
			Trans.SetElement(j, i, GetElement(i, j)) ;
	}

	return Trans;
}

//////////////////////////////////////////////////////////////////////
// Êµ¾ØÕóÇóÄæµÄÈ«Ñ¡Ö÷Ôª¸ßË¹£­Ô¼µ±·¨
//
// ²ÎÊý£ºÎÞ
//
// ·µ»ØÖµ£ºBOOLÐÍ£¬ÇóÄæÊÇ·ñ³É¹¦
//////////////////////////////////////////////////////////////////////
BOOL CMatrix::InvertGaussJordan()
{
	int *pnRow, *pnCol,i,j,k,l,u,v;
    double d = 0, p = 0;

	// ·ÖÅäÄÚ´æ
    pnRow = new int[m_nNumColumns];
    pnCol = new int[m_nNumColumns];
	if (pnRow == NULL || pnCol == NULL)
		return FALSE;

	// ÏûÔª
    for (k=0; k<=m_nNumColumns-1; k++)
    { 
		d=0.0;
        for (i=k; i<=m_nNumColumns-1; i++)
		{
			for (j=k; j<=m_nNumColumns-1; j++)
			{ 
				l=i*m_nNumColumns+j; p=fabs(m_pData[l]);
				if (p>d) 
				{ 
					d=p; 
					pnRow[k]=i; 
					pnCol[k]=j;
				}
			}
		}
        
		// Ê§°Ü
		if (d == 0.0)
		{
			delete[] pnRow;
			delete[] pnCol;
			return FALSE;
		}

        if (pnRow[k] != k)
		{
			for (j=0; j<=m_nNumColumns-1; j++)
			{ 
				u=k*m_nNumColumns+j; 
				v=pnRow[k]*m_nNumColumns+j;
				p=m_pData[u]; 
				m_pData[u]=m_pData[v]; 
				m_pData[v]=p;
			}
		}
        
		if (pnCol[k] != k)
		{
			for (i=0; i<=m_nNumColumns-1; i++)
            { 
				u=i*m_nNumColumns+k; 
				v=i*m_nNumColumns+pnCol[k];
				p=m_pData[u]; 
				m_pData[u]=m_pData[v]; 
				m_pData[v]=p;
            }
		}

        l=k*m_nNumColumns+k;
        m_pData[l]=1.0/m_pData[l];
        for (j=0; j<=m_nNumColumns-1; j++)
		{
			if (j != k)
            { 
				u=k*m_nNumColumns+j; 
				m_pData[u]=m_pData[u]*m_pData[l];
			}
		}

        for (i=0; i<=m_nNumColumns-1; i++)
		{
			if (i!=k)
			{
				for (j=0; j<=m_nNumColumns-1; j++)
				{
					if (j!=k)
					{ 
						u=i*m_nNumColumns+j;
						m_pData[u]=m_pData[u]-m_pData[i*m_nNumColumns+k]*m_pData[k*m_nNumColumns+j];
					}
                }
			}
		}

        for (i=0; i<=m_nNumColumns-1; i++)
		{
			if (i!=k)
            { 
				u=i*m_nNumColumns+k; 
				m_pData[u]=-m_pData[u]*m_pData[l];
			}
		}
    }

    // µ÷Õû»Ö¸´ÐÐÁÐ´ÎÐò
    for (k=m_nNumColumns-1; k>=0; k--)
    { 
		if (pnCol[k]!=k)
		{
			for (j=0; j<=m_nNumColumns-1; j++)
            { 
				u=k*m_nNumColumns+j; 
				v=pnCol[k]*m_nNumColumns+j;
				p=m_pData[u]; 
				m_pData[u]=m_pData[v]; 
				m_pData[v]=p;
            }
		}

        if (pnRow[k]!=k)
		{
			for (i=0; i<=m_nNumColumns-1; i++)
            { 
				u=i*m_nNumColumns+k; 
				v=i*m_nNumColumns+pnRow[k];
				p=m_pData[u]; 
				m_pData[u]=m_pData[v]; 
				m_pData[v]=p;
            }
		}
    }

	// ÇåÀíÄÚ´æ
	delete[] pnRow;
	delete[] pnCol;

	// ³É¹¦·µ»Ø
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// ¸´¾ØÕóÇóÄæµÄÈ«Ñ¡Ö÷Ôª¸ßË¹£­Ô¼µ±·¨
//
// ²ÎÊý£º
// 1. CMatrix& mtxImag - ¸´¾ØÕóµÄÐé²¿¾ØÕó£¬µ±Ç°¾ØÕóÎª¸´¾ØÕóµÄÊµ²¿
//
// ·µ»ØÖµ£ºBOOLÐÍ£¬ÇóÄæÊÇ·ñ³É¹¦
//////////////////////////////////////////////////////////////////////
BOOL CMatrix::InvertGaussJordan(CMatrix& mtxImag)
{
	int *pnRow,*pnCol,i,j,k,l,u,v,w;
    double p,q,s,t,d,b;

	// ·ÖÅäÄÚ´æ
    pnRow = new int[m_nNumColumns];
    pnCol = new int[m_nNumColumns];
	if (pnRow == NULL || pnCol == NULL)
		return FALSE;

	// ÏûÔª
    for (k=0; k<=m_nNumColumns-1; k++)
    { 
		d=0.0;
        for (i=k; i<=m_nNumColumns-1; i++)
		{
			for (j=k; j<=m_nNumColumns-1; j++)
			{ 
				u=i*m_nNumColumns+j;
				p=m_pData[u]*m_pData[u]+mtxImag.m_pData[u]*mtxImag.m_pData[u];
				if (p>d) 
				{ 
					d=p; 
					pnRow[k]=i; 
					pnCol[k]=j;
				}
			}
		}

		// Ê§°Ü
        if (d == 0.0)
        { 
			delete[] pnRow;
			delete[] pnCol;
            return(0);
        }

        if (pnRow[k]!=k)
		{
			for (j=0; j<=m_nNumColumns-1; j++)
            { 
				u=k*m_nNumColumns+j; 
				v=pnRow[k]*m_nNumColumns+j;
				t=m_pData[u]; 
				m_pData[u]=m_pData[v]; 
				m_pData[v]=t;
				t=mtxImag.m_pData[u]; 
				mtxImag.m_pData[u]=mtxImag.m_pData[v]; 
				mtxImag.m_pData[v]=t;
            }
		}

        if (pnCol[k]!=k)
		{
			for (i=0; i<=m_nNumColumns-1; i++)
            { 
				u=i*m_nNumColumns+k; 
				v=i*m_nNumColumns+pnCol[k];
				t=m_pData[u]; 
				m_pData[u]=m_pData[v]; 
				m_pData[v]=t;
				t=mtxImag.m_pData[u]; 
				mtxImag.m_pData[u]=mtxImag.m_pData[v]; 
				mtxImag.m_pData[v]=t;
            }
		}

        l=k*m_nNumColumns+k;
        m_pData[l]=m_pData[l]/d; mtxImag.m_pData[l]=-mtxImag.m_pData[l]/d;
        for (j=0; j<=m_nNumColumns-1; j++)
		{
			if (j!=k)
            { 
				u=k*m_nNumColumns+j;
				p=m_pData[u]*m_pData[l]; 
				q=mtxImag.m_pData[u]*mtxImag.m_pData[l];
				s=(m_pData[u]+mtxImag.m_pData[u])*(m_pData[l]+mtxImag.m_pData[l]);
				m_pData[u]=p-q; 
				mtxImag.m_pData[u]=s-p-q;
            }
		}

        for (i=0; i<=m_nNumColumns-1; i++)
		{
			if (i!=k)
            { 
				v=i*m_nNumColumns+k;
				for (j=0; j<=m_nNumColumns-1; j++)
				{
					if (j!=k)
					{ 
						u=k*m_nNumColumns+j;  
						w=i*m_nNumColumns+j;
						p=m_pData[u]*m_pData[v]; 
						q=mtxImag.m_pData[u]*mtxImag.m_pData[v];
						s=(m_pData[u]+mtxImag.m_pData[u])*(m_pData[v]+mtxImag.m_pData[v]);
						t=p-q; 
						b=s-p-q;
						m_pData[w]=m_pData[w]-t;
						mtxImag.m_pData[w]=mtxImag.m_pData[w]-b;
					}
				}
            }
		}

        for (i=0; i<=m_nNumColumns-1; i++)
		{
			if (i!=k)
            { 
				u=i*m_nNumColumns+k;
				p=m_pData[u]*m_pData[l]; 
				q=mtxImag.m_pData[u]*mtxImag.m_pData[l];
				s=(m_pData[u]+mtxImag.m_pData[u])*(m_pData[l]+mtxImag.m_pData[l]);
				m_pData[u]=q-p; 
				mtxImag.m_pData[u]=p+q-s;
            }
		}
    }

    // µ÷Õû»Ö¸´ÐÐÁÐ´ÎÐò
    for (k=m_nNumColumns-1; k>=0; k--)
    { 
		if (pnCol[k]!=k)
		{
			for (j=0; j<=m_nNumColumns-1; j++)
            { 
				u=k*m_nNumColumns+j; 
				v=pnCol[k]*m_nNumColumns+j;
				t=m_pData[u]; 
				m_pData[u]=m_pData[v]; 
				m_pData[v]=t;
				t=mtxImag.m_pData[u]; 
				mtxImag.m_pData[u]=mtxImag.m_pData[v]; 
				mtxImag.m_pData[v]=t;
            }
		}

        if (pnRow[k]!=k)
		{
			for (i=0; i<=m_nNumColumns-1; i++)
            { 
				u=i*m_nNumColumns+k; 
				v=i*m_nNumColumns+pnRow[k];
				t=m_pData[u]; 
				m_pData[u]=m_pData[v]; 
				m_pData[v]=t;
				t=mtxImag.m_pData[u]; 
				mtxImag.m_pData[u]=mtxImag.m_pData[v]; 
				mtxImag.m_pData[v]=t;
            }
		}
    }

	// ÇåÀíÄÚ´æ
	delete[] pnRow;
	delete[] pnCol;

	// ³É¹¦·µ»Ø
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// ¶Ô³ÆÕý¶¨¾ØÕóµÄÇóÄæ
//
// ²ÎÊý£ºÎÞ
//
// ·µ»ØÖµ£ºBOOLÐÍ£¬ÇóÄæÊÇ·ñ³É¹¦
//////////////////////////////////////////////////////////////////////
BOOL CMatrix::InvertSsgj()
{ 
	int i, j ,k, m;
    double w, g, *pTmp;

	// ÁÙÊ±ÄÚ´æ
    pTmp = new double[m_nNumColumns];

	// ÖðÁÐ´¦Àí
    for (k=0; k<=m_nNumColumns-1; k++)
    { 
		w=m_pData[0];
        if (w == 0.0)
        { 
			delete[] pTmp;
			return FALSE;
		}

        m=m_nNumColumns-k-1;
        for (i=1; i<=m_nNumColumns-1; i++)
        { 
			g=m_pData[i*m_nNumColumns]; 
			pTmp[i]=g/w;
            if (i<=m) 
				pTmp[i]=-pTmp[i];
            for (j=1; j<=i; j++)
              m_pData[(i-1)*m_nNumColumns+j-1]=m_pData[i*m_nNumColumns+j]+g*pTmp[j];
        }

        m_pData[m_nNumColumns*m_nNumColumns-1]=1.0/w;
        for (i=1; i<=m_nNumColumns-1; i++)
			m_pData[(m_nNumColumns-1)*m_nNumColumns+i-1]=pTmp[i];
    }

	// ÐÐÁÐµ÷Õû
    for (i=0; i<=m_nNumColumns-2; i++)
		for (j=i+1; j<=m_nNumColumns-1; j++)
			m_pData[i*m_nNumColumns+j]=m_pData[j*m_nNumColumns+i];

	// ÁÙÊ±ÄÚ´æÇåÀí
	delete[] pTmp;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// ÍÐ²®Àû×È¾ØÕóÇóÄæµÄ°£À¼ÌØ·½·¨
//
// ²ÎÊý£ºÎÞ
//
// ·µ»ØÖµ£ºBOOLÐÍ£¬ÇóÄæÊÇ·ñ³É¹¦
//////////////////////////////////////////////////////////////////////
BOOL CMatrix::InvertTrench()
{ 
	int i,j,k;
    double a,s,*t,*tt,*c,*r,*p;

	// ÉÏÈý½ÇÔªËØ
	t = new double[m_nNumColumns];
	// ÏÂÈý½ÇÔªËØ
	tt = new double[m_nNumColumns];

	// ÉÏ¡¢ÏÂÈý½ÇÔªËØ¸³Öµ
	for (i=0; i<m_nNumColumns; ++i)
	{
		t[i] = GetElement(0, i);
	    tt[i] = GetElement(i, 0);
	}

	// ÁÙÊ±»º³åÇø
	c = new double[m_nNumColumns];
	r = new double[m_nNumColumns];
	p = new double[m_nNumColumns];

	// ·ÇToeplitz¾ØÕó£¬·µ»Ø
    if (t[0] == 0.0)
    { 
		delete[] t;
		delete[] tt;
		delete[] c;
		delete[] r;
		delete[] p;
		return FALSE;
    }

    a=t[0]; 
	c[0]=tt[1]/t[0]; 
	r[0]=t[1]/t[0];

    for (k=0; k<=m_nNumColumns-3; k++)
    { 
		s=0.0;
        for (j=1; j<=k+1; j++)
			s=s+c[k+1-j]*tt[j];

        s=(s-tt[k+2])/a;
		for (i=0; i<=k; i++)
			p[i]=c[i]+s*r[k-i];

        c[k+1]=-s;
        s=0.0;
        for (j=1; j<=k+1; j++)
          s=s+r[k+1-j]*t[j];
        
		s=(s-t[k+2])/a;
        for (i=0; i<=k; i++)
        { 
			r[i]=r[i]+s*c[k-i];
            c[k-i]=p[k-i];
        }

        r[k+1]=-s;
		a=0.0;
        for (j=1; j<=k+2; j++)
          a=a+t[j]*c[j-1];

        a=t[0]-a;

		// Çó½âÊ§°Ü
        if (a == 0.0)
		{ 
			delete[] t;
			delete[] tt;
			delete[] c;
			delete[] r;
			delete[] p;
			return FALSE;
		}
    }

    m_pData[0]=1.0/a;
    for (i=0; i<=m_nNumColumns-2; i++)
    { 
		k=i+1; 
		j=(i+1)*m_nNumColumns;
        m_pData[k]=-r[i]/a; 
		m_pData[j]=-c[i]/a;
    }

   for (i=0; i<=m_nNumColumns-2; i++)
	{
		for (j=0; j<=m_nNumColumns-2; j++)
		{ 
			k=(i+1)*m_nNumColumns+j+1;
			m_pData[k]=m_pData[i*m_nNumColumns+j]-c[i]*m_pData[j+1];
			m_pData[k]=m_pData[k]+c[m_nNumColumns-j-2]*m_pData[m_nNumColumns-i-1];
		}
	}

    // ÁÙÊ±ÄÚ´æÇåÀí
	delete[] t;
	delete[] tt;
	delete[] c;
	delete[] r;
	delete[] p;

	return TRUE;
}
                                               
//////////////////////////////////////////////////////////////////////
// ÇóÐÐÁÐÊ½ÖµµÄÈ«Ñ¡Ö÷Ôª¸ßË¹ÏûÈ¥·¨
//
// ²ÎÊý£ºÎÞ
//
// ·µ»ØÖµ£ºdoubleÐÍ£¬ÐÐÁÐÊ½µÄÖµ
//////////////////////////////////////////////////////////////////////
double CMatrix::DetGauss()
{ 
	int i,j,k,is,js,l,u,v;
    double f,det,q,d;
    
	// ³õÖµ
	f=1.0; 
	det=1.0;
    
	// ÏûÔª
	for (k=0; k<=m_nNumColumns-2; k++)
    { 
		q=0.0;
        for (i=k; i<=m_nNumColumns-1; i++)
		{
			for (j=k; j<=m_nNumColumns-1; j++)
			{ 
				l=i*m_nNumColumns+j; 
				d=fabs(m_pData[l]);
				if (d>q) 
				{ 
					q=d; 
					is=i; 
					js=j;
				}
			}
		}

        if (q == 0.0)
        { 
			det=0.0; 
			return(det);
		}
        
		if (is!=k)
        { 
			f=-f;
            for (j=k; j<=m_nNumColumns-1; j++)
            { 
				u=k*m_nNumColumns+j; 
				v=is*m_nNumColumns+j;
                d=m_pData[u]; 
				m_pData[u]=m_pData[v]; 
				m_pData[v]=d;
            }
        }
        
		if (js!=k)
        { 
			f=-f;
            for (i=k; i<=m_nNumColumns-1; i++)
            {
				u=i*m_nNumColumns+js; 
				v=i*m_nNumColumns+k;
                d=m_pData[u]; 
				m_pData[u]=m_pData[v]; 
				m_pData[v]=d;
            }
        }

        l=k*m_nNumColumns+k;
        det=det*m_pData[l];
        for (i=k+1; i<=m_nNumColumns-1; i++)
        { 
			d=m_pData[i*m_nNumColumns+k]/m_pData[l];
            for (j=k+1; j<=m_nNumColumns-1; j++)
            { 
				u=i*m_nNumColumns+j;
                m_pData[u]=m_pData[u]-d*m_pData[k*m_nNumColumns+j];
            }
        }
    }
    
	// ÇóÖµ
	det=f*det*m_pData[m_nNumColumns*m_nNumColumns-1];

    return(det);
}

//////////////////////////////////////////////////////////////////////
// Çó¾ØÕóÖÈµÄÈ«Ñ¡Ö÷Ôª¸ßË¹ÏûÈ¥·¨
//
// ²ÎÊý£ºÎÞ
//
// ·µ»ØÖµ£ºintÐÍ£¬¾ØÕóµÄÖÈ
//////////////////////////////////////////////////////////////////////
int CMatrix::RankGauss()
{ 
	int i,j,k,nn,is,js,l,ll,u,v;
    double q,d;
    
	// ÖÈÐ¡ÓÚµÈÓÚÐÐÁÐÊý
	nn = m_nNumRows;
    if (m_nNumRows >= m_nNumColumns) 
		nn = m_nNumColumns;

    k=0;

	// ÏûÔªÇó½â
    for (l=0; l<=nn-1; l++)
    { 
		q=0.0;
        for (i=l; i<=m_nNumRows-1; i++)
		{
			for (j=l; j<=m_nNumColumns-1; j++)
			{ 
				ll=i*m_nNumColumns+j; 
				d=fabs(m_pData[ll]);
				if (d>q) 
				{ 
					q=d; 
					is=i; 
					js=j;
				}
			}
		}

        if (q == 0.0) 
			return(k);

        k=k+1;
        if (is!=l)
        { 
			for (j=l; j<=m_nNumColumns-1; j++)
            { 
				u=l*m_nNumColumns+j; 
				v=is*m_nNumColumns+j;
                d=m_pData[u]; 
				m_pData[u]=m_pData[v]; 
				m_pData[v]=d;
            }
        }
        if (js!=l)
        { 
			for (i=l; i<=m_nNumRows-1; i++)
            { 
				u=i*m_nNumColumns+js; 
				v=i*m_nNumColumns+l;
                d=m_pData[u]; 
				m_pData[u]=m_pData[v]; 
				m_pData[v]=d;
            }
        }
        
		ll=l*m_nNumColumns+l;
        for (i=l+1; i<=m_nNumColumns-1; i++)
        { 
			d=m_pData[i*m_nNumColumns+l]/m_pData[ll];
            for (j=l+1; j<=m_nNumColumns-1; j++)
            { 
				u=i*m_nNumColumns+j;
                m_pData[u]=m_pData[u]-d*m_pData[l*m_nNumColumns+j];
            }
        }
    }
    
	return(k);
}

//////////////////////////////////////////////////////////////////////
// ¶Ô³ÆÕý¶¨¾ØÕóµÄÇÇÀïË¹»ù·Ö½âÓëÐÐÁÐÊ½µÄÇóÖµ
//
// ²ÎÊý£º
// 1. double* dblDet - ·µ»ØÐÐÁÐÊ½µÄÖµ
//
// ·µ»ØÖµ£ºBOOLÐÍ£¬Çó½âÊÇ·ñ³É¹¦
//////////////////////////////////////////////////////////////////////
BOOL CMatrix::DetCholesky(double* dblDet)
{ 
	int i,j,k,u,l;
    double d;
    
	// ²»Âú×ãÇó½âÒªÇó
	if (m_pData[0] <= 0.0)
		return FALSE;

	// ÇÇÀïË¹»ù·Ö½â

    m_pData[0]=sqrt(m_pData[0]);
    d=m_pData[0];

    for (i=1; i<=m_nNumColumns-1; i++)
    { 
		u=i*m_nNumColumns; 
		m_pData[u]=m_pData[u]/m_pData[0];
	}
    
	for (j=1; j<=m_nNumColumns-1; j++)
    { 
		l=j*m_nNumColumns+j;
        for (k=0; k<=j-1; k++)
        { 
			u=j*m_nNumColumns+k; 
			m_pData[l]=m_pData[l]-m_pData[u]*m_pData[u];
		}
        
		if (m_pData[l] <= 0.0)
			return FALSE;

        m_pData[l]=sqrt(m_pData[l]);
        d=d*m_pData[l];
        
		for (i=j+1; i<=m_nNumColumns-1; i++)
        { 
			u=i*m_nNumColumns+j;
            for (k=0; k<=j-1; k++)
				m_pData[u]=m_pData[u]-m_pData[i*m_nNumColumns+k]*m_pData[j*m_nNumColumns+k];
            
			m_pData[u]=m_pData[u]/m_pData[l];
        }
    }
    
	// ÐÐÁÐÊ½ÇóÖµ
	*dblDet=d*d;
    
	// ÏÂÈý½Ç¾ØÕó
    for (i=0; i<=m_nNumColumns-2; i++)
		for (j=i+1; j<=m_nNumColumns-1; j++)
			m_pData[i*m_nNumColumns+j]=0.0;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// ¾ØÕóµÄÈý½Ç·Ö½â£¬·Ö½â³É¹¦ºó£¬Ô­¾ØÕó½«³ÉÎªQ¾ØÕó
//
// ²ÎÊý£º
// 1. CMatrix& mtxL - ·µ»Ø·Ö½âºóµÄL¾ØÕó
// 2. CMatrix& mtxU - ·µ»Ø·Ö½âºóµÄU¾ØÕó
//
// ·µ»ØÖµ£ºBOOLÐÍ£¬Çó½âÊÇ·ñ³É¹¦
//////////////////////////////////////////////////////////////////////
BOOL CMatrix::SplitLU(CMatrix& mtxL, CMatrix& mtxU)
{ 
	int i,j,k,w,v,ll;
    
	// ³õÊ¼»¯½á¹û¾ØÕó
	if (! mtxL.Init(m_nNumColumns, m_nNumColumns) ||
		! mtxU.Init(m_nNumColumns, m_nNumColumns))
		return FALSE;

	for (k=0; k<=m_nNumColumns-2; k++)
    { 
		ll=k*m_nNumColumns+k;
		if (m_pData[ll] == 0.0)
			return FALSE;

        for (i=k+1; i<=m_nNumColumns-1; i++)
		{ 
			w=i*m_nNumColumns+k; 
			m_pData[w]=m_pData[w]/m_pData[ll];
		}

        for (i=k+1; i<=m_nNumColumns-1; i++)
        { 
			w=i*m_nNumColumns+k;
            for (j=k+1; j<=m_nNumColumns-1; j++)
            { 
				v=i*m_nNumColumns+j;
                m_pData[v]=m_pData[v]-m_pData[w]*m_pData[k*m_nNumColumns+j];
            }
        }
    }
    
	for (i=0; i<=m_nNumColumns-1; i++)
    {
		for (j=0; j<i; j++)
        { 
			w=i*m_nNumColumns+j; 
			mtxL.m_pData[w]=m_pData[w]; 
			mtxU.m_pData[w]=0.0;
		}

        w=i*m_nNumColumns+i;
        mtxL.m_pData[w]=1.0; 
		mtxU.m_pData[w]=m_pData[w];
        
		for (j=i+1; j<=m_nNumColumns-1; j++)
        { 
			w=i*m_nNumColumns+j; 
			mtxL.m_pData[w]=0.0; 
			mtxU.m_pData[w]=m_pData[w];
		}
    }

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Ò»°ãÊµ¾ØÕóµÄQR·Ö½â£¬·Ö½â³É¹¦ºó£¬Ô­¾ØÕó½«³ÉÎªR¾ØÕó
//
// ²ÎÊý£º
// 1. CMatrix& mtxQ - ·µ»Ø·Ö½âºóµÄQ¾ØÕó
//
// ·µ»ØÖµ£ºBOOLÐÍ£¬Çó½âÊÇ·ñ³É¹¦
//////////////////////////////////////////////////////////////////////
BOOL CMatrix::SplitQR(CMatrix& mtxQ)
{ 
	int i,j,k,l,nn,p,jj;
    double u,alpha,w,t;
    
	if (m_nNumRows < m_nNumColumns)
		return FALSE;

	// ³õÊ¼»¯Q¾ØÕó
	if (! mtxQ.Init(m_nNumRows, m_nNumRows))
		return FALSE;

	// ¶Ô½ÇÏßÔªËØµ¥Î»»¯
    for (i=0; i<=m_nNumRows-1; i++)
	{
		for (j=0; j<=m_nNumRows-1; j++)
		{ 
			l=i*m_nNumRows+j; 
			mtxQ.m_pData[l]=0.0;
			if (i==j) 
				mtxQ.m_pData[l]=1.0;
		}
	}

	// ¿ªÊ¼·Ö½â

    nn=m_nNumColumns;
    if (m_nNumRows == m_nNumColumns) 
		nn=m_nNumRows-1;

    for (k=0; k<=nn-1; k++)
    { 
		u=0.0; 
		l=k*m_nNumColumns+k;
        for (i=k; i<=m_nNumRows-1; i++)
        { 
			w=fabs(m_pData[i*m_nNumColumns+k]);
            if (w>u) 
				u=w;
        }
        
		alpha=0.0;
        for (i=k; i<=m_nNumRows-1; i++)
        { 
			t=m_pData[i*m_nNumColumns+k]/u; 
			alpha=alpha+t*t;
		}

        if (m_pData[l]>0.0) 
			u=-u;

        alpha=u*sqrt(alpha);
        if (alpha == 0.0)
			return FALSE;

        u=sqrt(2.0*alpha*(alpha-m_pData[l]));
        if ((u+1.0)!=1.0)
        { 
			m_pData[l]=(m_pData[l]-alpha)/u;
            for (i=k+1; i<=m_nNumRows-1; i++)
            { 
				p=i*m_nNumColumns+k; 
				m_pData[p]=m_pData[p]/u;
			}
            
			for (j=0; j<=m_nNumRows-1; j++)
            { 
				t=0.0;
                for (jj=k; jj<=m_nNumRows-1; jj++)
					t=t+m_pData[jj*m_nNumColumns+k]*mtxQ.m_pData[jj*m_nNumRows+j];

                for (i=k; i<=m_nNumRows-1; i++)
                { 
					p=i*m_nNumRows+j; 
					mtxQ.m_pData[p]=mtxQ.m_pData[p]-2.0*t*m_pData[i*m_nNumColumns+k];
				}
            }
            
			for (j=k+1; j<=m_nNumColumns-1; j++)
            { 
				t=0.0;
                
				for (jj=k; jj<=m_nNumRows-1; jj++)
					t=t+m_pData[jj*m_nNumColumns+k]*m_pData[jj*m_nNumColumns+j];
                
				for (i=k; i<=m_nNumRows-1; i++)
                { 
					p=i*m_nNumColumns+j; 
					m_pData[p]=m_pData[p]-2.0*t*m_pData[i*m_nNumColumns+k];
				}
            }
            
			m_pData[l]=alpha;
            for (i=k+1; i<=m_nNumRows-1; i++)
				m_pData[i*m_nNumColumns+k]=0.0;
        }
    }
    
	// µ÷ÕûÔªËØ
	for (i=0; i<=m_nNumRows-2; i++)
	{
		for (j=i+1; j<=m_nNumRows-1;j++)
		{ 
			p=i*m_nNumRows+j; 
			l=j*m_nNumRows+i;
			t=mtxQ.m_pData[p]; 
			mtxQ.m_pData[p]=mtxQ.m_pData[l]; 
			mtxQ.m_pData[l]=t;
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Ò»°ãÊµ¾ØÕóµÄÆæÒìÖµ·Ö½â£¬·Ö½â³É¹¦ºó£¬Ô­¾ØÕó¶Ô½ÇÏßÔªËØ¾ÍÊÇ¾ØÕóµÄÆæÒìÖµ
//
// ²ÎÊý£º
// 1. CMatrix& mtxU - ·µ»Ø·Ö½âºóµÄU¾ØÕó
// 2. CMatrix& mtxV - ·µ»Ø·Ö½âºóµÄV¾ØÕó
// 3. double eps - ¼ÆËã¾«¶È£¬Ä¬ÈÏÖµÎª0.000001
//
// ·µ»ØÖµ£ºBOOLÐÍ£¬Çó½âÊÇ·ñ³É¹¦
//////////////////////////////////////////////////////////////////////
BOOL CMatrix::SplitUV(CMatrix& mtxU, CMatrix& mtxV, double eps /*= 0.000001*/)
{ 
	int i,j,k,l,it,ll,kk,ix,iy,mm,nn,iz,m1,ks;
    double d,dd,t,sm,sm1,em1,sk,ek,b,c,shh,fg[2],cs[2];
    double *s,*e,*w;

	int m = m_nNumRows;
	int n = m_nNumColumns;

	// ³õÊ¼»¯U, V¾ØÕó
	if (! mtxU.Init(m, m) || ! mtxV.Init(n, n))
		return FALSE;

	// ÁÙÊ±»º³åÇø
	int ka = max(m, n) + 1;
    s = new double[ka];
    e = new double[ka];
    w = new double[ka];

	// Ö¸¶¨µü´ú´ÎÊýÎª60
    it=60; 
	k=n;

    if (m-1<n) 
		k=m-1;

    l=m;
    if (n-2<m) 
		l=n-2;
    if (l<0) 
		l=0;

	// Ñ­»·µü´ú¼ÆËã
    ll=k;
    if (l>k) 
		ll=l;
    if (ll>=1)
    { 
		for (kk=1; kk<=ll; kk++)
        { 
			if (kk<=k)
            { 
				d=0.0;
                for (i=kk; i<=m; i++)
                { 
					ix=(i-1)*n+kk-1; 
					d=d+m_pData[ix]*m_pData[ix];
				}

                s[kk-1]=sqrt(d);
                if (s[kk-1]!=0.0)
                { 
					ix=(kk-1)*n+kk-1;
                    if (m_pData[ix]!=0.0)
                    { 
						s[kk-1]=fabs(s[kk-1]);
                        if (m_pData[ix]<0.0) 
							s[kk-1]=-s[kk-1];
                    }
                    
					for (i=kk; i<=m; i++)
                    { 
						iy=(i-1)*n+kk-1;
                        m_pData[iy]=m_pData[iy]/s[kk-1];
                    }
                    
					m_pData[ix]=1.0+m_pData[ix];
                }
                
				s[kk-1]=-s[kk-1];
            }
            
			if (n>=kk+1)
            { 
				for (j=kk+1; j<=n; j++)
                { 
					if ((kk<=k)&&(s[kk-1]!=0.0))
                    { 
						d=0.0;
                        for (i=kk; i<=m; i++)
                        { 
							ix=(i-1)*n+kk-1;
                            iy=(i-1)*n+j-1;
                            d=d+m_pData[ix]*m_pData[iy];
                        }
                        
						d=-d/m_pData[(kk-1)*n+kk-1];
                        for (i=kk; i<=m; i++)
                        { 
							ix=(i-1)*n+j-1;
                            iy=(i-1)*n+kk-1;
                            m_pData[ix]=m_pData[ix]+d*m_pData[iy];
                        }
                    }
                    
					e[j-1]=m_pData[(kk-1)*n+j-1];
                }
            }
            
			if (kk<=k)
            { 
				for (i=kk; i<=m; i++)
                { 
					ix=(i-1)*m+kk-1; 
					iy=(i-1)*n+kk-1;
                    mtxU.m_pData[ix]=m_pData[iy];
                }
            }
            
			if (kk<=l)
            { 
				d=0.0;
                for (i=kk+1; i<=n; i++)
					d=d+e[i-1]*e[i-1];
                
				e[kk-1]=sqrt(d);
                if (e[kk-1]!=0.0)
                { 
					if (e[kk]!=0.0)
                    { 
						e[kk-1]=fabs(e[kk-1]);
                        if (e[kk]<0.0) 
							e[kk-1]=-e[kk-1];
                    }

                    for (i=kk+1; i<=n; i++)
                      e[i-1]=e[i-1]/e[kk-1];
                    
					e[kk]=1.0+e[kk];
                }
                
				e[kk-1]=-e[kk-1];
                if ((kk+1<=m)&&(e[kk-1]!=0.0))
                { 
					for (i=kk+1; i<=m; i++) 
						w[i-1]=0.0;
                    
					for (j=kk+1; j<=n; j++)
						for (i=kk+1; i<=m; i++)
							w[i-1]=w[i-1]+e[j-1]*m_pData[(i-1)*n+j-1];
                    
					for (j=kk+1; j<=n; j++)
					{
						for (i=kk+1; i<=m; i++)
                        { 
							ix=(i-1)*n+j-1;
							m_pData[ix]=m_pData[ix]-w[i-1]*e[j-1]/e[kk];
                        }
					}
                }
                
				for (i=kk+1; i<=n; i++)
                  mtxV.m_pData[(i-1)*n+kk-1]=e[i-1];
            }
        }
    }
    
	mm=n;
    if (m+1<n) 
		mm=m+1;
    if (k<n) 
		s[k]=m_pData[k*n+k];
    if (m<mm) 
		s[mm-1]=0.0;
    if (l+1<mm) 
		e[l]=m_pData[l*n+mm-1];

    e[mm-1]=0.0;
    nn=m;
    if (m>n) 
		nn=n;
    if (nn>=k+1)
    { 
		for (j=k+1; j<=nn; j++)
        { 
			for (i=1; i<=m; i++)
				mtxU.m_pData[(i-1)*m+j-1]=0.0;
            mtxU.m_pData[(j-1)*m+j-1]=1.0;
        }
    }
    
	if (k>=1)
    { 
		for (ll=1; ll<=k; ll++)
        { 
			kk=k-ll+1; 
			iz=(kk-1)*m+kk-1;
            if (s[kk-1]!=0.0)
            { 
				if (nn>=kk+1)
				{
					for (j=kk+1; j<=nn; j++)
					{ 
						d=0.0;
						for (i=kk; i<=m; i++)
						{ 
							ix=(i-1)*m+kk-1;
							iy=(i-1)*m+j-1;
							d=d+mtxU.m_pData[ix]*mtxU.m_pData[iy]/mtxU.m_pData[iz];
						}

						d=-d;
						for (i=kk; i<=m; i++)
						{ 
							ix=(i-1)*m+j-1;
							iy=(i-1)*m+kk-1;
							mtxU.m_pData[ix]=mtxU.m_pData[ix]+d*mtxU.m_pData[iy];
						}
					}
				}
                  
				for (i=kk; i<=m; i++)
				{ 
					ix=(i-1)*m+kk-1; 
					mtxU.m_pData[ix]=-mtxU.m_pData[ix];
				}

				mtxU.m_pData[iz]=1.0+mtxU.m_pData[iz];
				if (kk-1>=1)
				{
					for (i=1; i<=kk-1; i++)
						mtxU.m_pData[(i-1)*m+kk-1]=0.0;
				}
			}
            else
            { 
				for (i=1; i<=m; i++)
					mtxU.m_pData[(i-1)*m+kk-1]=0.0;
                mtxU.m_pData[(kk-1)*m+kk-1]=1.0;
            }
		}
    }

    for (ll=1; ll<=n; ll++)
    { 
		kk=n-ll+1; 
		iz=kk*n+kk-1;
        
		if ((kk<=l)&&(e[kk-1]!=0.0))
        { 
			for (j=kk+1; j<=n; j++)
            { 
				d=0.0;
                for (i=kk+1; i<=n; i++)
                { 
					ix=(i-1)*n+kk-1; 
					iy=(i-1)*n+j-1;
                    d=d+mtxV.m_pData[ix]*mtxV.m_pData[iy]/mtxV.m_pData[iz];
                }
                
				d=-d;
                for (i=kk+1; i<=n; i++)
                { 
					ix=(i-1)*n+j-1; 
					iy=(i-1)*n+kk-1;
                    mtxV.m_pData[ix]=mtxV.m_pData[ix]+d*mtxV.m_pData[iy];
                }
            }
        }
        
		for (i=1; i<=n; i++)
			mtxV.m_pData[(i-1)*n+kk-1]=0.0;
        
		mtxV.m_pData[iz-n]=1.0;
    }
    
	for (i=1; i<=m; i++)
		for (j=1; j<=n; j++)
			m_pData[(i-1)*n+j-1]=0.0;
    
	m1=mm; 
	it=60;
    while (TRUE)
    { 
		if (mm==0)
        { 
			ppp(m_pData,e,s,mtxV.m_pData,m,n);
			delete s;
			delete e;
			delete w;
            return TRUE;
        }
        if (it==0)
        { 
			ppp(m_pData,e,s,mtxV.m_pData,m,n);
			delete s;
			delete e;
			delete w;
            return FALSE;
        }
        
		kk=mm-1;
		while ((kk!=0)&&(fabs(e[kk-1])!=0.0))
        { 
			d=fabs(s[kk-1])+fabs(s[kk]);
            dd=fabs(e[kk-1]);
            if (dd>eps*d) 
				kk=kk-1;
            else 
				e[kk-1]=0.0;
        }
        
		if (kk==mm-1)
        { 
			kk=kk+1;
            if (s[kk-1]<0.0)
            { 
				s[kk-1]=-s[kk-1];
                for (i=1; i<=n; i++)
                { 
					ix=(i-1)*n+kk-1; 
					mtxV.m_pData[ix]=-mtxV.m_pData[ix];}
				}
				
				while ((kk!=m1)&&(s[kk-1]<s[kk]))
				{ 
					d=s[kk-1]; 
					s[kk-1]=s[kk]; 
					s[kk]=d;
					if (kk<n)
					{
						for (i=1; i<=n; i++)
						{ 
							ix=(i-1)*n+kk-1; 
							iy=(i-1)*n+kk;
							d=mtxV.m_pData[ix]; 
							mtxV.m_pData[ix]=mtxV.m_pData[iy]; 
							mtxV.m_pData[iy]=d;
						}
					}

					if (kk<m)
					{
						for (i=1; i<=m; i++)
						{ 
							ix=(i-1)*m+kk-1; 
							iy=(i-1)*m+kk;
							d=mtxU.m_pData[ix]; 
							mtxU.m_pData[ix]=mtxU.m_pData[iy]; 
							mtxU.m_pData[iy]=d;
						}
					}

					kk=kk+1;
            }
            
			it=60;
            mm=mm-1;
        }
        else
        { 
			ks=mm;
            while ((ks>kk)&&(fabs(s[ks-1])!=0.0))
            { 
				d=0.0;
                if (ks!=mm) 
					d=d+fabs(e[ks-1]);
                if (ks!=kk+1) 
					d=d+fabs(e[ks-2]);
                
				dd=fabs(s[ks-1]);
                if (dd>eps*d) 
					ks=ks-1;
                else 
					s[ks-1]=0.0;
            }
            
			if (ks==kk)
            { 
				kk=kk+1;
                d=fabs(s[mm-1]);
                t=fabs(s[mm-2]);
                if (t>d) 
					d=t;
                
				t=fabs(e[mm-2]);
                if (t>d) 
					d=t;
                
				t=fabs(s[kk-1]);
                if (t>d) 
					d=t;
                
				t=fabs(e[kk-1]);
                if (t>d) 
					d=t;
                
				sm=s[mm-1]/d; 
				sm1=s[mm-2]/d;
                em1=e[mm-2]/d;
                sk=s[kk-1]/d; 
				ek=e[kk-1]/d;
                b=((sm1+sm)*(sm1-sm)+em1*em1)/2.0;
                c=sm*em1; 
				c=c*c; 
				shh=0.0;

                if ((b!=0.0)||(c!=0.0))
                { 
					shh=sqrt(b*b+c);
                    if (b<0.0) 
						shh=-shh;

                    shh=c/(b+shh);
                }
                
				fg[0]=(sk+sm)*(sk-sm)-shh;
                fg[1]=sk*ek;
                for (i=kk; i<=mm-1; i++)
                { 
					sss(fg,cs);
                    if (i!=kk) 
						e[i-2]=fg[0];

                    fg[0]=cs[0]*s[i-1]+cs[1]*e[i-1];
                    e[i-1]=cs[0]*e[i-1]-cs[1]*s[i-1];
                    fg[1]=cs[1]*s[i];
                    s[i]=cs[0]*s[i];

                    if ((cs[0]!=1.0)||(cs[1]!=0.0))
					{
						for (j=1; j<=n; j++)
                        { 
							ix=(j-1)*n+i-1;
							iy=(j-1)*n+i;
							d=cs[0]*mtxV.m_pData[ix]+cs[1]*mtxV.m_pData[iy];
							mtxV.m_pData[iy]=-cs[1]*mtxV.m_pData[ix]+cs[0]*mtxV.m_pData[iy];
							mtxV.m_pData[ix]=d;
                        }
					}

                    sss(fg,cs);
                    s[i-1]=fg[0];
                    fg[0]=cs[0]*e[i-1]+cs[1]*s[i];
                    s[i]=-cs[1]*e[i-1]+cs[0]*s[i];
                    fg[1]=cs[1]*e[i];
                    e[i]=cs[0]*e[i];

                    if (i<m)
					{
						if ((cs[0]!=1.0)||(cs[1]!=0.0))
						{
							for (j=1; j<=m; j++)
							{ 
								ix=(j-1)*m+i-1;
								iy=(j-1)*m+i;
								d=cs[0]*mtxU.m_pData[ix]+cs[1]*mtxU.m_pData[iy];
								mtxU.m_pData[iy]=-cs[1]*mtxU.m_pData[ix]+cs[0]*mtxU.m_pData[iy];
								mtxU.m_pData[ix]=d;
							}
						}
					}
                }
                
				e[mm-2]=fg[0];
                it=it-1;
            }
            else
            { 
				if (ks==mm)
                { 
					kk=kk+1;
                    fg[1]=e[mm-2]; 
					e[mm-2]=0.0;
                    for (ll=kk; ll<=mm-1; ll++)
                    { 
						i=mm+kk-ll-1;
                        fg[0]=s[i-1];
                        sss(fg,cs);
                        s[i-1]=fg[0];
                        if (i!=kk)
                        { 
							fg[1]=-cs[1]*e[i-2];
                            e[i-2]=cs[0]*e[i-2];
                        }
                        
						if ((cs[0]!=1.0)||(cs[1]!=0.0))
						{
							for (j=1; j<=n; j++)
                            { 
								ix=(j-1)*n+i-1;
								iy=(j-1)*n+mm-1;
								d=cs[0]*mtxV.m_pData[ix]+cs[1]*mtxV.m_pData[iy];
								mtxV.m_pData[iy]=-cs[1]*mtxV.m_pData[ix]+cs[0]*mtxV.m_pData[iy];
								mtxV.m_pData[ix]=d;
                            }
						}
                    }
                }
                else
                { 
					kk=ks+1;
                    fg[1]=e[kk-2];
                    e[kk-2]=0.0;
                    for (i=kk; i<=mm; i++)
                    { 
						fg[0]=s[i-1];
                        sss(fg,cs);
                        s[i-1]=fg[0];
                        fg[1]=-cs[1]*e[i-1];
                        e[i-1]=cs[0]*e[i-1];
                        if ((cs[0]!=1.0)||(cs[1]!=0.0))
						{
							for (j=1; j<=m; j++)
                            { 
								ix=(j-1)*m+i-1;
								iy=(j-1)*m+kk-2;
								d=cs[0]*mtxU.m_pData[ix]+cs[1]*mtxU.m_pData[iy];
								mtxU.m_pData[iy]=-cs[1]*mtxU.m_pData[ix]+cs[0]*mtxU.m_pData[iy];
								mtxU.m_pData[ix]=d;
                            }
						}
                    }
                }
            }
        }
    }
    
	delete s;
	delete e;
	delete w;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// ÄÚ²¿º¯Êý£¬ÓÉSplitUVº¯Êýµ÷ÓÃ
//////////////////////////////////////////////////////////////////////
void CMatrix::ppp(double a[], double e[], double s[], double v[], int m, int n)
{ 
	int i,j,p,q;
    double d;

    if (m>=n) 
		i=n;
    else 
		i=m;

    for (j=1; j<=i-1; j++)
    { 
		a[(j-1)*n+j-1]=s[j-1];
        a[(j-1)*n+j]=e[j-1];
    }
    
	a[(i-1)*n+i-1]=s[i-1];
    if (m<n) 
		a[(i-1)*n+i]=e[i-1];
    
	for (i=1; i<=n-1; i++)
	{
		for (j=i+1; j<=n; j++)
		{ 
			p=(i-1)*n+j-1; 
			q=(j-1)*n+i-1;
			d=v[p]; 
			v[p]=v[q]; 
			v[q]=d;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// ÄÚ²¿º¯Êý£¬ÓÉSplitUVº¯Êýµ÷ÓÃ
//////////////////////////////////////////////////////////////////////
void CMatrix::sss(double fg[2], double cs[2])
{ 
	double r,d;
    
	if ((fabs(fg[0])+fabs(fg[1]))==0.0)
    { 
		cs[0]=1.0; 
		cs[1]=0.0; 
		d=0.0;
	}
    else 
    { 
		d=sqrt(fg[0]*fg[0]+fg[1]*fg[1]);
        if (fabs(fg[0])>fabs(fg[1]))
        { 
			d=fabs(d);
            if (fg[0]<0.0) 
				d=-d;
        }
        if (fabs(fg[1])>=fabs(fg[0]))
        { 
			d=fabs(d);
            if (fg[1]<0.0) 
				d=-d;
        }
        
		cs[0]=fg[0]/d; 
		cs[1]=fg[1]/d;
    }
    
	r=1.0;
    if (fabs(fg[0])>fabs(fg[1])) 
		r=cs[1];
    else if (cs[0]!=0.0) 
		r=1.0/cs[0];

    fg[0]=d; 
	fg[1]=r;
}

//////////////////////////////////////////////////////////////////////
// Çó¹ãÒåÄæµÄÆæÒìÖµ·Ö½â·¨£¬·Ö½â³É¹¦ºó£¬Ô­¾ØÕó¶Ô½ÇÏßÔªËØ¾ÍÊÇ¾ØÕóµÄÆæÒìÖµ
//
// ²ÎÊý£º
// 1. CMatrix& mtxAP - ·µ»ØÔ­¾ØÕóµÄ¹ãÒåÄæ¾ØÕó
// 2. CMatrix& mtxU - ·µ»Ø·Ö½âºóµÄU¾ØÕó
// 3. CMatrix& mtxV - ·µ»Ø·Ö½âºóµÄV¾ØÕó
// 4. double eps - ¼ÆËã¾«¶È£¬Ä¬ÈÏÖµÎª0.000001
//
// ·µ»ØÖµ£ºBOOLÐÍ£¬Çó½âÊÇ·ñ³É¹¦
//////////////////////////////////////////////////////////////////////
BOOL CMatrix::GInvertUV(CMatrix& mtxAP, CMatrix& mtxU, CMatrix& mtxV, double eps /*= 0.000001*/)
{ 
	int i,j,k,l,t,p,q,f;

	// µ÷ÓÃÆæÒìÖµ·Ö½â
    if (! SplitUV(mtxU, mtxV, eps))
		return FALSE;

	int m = m_nNumRows;
	int n = m_nNumColumns;

	// ³õÊ¼»¯¹ãÒåÄæ¾ØÕó
	if (! mtxAP.Init(n, m))
		return FALSE;

	// ¼ÆËã¹ãÒåÄæ¾ØÕó

    j=n;
    if (m<n) 
		j=m;
    j=j-1;
    k=0;
    while ((k<=j)&&(m_pData[k*n+k]!=0.0)) 
		k=k+1;

    k=k-1;
    for (i=0; i<=n-1; i++)
	{
		for (j=0; j<=m-1; j++)
		{ 
			t=i*m+j;	
			mtxAP.m_pData[t]=0.0;
			for (l=0; l<=k; l++)
			{ 
				f=l*n+i; 
				p=j*m+l; 
				q=l*n+l;
				mtxAP.m_pData[t]=mtxAP.m_pData[t]+mtxV.m_pData[f]*mtxU.m_pData[p]/m_pData[q];
			}
		}
	}

    return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Ô¼»¯¶Ô³Æ¾ØÕóÎª¶Ô³ÆÈý¶Ô½ÇÕóµÄºÀË¹ºÉ¶ûµÂ±ä»»·¨
//
// ²ÎÊý£º
// 1. CMatrix& mtxQ - ·µ»ØºÀË¹ºÉ¶ûµÂ±ä»»µÄ³Ë»ý¾ØÕóQ
// 2. CMatrix& mtxT - ·µ»ØÇóµÃµÄ¶Ô³ÆÈý¶Ô½ÇÕó
// 3. double dblB[] - Ò»Î¬Êý×é£¬³¤¶ÈÎª¾ØÕóµÄ½×Êý£¬·µ»Ø¶Ô³ÆÈý¶Ô½ÇÕóµÄÖ÷¶Ô½ÇÏßÔªËØ
// 4. double dblC[] - Ò»Î¬Êý×é£¬³¤¶ÈÎª¾ØÕóµÄ½×Êý£¬Ç°n-1¸öÔªËØ·µ»Ø¶Ô³ÆÈý¶Ô½ÇÕóµÄ´Î¶Ô½ÇÏßÔªËØ
//
// ·µ»ØÖµ£ºBOOLÐÍ£¬Çó½âÊÇ·ñ³É¹¦
//////////////////////////////////////////////////////////////////////
BOOL CMatrix::MakeSymTri(CMatrix& mtxQ, CMatrix& mtxT, double dblB[], double dblC[])
{ 
	int i,j,k,u;
    double h,f,g,h2;
    
	// ³õÊ¼»¯¾ØÕóQºÍT
	if (! mtxQ.Init(m_nNumColumns, m_nNumColumns) ||
		! mtxT.Init(m_nNumColumns, m_nNumColumns))
		return FALSE;

	if (dblB == NULL || dblC == NULL)
		return FALSE;

	for (i=0; i<=m_nNumColumns-1; i++)
	{
		for (j=0; j<=m_nNumColumns-1; j++)
		{ 
			u=i*m_nNumColumns+j; 
			mtxQ.m_pData[u]=m_pData[u];
		}
	}

    for (i=m_nNumColumns-1; i>=1; i--)
    { 
		h=0.0;
        if (i>1)
		{
			for (k=0; k<=i-1; k++)
            { 
				u=i*m_nNumColumns+k; 
				h=h+mtxQ.m_pData[u]*mtxQ.m_pData[u];
			}
		}

        if (h == 0.0)
        { 
			dblC[i]=0.0;
            if (i==1) 
				dblC[i]=mtxQ.m_pData[i*m_nNumColumns+i-1];
            dblB[i]=0.0;
        }
        else
        { 
			dblC[i]=sqrt(h);
            u=i*m_nNumColumns+i-1;
            if (mtxQ.m_pData[u]>0.0) 
				dblC[i]=-dblC[i];

            h=h-mtxQ.m_pData[u]*dblC[i];
            mtxQ.m_pData[u]=mtxQ.m_pData[u]-dblC[i];
            f=0.0;
            for (j=0; j<=i-1; j++)
            { 
				mtxQ.m_pData[j*m_nNumColumns+i]=mtxQ.m_pData[i*m_nNumColumns+j]/h;
                g=0.0;
                for (k=0; k<=j; k++)
					g=g+mtxQ.m_pData[j*m_nNumColumns+k]*mtxQ.m_pData[i*m_nNumColumns+k];

				if (j+1<=i-1)
					for (k=j+1; k<=i-1; k++)
						g=g+mtxQ.m_pData[k*m_nNumColumns+j]*mtxQ.m_pData[i*m_nNumColumns+k];

                dblC[j]=g/h;
                f=f+g*mtxQ.m_pData[j*m_nNumColumns+i];
            }
            
			h2=f/(h+h);
            for (j=0; j<=i-1; j++)
            { 
				f=mtxQ.m_pData[i*m_nNumColumns+j];
                g=dblC[j]-h2*f;
                dblC[j]=g;
                for (k=0; k<=j; k++)
                { 
					u=j*m_nNumColumns+k;
                    mtxQ.m_pData[u]=mtxQ.m_pData[u]-f*dblC[k]-g*mtxQ.m_pData[i*m_nNumColumns+k];
                }
            }
            
			dblB[i]=h;
        }
    }
    
	for (i=0; i<=m_nNumColumns-2; i++) 
		dblC[i]=dblC[i+1];
    
	dblC[m_nNumColumns-1]=0.0;
    dblB[0]=0.0;
    for (i=0; i<=m_nNumColumns-1; i++)
    { 
		if ((dblB[i]!=0.0)&&(i-1>=0))
		{
			for (j=0; j<=i-1; j++)
            { 
				g=0.0;
				for (k=0; k<=i-1; k++)
					g=g+mtxQ.m_pData[i*m_nNumColumns+k]*mtxQ.m_pData[k*m_nNumColumns+j];

				for (k=0; k<=i-1; k++)
                { 
					u=k*m_nNumColumns+j;
					mtxQ.m_pData[u]=mtxQ.m_pData[u]-g*mtxQ.m_pData[k*m_nNumColumns+i];
                }
            }
		}

        u=i*m_nNumColumns+i;
        dblB[i]=mtxQ.m_pData[u]; mtxQ.m_pData[u]=1.0;
        if (i-1>=0)
		{
			for (j=0; j<=i-1; j++)
            { 
				mtxQ.m_pData[i*m_nNumColumns+j]=0.0; 
				mtxQ.m_pData[j*m_nNumColumns+i]=0.0;
			}
		}
    }

    // ¹¹Ôì¶Ô³ÆÈý¶Ô½Ç¾ØÕó
    for (i=0; i<m_nNumColumns; ++i)
	{
	    for (j=0; j<m_nNumColumns; ++j)
		{
            mtxT.SetElement(i, j, 0);
            k = i - j;
            if (k == 0) 
	            mtxT.SetElement(i, j, dblB[j]);
			else if (k == 1)
	            mtxT.SetElement(i, j, dblC[j]);
			else if (k == -1)
	            mtxT.SetElement(i, j, dblC[i]);
        }
    }

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Êµ¶Ô³ÆÈý¶Ô½ÇÕóµÄÈ«²¿ÌØÕ÷ÖµÓëÌØÕ÷ÏòÁ¿µÄ¼ÆËã
//
// ²ÎÊý£º
// 1. double dblB[] - Ò»Î¬Êý×é£¬³¤¶ÈÎª¾ØÕóµÄ½×Êý£¬´«Èë¶Ô³ÆÈý¶Ô½ÇÕóµÄÖ÷¶Ô½ÇÏßÔªËØ£»
//    ·µ»ØÊ±´æ·ÅÈ«²¿ÌØÕ÷Öµ¡£
// 2. double dblC[] - Ò»Î¬Êý×é£¬³¤¶ÈÎª¾ØÕóµÄ½×Êý£¬Ç°n-1¸öÔªËØ´«Èë¶Ô³ÆÈý¶Ô½ÇÕóµÄ´Î¶Ô½ÇÏßÔªËØ
// 3. CMatrix& mtxQ - Èç¹û´«Èëµ¥Î»¾ØÕó£¬Ôò·µ»ØÊµ¶Ô³ÆÈý¶Ô½ÇÕóµÄÌØÕ÷ÖµÏòÁ¿¾ØÕó£»
//    Èç¹û´«ÈëMakeSymTriº¯ÊýÇóµÃµÄ¾ØÕóAµÄºÀË¹ºÉ¶ûµÂ±ä»»µÄ³Ë»ý¾ØÕóQ£¬Ôò·µ»Ø¾ØÕóAµÄ
//    ÌØÕ÷ÖµÏòÁ¿¾ØÕó¡£ÆäÖÐµÚiÁÐÎªÓëÊý×édblBÖÐµÚj¸öÌØÕ÷Öµ¶ÔÓ¦µÄÌØÕ÷ÏòÁ¿¡£
// 4. int nMaxIt - µü´ú´ÎÊý£¬Ä¬ÈÏÖµÎª60
// 5. double eps - ¼ÆËã¾«¶È£¬Ä¬ÈÏÖµÎª0.000001
//
// ·µ»ØÖµ£ºBOOLÐÍ£¬Çó½âÊÇ·ñ³É¹¦
//////////////////////////////////////////////////////////////////////
BOOL CMatrix::SymTriEigenv(double dblB[], double dblC[], CMatrix& mtxQ, int nMaxIt /*= 60*/, double eps /*= 0.000001*/)
{
	int i,j,k,m,it,u,v;
    double d,f,h,g,p,r,e,s;
    
	// ³õÖµ
	int n = mtxQ.GetNumColumns();
	dblC[n-1]=0.0; 
	d=0.0; 
	f=0.0;
    
	// µü´ú¼ÆËã

	for (j=0; j<=n-1; j++)
    { 
		it=0;
        h=eps*(fabs(dblB[j])+fabs(dblC[j]));
        if (h>d) 
			d=h;
        
		m=j;
        while ((m<=n-1)&&(fabs(dblC[m])>d)) 
			m=m+1;
        
		if (m!=j)
        { 
			do
            { 
				if (it==nMaxIt)
					return FALSE;

                it=it+1;
                g=dblB[j];
                p=(dblB[j+1]-g)/(2.0*dblC[j]);
                r=sqrt(p*p+1.0);
                if (p>=0.0) 
					dblB[j]=dblC[j]/(p+r);
                else 
					dblB[j]=dblC[j]/(p-r);
                
				h=g-dblB[j];
                for (i=j+1; i<=n-1; i++)
					dblB[i]=dblB[i]-h;
                
				f=f+h; 
				p=dblB[m]; 
				e=1.0; 
				s=0.0;
                for (i=m-1; i>=j; i--)
                { 
					g=e*dblC[i]; 
					h=e*p;
                    if (fabs(p)>=fabs(dblC[i]))
                    { 
						e=dblC[i]/p; 
						r=sqrt(e*e+1.0);
                        dblC[i+1]=s*p*r; 
						s=e/r; 
						e=1.0/r;
                    }
                    else
					{ 
						e=p/dblC[i]; 
						r=sqrt(e*e+1.0);
                        dblC[i+1]=s*dblC[i]*r;
                        s=1.0/r; 
						e=e/r;
                    }
                    
					p=e*dblB[i]-s*g;
                    dblB[i+1]=h+s*(e*g+s*dblB[i]);
                    for (k=0; k<=n-1; k++)
                    { 
						u=k*n+i+1; 
						v=u-1;
                        h=mtxQ.m_pData[u]; 
						mtxQ.m_pData[u]=s*mtxQ.m_pData[v]+e*h;
                        mtxQ.m_pData[v]=e*mtxQ.m_pData[v]-s*h;
                    }
                }
                
				dblC[j]=s*p; 
				dblB[j]=e*p;
            
			} while (fabs(dblC[j])>d);
        }
        
		dblB[j]=dblB[j]+f;
    }
    
	for (i=0; i<=n-1; i++)
    { 
		k=i; 
		p=dblB[i];
        if (i+1<=n-1)
        { 
			j=i+1;
            while ((j<=n-1)&&(dblB[j]<=p))
            { 
				k=j; 
				p=dblB[j]; 
				j=j+1;
			}
        }

        if (k!=i)
        { 
			dblB[k]=dblB[i]; 
			dblB[i]=p;
            for (j=0; j<=n-1; j++)
            { 
				u=j*n+i; 
				v=j*n+k;
                p=mtxQ.m_pData[u]; 
				mtxQ.m_pData[u]=mtxQ.m_pData[v]; 
				mtxQ.m_pData[v]=p;
            }
        }
    }
    
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Ô¼»¯Ò»°ãÊµ¾ØÕóÎªºÕÉê²®¸ñ¾ØÕóµÄ³õµÈÏàËÆ±ä»»·¨
//
// ²ÎÊý£ºÎÞ
//
// ·µ»ØÖµ£ºÎÞ
//////////////////////////////////////////////////////////////////////
void CMatrix::MakeHberg()
{ 
	int i,j,k,u,v;
    double d,t;

    for (k=1; k<=m_nNumColumns-2; k++)
    { 
		d=0.0;
        for (j=k; j<=m_nNumColumns-1; j++)
        { 
			u=j*m_nNumColumns+k-1; 
			t=m_pData[u];
            if (fabs(t)>fabs(d))
            { 
				d=t; 
				i=j;
			}
        }
        
		if (d != 0.0)
        { 
			if (i!=k)
            { 
				for (j=k-1; j<=m_nNumColumns-1; j++)
                { 
					u=i*m_nNumColumns+j; 
					v=k*m_nNumColumns+j;
                    t=m_pData[u]; 
					m_pData[u]=m_pData[v]; 
					m_pData[v]=t;
                }
                
				for (j=0; j<=m_nNumColumns-1; j++)
                { 
					u=j*m_nNumColumns+i; 
					v=j*m_nNumColumns+k;
                    t=m_pData[u]; 
					m_pData[u]=m_pData[v]; 
					m_pData[v]=t;
                }
            }
            
			for (i=k+1; i<=m_nNumColumns-1; i++)
            { 
				u=i*m_nNumColumns+k-1; 
				t=m_pData[u]/d; 
				m_pData[u]=0.0;
                for (j=k; j<=m_nNumColumns-1; j++)
                { 
					v=i*m_nNumColumns+j;
                    m_pData[v]=m_pData[v]-t*m_pData[k*m_nNumColumns+j];
                }
                
				for (j=0; j<=m_nNumColumns-1; j++)
                { 
					v=j*m_nNumColumns+k;
                    m_pData[v]=m_pData[v]+t*m_pData[j*m_nNumColumns+i];
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
// ÇóºÕÉê²®¸ñ¾ØÕóÈ«²¿ÌØÕ÷ÖµµÄQR·½·¨
//
// ²ÎÊý£º
// 1. double dblU[] - Ò»Î¬Êý×é£¬³¤¶ÈÎª¾ØÕóµÄ½×Êý£¬·µ»ØÊ±´æ·ÅÌØÕ÷ÖµµÄÊµ²¿
// 2. double dblV[] - Ò»Î¬Êý×é£¬³¤¶ÈÎª¾ØÕóµÄ½×Êý£¬·µ»ØÊ±´æ·ÅÌØÕ÷ÖµµÄÐé²¿
// 3. int nMaxIt - µü´ú´ÎÊý£¬Ä¬ÈÏÖµÎª60
// 4. double eps - ¼ÆËã¾«¶È£¬Ä¬ÈÏÖµÎª0.000001
//
// ·µ»ØÖµ£ºBOOLÐÍ£¬Çó½âÊÇ·ñ³É¹¦
//////////////////////////////////////////////////////////////////////
BOOL CMatrix::HBergEigenv(double dblU[], double dblV[], int nMaxIt /*= 60*/, double eps /*= 0.000001*/)
{ 
	int m,it,i,j,k,l,ii,jj,kk,ll;
    double b,c,w,g,xy,p,q,r,x,s,e,f,z,y;
    
	int n = m_nNumColumns;

	it=0; 
	m=n;
    while (m!=0)
    { 
		l=m-1;
        while ((l>0)&&(fabs(m_pData[l*n+l-1]) > 
				eps*(fabs(m_pData[(l-1)*n+l-1])+fabs(m_pData[l*n+l])))) 
		  l=l-1;

        ii=(m-1)*n+m-1; 
		jj=(m-1)*n+m-2;
        kk=(m-2)*n+m-1; 
		ll=(m-2)*n+m-2;
        if (l==m-1)
        { 
			dblU[m-1]=m_pData[(m-1)*n+m-1]; 
			dblV[m-1]=0.0;
            m=m-1; 
			it=0;
        }
        else if (l==m-2)
        { 
			b=-(m_pData[ii]+m_pData[ll]);
            c=m_pData[ii]*m_pData[ll]-m_pData[jj]*m_pData[kk];
            w=b*b-4.0*c;
            y=sqrt(fabs(w));
            if (w>0.0)
            { 
				xy=1.0;
                if (b<0.0) 
					xy=-1.0;
                dblU[m-1]=(-b-xy*y)/2.0;
                dblU[m-2]=c/dblU[m-1];
                dblV[m-1]=0.0; dblV[m-2]=0.0;
            }
            else
            { 
				dblU[m-1]=-b/2.0; 
				dblU[m-2]=dblU[m-1];
                dblV[m-1]=y/2.0; 
				dblV[m-2]=-dblV[m-1];
            }
            
			m=m-2; 
			it=0;
        }
        else
        { 
			if (it>=nMaxIt)
				return FALSE;

            it=it+1;
            for (j=l+2; j<=m-1; j++)
				m_pData[j*n+j-2]=0.0;
            for (j=l+3; j<=m-1; j++)
				m_pData[j*n+j-3]=0.0;
            for (k=l; k<=m-2; k++)
            { 
				if (k!=l)
                { 
					p=m_pData[k*n+k-1]; 
					q=m_pData[(k+1)*n+k-1];
                    r=0.0;
                    if (k!=m-2) 
						r=m_pData[(k+2)*n+k-1];
                }
                else
                { 
					x=m_pData[ii]+m_pData[ll];
                    y=m_pData[ll]*m_pData[ii]-m_pData[kk]*m_pData[jj];
                    ii=l*n+l; 
					jj=l*n+l+1;
                    kk=(l+1)*n+l; 
					ll=(l+1)*n+l+1;
                    p=m_pData[ii]*(m_pData[ii]-x)+m_pData[jj]*m_pData[kk]+y;
                    q=m_pData[kk]*(m_pData[ii]+m_pData[ll]-x);
                    r=m_pData[kk]*m_pData[(l+2)*n+l+1];
                }
                
				if ((fabs(p)+fabs(q)+fabs(r))!=0.0)
                { 
					xy=1.0;
                    if (p<0.0) 
						xy=-1.0;
                    s=xy*sqrt(p*p+q*q+r*r);
                    if (k!=l) 
						m_pData[k*n+k-1]=-s;
                    e=-q/s; 
					f=-r/s; 
					x=-p/s;
                    y=-x-f*r/(p+s);
                    g=e*r/(p+s);
                    z=-x-e*q/(p+s);
                    for (j=k; j<=m-1; j++)
                    { 
						ii=k*n+j; 
						jj=(k+1)*n+j;
                        p=x*m_pData[ii]+e*m_pData[jj];
                        q=e*m_pData[ii]+y*m_pData[jj];
                        r=f*m_pData[ii]+g*m_pData[jj];
                        if (k!=m-2)
                        { 
							kk=(k+2)*n+j;
                            p=p+f*m_pData[kk];
                            q=q+g*m_pData[kk];
                            r=r+z*m_pData[kk]; 
							m_pData[kk]=r;
                        }
                        
						m_pData[jj]=q; m_pData[ii]=p;
                    }
                    
					j=k+3;
                    if (j>=m-1) 
						j=m-1;
                    
					for (i=l; i<=j; i++)
                    { 
						ii=i*n+k; 
						jj=i*n+k+1;
                        p=x*m_pData[ii]+e*m_pData[jj];
                        q=e*m_pData[ii]+y*m_pData[jj];
                        r=f*m_pData[ii]+g*m_pData[jj];
                        if (k!=m-2)
                        { 
							kk=i*n+k+2;
                            p=p+f*m_pData[kk];
                            q=q+g*m_pData[kk];
                            r=r+z*m_pData[kk]; 
							m_pData[kk]=r;
                        }
                        
						m_pData[jj]=q; 
						m_pData[ii]=p;
                    }
                }
            }
        }
    }
    
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// ÇóÊµ¶Ô³Æ¾ØÕóÌØÕ÷ÖµÓëÌØÕ÷ÏòÁ¿µÄÑÅ¿É±È·¨
//
// ²ÎÊý£º
// 1. double dblEigenValue[] - Ò»Î¬Êý×é£¬³¤¶ÈÎª¾ØÕóµÄ½×Êý£¬·µ»ØÊ±´æ·ÅÌØÕ÷Öµ
// 2. CMatrix& mtxEigenVector - ·µ»ØÊ±´æ·ÅÌØÕ÷ÏòÁ¿¾ØÕó£¬ÆäÖÐµÚiÁÐÎªÓë
//    Êý×édblEigenValueÖÐµÚj¸öÌØÕ÷Öµ¶ÔÓ¦µÄÌØÕ÷ÏòÁ¿
// 3. int nMaxIt - µü´ú´ÎÊý£¬Ä¬ÈÏÖµÎª60
// 4. double eps - ¼ÆËã¾«¶È£¬Ä¬ÈÏÖµÎª0.000001
//
// ·µ»ØÖµ£ºBOOLÐÍ£¬Çó½âÊÇ·ñ³É¹¦
//////////////////////////////////////////////////////////////////////
BOOL CMatrix::JacobiEigenv(double dblEigenValue[], CMatrix& mtxEigenVector, int nMaxIt /*= 60*/, double eps /*= 0.000001*/)
{ 
	int i,j,p,q,u,w,t,s,l;
    double fm,cn,sn,omega,x,y,d;
    
	if (! mtxEigenVector.Init(m_nNumColumns, m_nNumColumns))
		return FALSE;

	l=1;
    for (i=0; i<=m_nNumColumns-1; i++)
    { 
		mtxEigenVector.m_pData[i*m_nNumColumns+i]=1.0;
        for (j=0; j<=m_nNumColumns-1; j++)
			if (i!=j) 
				mtxEigenVector.m_pData[i*m_nNumColumns+j]=0.0;
    }
    
	while (TRUE)
    { 
		fm=0.0;
        for (i=1; i<=m_nNumColumns-1; i++)
		{
			for (j=0; j<=i-1; j++)
			{ 
				d=fabs(m_pData[i*m_nNumColumns+j]);
				if ((i!=j)&&(d>fm))
				{ 
					fm=d; 
					p=i; 
					q=j;
				}
			}
		}

        if (fm<eps)
		{
			for (i=0; i<m_nNumColumns; ++i)
				dblEigenValue[i] = GetElement(i,i);
			return TRUE;
		}

        if (l>nMaxIt)  
			return FALSE;
        
		l=l+1;
        u=p*m_nNumColumns+q; 
		w=p*m_nNumColumns+p; 
		t=q*m_nNumColumns+p; 
		s=q*m_nNumColumns+q;
        x=-m_pData[u]; 
		y=(m_pData[s]-m_pData[w])/2.0;
        omega=x/sqrt(x*x+y*y);

        if (y<0.0) 
			omega=-omega;

        sn=1.0+sqrt(1.0-omega*omega);
        sn=omega/sqrt(2.0*sn);
        cn=sqrt(1.0-sn*sn);
        fm=m_pData[w];
        m_pData[w]=fm*cn*cn+m_pData[s]*sn*sn+m_pData[u]*omega;
        m_pData[s]=fm*sn*sn+m_pData[s]*cn*cn-m_pData[u]*omega;
        m_pData[u]=0.0; 
		m_pData[t]=0.0;
        for (j=0; j<=m_nNumColumns-1; j++)
		{
			if ((j!=p)&&(j!=q))
			{ 
				u=p*m_nNumColumns+j; w=q*m_nNumColumns+j;
				fm=m_pData[u];
				m_pData[u]=fm*cn+m_pData[w]*sn;
				m_pData[w]=-fm*sn+m_pData[w]*cn;
			}
		}

        for (i=0; i<=m_nNumColumns-1; i++)
		{
			if ((i!=p)&&(i!=q))
            { 
				u=i*m_nNumColumns+p; 
				w=i*m_nNumColumns+q;
				fm=m_pData[u];
				m_pData[u]=fm*cn+m_pData[w]*sn;
				m_pData[w]=-fm*sn+m_pData[w]*cn;
            }
		}

        for (i=0; i<=m_nNumColumns-1; i++)
        { 
			u=i*m_nNumColumns+p; 
			w=i*m_nNumColumns+q;
            fm=mtxEigenVector.m_pData[u];
            mtxEigenVector.m_pData[u]=fm*cn+mtxEigenVector.m_pData[w]*sn;
            mtxEigenVector.m_pData[w]=-fm*sn+mtxEigenVector.m_pData[w]*cn;
        }
    }
    
	for (i=0; i<m_nNumColumns; ++i)
		dblEigenValue[i] = GetElement(i,i);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// ÇóÊµ¶Ô³Æ¾ØÕóÌØÕ÷ÖµÓëÌØÕ÷ÏòÁ¿µÄÑÅ¿É±È¹ý¹Ø·¨
//
// ²ÎÊý£º
// 1. double dblEigenValue[] - Ò»Î¬Êý×é£¬³¤¶ÈÎª¾ØÕóµÄ½×Êý£¬·µ»ØÊ±´æ·ÅÌØÕ÷Öµ
// 2. CMatrix& mtxEigenVector - ·µ»ØÊ±´æ·ÅÌØÕ÷ÏòÁ¿¾ØÕó£¬ÆäÖÐµÚiÁÐÎªÓë
//    Êý×édblEigenValueÖÐµÚj¸öÌØÕ÷Öµ¶ÔÓ¦µÄÌØÕ÷ÏòÁ¿
// 3. double eps - ¼ÆËã¾«¶È£¬Ä¬ÈÏÖµÎª0.000001
//
// ·µ»ØÖµ£ºBOOLÐÍ£¬Çó½âÊÇ·ñ³É¹¦
//////////////////////////////////////////////////////////////////////
BOOL CMatrix::JacobiEigenv2(double dblEigenValue[], CMatrix& mtxEigenVector, double eps /*= 0.000001*/)
{ 
	int i,j,p,q,u,w,t,s;
    double ff,fm,cn,sn,omega,x,y,d;
    
	if (! mtxEigenVector.Init(m_nNumColumns, m_nNumColumns))
		return FALSE;

	for (i=0; i<=m_nNumColumns-1; i++)
    { 
		mtxEigenVector.m_pData[i*m_nNumColumns+i]=1.0;
        for (j=0; j<=m_nNumColumns-1; j++)
			if (i!=j) 
				mtxEigenVector.m_pData[i*m_nNumColumns+j]=0.0;
    }
    
	ff=0.0;
    for (i=1; i<=m_nNumColumns-1; i++)
	{
		for (j=0; j<=i-1; j++)
		{ 
			d=m_pData[i*m_nNumColumns+j]; 
			ff=ff+d*d; 
		}
	}

    ff=sqrt(2.0*ff);

Loop_0:
    
	ff=ff/(1.0*m_nNumColumns);

Loop_1:

    for (i=1; i<=m_nNumColumns-1; i++)
	{
		for (j=0; j<=i-1; j++)
        { 
			d=fabs(m_pData[i*m_nNumColumns+j]);
            if (d>ff)
            { 
				p=i; 
				q=j;
                goto Loop_2;
            }
        }
	}
        
	if (ff<eps) 
	{
		for (i=0; i<m_nNumColumns; ++i)
			dblEigenValue[i] = GetElement(i,i);
		return TRUE;
	}
    
	goto Loop_0;

Loop_2: 
		
	u=p*m_nNumColumns+q; 
	w=p*m_nNumColumns+p; 
	t=q*m_nNumColumns+p; 
	s=q*m_nNumColumns+q;
    x=-m_pData[u]; 
	y=(m_pData[s]-m_pData[w])/2.0;
    omega=x/sqrt(x*x+y*y);
    if (y<0.0) 
		omega=-omega;
    
	sn=1.0+sqrt(1.0-omega*omega);
    sn=omega/sqrt(2.0*sn);
    cn=sqrt(1.0-sn*sn);
    fm=m_pData[w];
    m_pData[w]=fm*cn*cn+m_pData[s]*sn*sn+m_pData[u]*omega;
    m_pData[s]=fm*sn*sn+m_pData[s]*cn*cn-m_pData[u]*omega;
    m_pData[u]=0.0; m_pData[t]=0.0;
    
	for (j=0; j<=m_nNumColumns-1; j++)
	{
		if ((j!=p)&&(j!=q))
		{ 
			u=p*m_nNumColumns+j; 
			w=q*m_nNumColumns+j;
			fm=m_pData[u];
			m_pData[u]=fm*cn+m_pData[w]*sn;
			m_pData[w]=-fm*sn+m_pData[w]*cn;
		}
	}

    for (i=0; i<=m_nNumColumns-1; i++)
    {
		if ((i!=p)&&(i!=q))
        { 
			u=i*m_nNumColumns+p; 
			w=i*m_nNumColumns+q;
			fm=m_pData[u];
			m_pData[u]=fm*cn+m_pData[w]*sn;
			m_pData[w]=-fm*sn+m_pData[w]*cn;
        }
	}
    
	for (i=0; i<=m_nNumColumns-1; i++)
    { 
		u=i*m_nNumColumns+p; 
		w=i*m_nNumColumns+q;
        fm=mtxEigenVector.m_pData[u];
        mtxEigenVector.m_pData[u]=fm*cn+mtxEigenVector.m_pData[w]*sn;
        mtxEigenVector.m_pData[w]=-fm*sn+mtxEigenVector.m_pData[w]*cn;
	}

	goto Loop_1;
}


void CMatrix::SetRowCol(int nRows, int nCols)
{
	m_nNumRows = nRows;
	m_nNumColumns = nCols;
	if(m_pData)
	{
		delete[] m_pData;
		m_pData = NULL;
	}
	BOOL bSuccess = Init(m_nNumRows, m_nNumColumns);
	ASSERT(bSuccess);
}

// Êä³öµ½×Ö·ûÖÕ¶Ë
// mtxName -- The Name of the Matrix
void CMatrix::Print(CString mtxName)
{
	fprintf(stdout, "\nMatrix %s(%d, %d)\n", mtxName, m_nNumRows, m_nNumColumns);
	for(int i = 0; i < m_nNumRows; i ++)
	{
		for(int j = 0; j < m_nNumColumns; j ++)
		{
			fprintf(stdout, "M[%d][%d] = %f,",i,j,GetElement(i,j));
		}
		fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");
}

void CMatrix::Matrix2Array(double** data)
{
	int i, j;
	for(i = 0; i < m_nNumRows; ++ i)
	{
		for(j = 0; j < m_nNumColumns; ++ j)
		{
			data[i][j] = m_pData[i*m_nNumColumns+j];
		}
	}
}
void CMatrix::Array2Matrix(double** data)
{
	int i, j;
	for(i = 0; i < m_nNumRows; ++ i)
	{
		for(j = 0; j < m_nNumColumns; ++ j)
		{
			m_pData[i*m_nNumColumns+j] = data[i][j];
		}
	}
}

/*******************************************************
 *	Added Function for OpenGL Transformations
 *******************************************************/ 
// Input: Column-orientated date pointer
// Function: Fill the column-orientated matrix with given data
void CMatrix::Array2Matrix(double *data)
{
	int i, j;
	for(i = 0; i < m_nNumRows; ++ i)
	{
		for(j = 0; j < m_nNumColumns; ++ j)
		{
			m_pData[j*m_nNumColumns+i] = data[i*m_nNumColumns+j];
		}
	}
}

void CMatrix::Array2Matrix(double data[3][3])
{
	ASSERT(m_nNumRows == 3 && m_nNumColumns == 3);
	
	int i, j;
	for(i = 0; i < 3; ++ i)
	{
		for(j = 0; j < 3; ++ j)
			m_pData[i*m_nNumColumns+j] = data[i][j];
	}
}

// Output: Column-orientated date pointer
// Function: Fill the column-orientated data pointer with given matrix
void CMatrix::Matrix2Array(double *data)
{
	int i, j;
	for(i = 0; i < m_nNumRows; ++ i)
	{
		for(j = 0; j < m_nNumColumns; ++ j)
		{
			data[i*m_nNumColumns+j] = m_pData[j*m_nNumColumns+i];
		}
	}
}

// Input: the world coordinate
// Output: the coordinate transformed by given matrix
bool CMatrix::TransformVector(Coord &coord)
{
	if(this->m_nNumRows!=4||this->m_nNumColumns!=4)
		return FALSE;
	
	double datacd[4];
	double rslt[4];
	int i,j;
	datacd[0] = coord[0];
	datacd[1] = coord[1];
	datacd[2] = coord[2];
	datacd[3] = 1.0f;

	for(i = 0; i < 4; ++ i)
	{
		rslt[i] = 0;
		for(j = 0; j < 4; ++ j)
			rslt[i] += m_pData[i*4+j]*datacd[j];
	}
	coord[0] = rslt[0];
	coord[1] = rslt[1];
	coord[2] = rslt[2];

	return TRUE;
}

// Input: the translation vector
// Output: the matrix representing given translation vector
bool CMatrix::SetTrans2Matrix(Coord trans)
{
	double TransM[16];
	for(int i = 0;i<16;i++)
		TransM[i] = 0;
	TransM[0] = 1;
	TransM[5] = 1;
	TransM[10] = 1;
	TransM[12] = trans[0];
	TransM[13] = trans[1];
	TransM[14] = trans[2];
	TransM[15] = 1;
	this->Array2Matrix(TransM);

	return true;
}

// Input: the scaling factors
// Output: the matrix representing given scaling factors
bool CMatrix::SetScale2Matrix(Coord scale)
{
	double ScaleM[16];
	for(int ti = 0; ti<16;ti++)
		ScaleM[ti] = 0;
	ScaleM[0] = scale[0];
	ScaleM[5] = scale[1];
	ScaleM[10] = scale[2];
	ScaleM[15] = 1;
	this->Array2Matrix(ScaleM);

	return true;
}

// Input: the rotation angle and axis
// Output: the matrix representing given rotation
bool CMatrix::SetRotation2Matrix(float angle, Coord axis)
{	
	double RotateM[16];
	double length = axis.abs();
	if(length > 1.0e-10)
	{
		axis /= length;

		ROTATION rotate = ROTATION(axis, (double)angle);
		RotationToMatrix(RotateM, rotate);
		this->Array2Matrix(RotateM);
	}
	else
		this->MakeUnitMatrix(4);

	return true;
}