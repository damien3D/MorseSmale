/*_____________________________________________________
 |
 |      Intra3D Layer1 :  Algebra ¹¹¼þ×é³ÉÖ®Ò»
 |                                          
 |  ÎÄ¼þ£ºRotation.h
 |
 |  ¹¦ÄÜ£ºËÄÔª×éÐý×ªÔËËã
 |
 |  ¿ª·¢£ºÁÖÈñ £¬1999/01/10
 |
 |      Ô´³ÌÐò²âÊÔ£º½øÐÐÁËµ¥²½¸ú×Ù²âÊÔ
 |_____________________________________________________*/

/*_____________________________________________________
 |  
 |      »ùÓÚ OpenGL µÄ½»»¥Ê½ÈýÎ¬Í¼ÐÎÈí¼þ¿ª·¢¹¤¾ß
 |
 |      **   Intra3D 2.0 for Windows 9X/NT  **
 |
 |                Öø×÷È¨ÈË£ºÁÖÈñ 
 |      
 |      Õã½­´óÑ§ CAD&CG ¹ú¼ÒÖØµãÊµÑéÊÒ (310027)
 |_____________________________________________________*/


#if !defined(ROTATION_H)
#define          ROTATION_H

#include "..\MeshLib\BasicElement.h"

/*______________________ ROTATION Í¨¹ý QUATERNION ÊµÏÖ _____________________*/


// ¶¨ÒåÐý×ª ROTATION ±ä»», ROTATION ¹éÒ»»¯ÊÇÖ¸ axis ¹éÒ»»¯
// ÓÃ ROTATION ½á¹¹±íÊ¾Ðý×ªÊ±£¬(axis, angle) Óë -(axis, angle) µÈ¼Û

class ROTATION
{
public:
        Coord axis ;    // Unit Vector  
        double  angle;  // 0 - 360 degree
        // ¹¹Ôì¹éÒ»»¯µÄ ROTATION 
        ROTATION(Coord Axis, double fAngle=0.0f)
        { axis=Axis; angle=fAngle; axis.normalize(); }
        // ¹¹Ôì¹éÒ»»¯µÄ ROTATION 
        ROTATION(double fX=0.0f, double fY=0.0f, double fZ=1.0f, double fAngle=0.0f)
        { axis[0]=fX; axis[1]=fY; axis[2]=fZ; angle=fAngle; axis.normalize(); }

        
};

// ROTATION Ïà³Ë£¬·µ»ØÖµ = R1 * R2  (ÏÈÖ´ÐÐ R1 Ðý×ª£¬ºóÖ´ÐÐ R2 Ðý×ª)
// ÒªÇó R1 R2 ÏÈ¹éÒ»»¯
ROTATION RotationMultiply( ROTATION R1,  ROTATION R2);

ROTATION operator *      ( ROTATION R1,  ROTATION R2);

// V ¾­¹ý R ±ä»»ºóÎª V', º¯Êý·µ»Ø V'
// ÒªÇó R ÏÈ¹éÒ»»¯

Coord VectorTransform(const Coord V, const ROTATION R);

// ½« ROTATION ½á¹¹±íÊ¾³É Matrix
// ÒªÇó R ÏÈ¹éÒ»»¯
void RotationToMatrix(double M[16], const ROTATION R);


/*_______________ ²»ÏëÁË½âËÄÔª×éÏ¸½ÚµÄ³ÌÐòÔ±²»±ØÍùÏÂ¿´ _____________________*/



// ¶¨ÒåËÄÔª×é QUATERNION
// ÓÃ QUATERNION ½á¹¹±íÊ¾Ðý×ªÊ±£¬(x,y,z,w) Óë -(x,y,z,w) µÈ¼Û

class QUATERNION
{
public:
        double x, y, z, w;
        QUATERNION(double fX=0.0f, double fY=0.0f, double fZ=0.0f, double fW=1.0f)
        { x=fX; y=fY; z=fZ; w=fW; }
};


// ËÄÔª×éÇóÄ££¬·µ»ØÖµ = |A|
double QuaternionMagnitude(const QUATERNION A);

// ËÄÔª×é¹éÒ»»¯
// Èç¹û |A|=0£¬Êä³öÖµ = A£»Êä³öÖµ = A/(|A|)
void QuaternionNormalize(QUATERNION *A);

// ËÄÔª×éÇóÄæ
// Èç¹û |A|=0£¬Êä³öÖµ = A£»·ñÔòÊä³öÖµ = A µÄÄæ
void QuaternionInverse(QUATERNION *A);

// ËÄÔª×é¹²¶ó£¬Êä³öÖµ = A µÄ¹²¶ó
void QuaternionConjugate(QUATERNION *A);

// ËÄÔª×éÏà¼Ó£¬·µ»ØÖµ = A + B
QUATERNION QuaternionAdd(const QUATERNION A, const QUATERNION B);

QUATERNION operator +   (const QUATERNION A, const QUATERNION B);

// ËÄÔª×éÏà¼õ£¬·µ»ØÖµ = A - B
QUATERNION QuaternionSub(const QUATERNION A, const QUATERNION B);

QUATERNION operator -   (const QUATERNION A, const QUATERNION B);

// ËÄÔª×éËõ·Å£¬·µ»ØÖµ = s * A 
QUATERNION QuaternionScale(const QUATERNION A, const double s);

QUATERNION operator *     (const QUATERNION A, const double s);

QUATERNION QuaternionScale(const double s, const QUATERNION A);

QUATERNION operator *     (const double s, const QUATERNION A);

// ËÄÔª×éÏà³Ë£¬·µ»ØÖµ = A * B
QUATERNION QuaternionMultiply(const QUATERNION A, const QUATERNION B);

QUATERNION operator *        (const QUATERNION A, const QUATERNION B);  

// Spherical Linear Interpolation Between two Quaternions, 0 < t < 1
QUATERNION QuaternionSlerp(const QUATERNION from, const QUATERNION to, double t);

/*_____________________________________________________
 |                                                     
 |   ÎªÌá¸ß¼ÆËãÐÔÄÜ£¬ÒÔÏÂ±ä»»º¯Êý¾ù¼Ù¶¨
 |
 |   ÊäÈë²ÎÊý R Q V ÒÑ¾­½øÐÐÁË Normalize ´¦Àí    
 |_____________________________________________________*/


/*------------------------------------------------------------------*/
/*----------------------  QUATERNION ¡ª ROTATION  ------------------*/

// ½« ROTATION ½á¹¹±íÊ¾³É QUATERNION
QUATERNION RotationToQuaternion(const ROTATION R);

// ½« QUATERNION ½á¹¹±íÊ¾³É ROTATION
ROTATION QuaternionToRotation(const QUATERNION Q);

/*------------------------------------------------------------------*/
/*----------------------  QUATERNION ¡ª Matrix  --------------------*/

// ½« QUATERNION ½á¹¹±íÊ¾³É Matrix
void QuaternionToMatrix(double M[16], const QUATERNION Q);
void QuaternionToMatrix(double R[3][3], const QUATERNION quat);

// ½« Matrix ½á¹¹±íÊ¾³É QUATERNION
// ÒªÇó Matrix ÊÇÒ»ÖÖÐý×ª¾ØÕó,·ñÔò²»ÄÜµÃµ½ÕýÈ·½á¹û
void MatrixToQuaternion(QUATERNION *Q, const double M[16]);
void MatrixToQuaternion(QUATERNION *quat, const double R[3][3]);
void MatrixToQuaternion2(QUATERNION& quat, const double R[3][3]);
/*------------------------------------------------------------------*/
/*-----------------------  QUATERNION ¡ª Coord  -------------------*/


// ½«Ê¸Á¿£¨»òÈýÎ¬¿Õ¼äµÄÒ»µã£©±íÊ¾³ÉËÄÔª×é
QUATERNION VectorToQuaternion(const Coord V);

// ½«ËÄÔª×éµÄÐé²¿ÓÃÊ¸Á¿±íÊ¾
Coord QuaternionToVector(const QUATERNION Q);

// V ¾­¹ý Q ±ä»»ºóÎª V', º¯Êý·µ»Ø V'
Coord VectorTransform(const Coord V, const QUATERNION Q);
Coord VectorTransform(const Coord V, const ROTATION Q);

#endif
/*_____________________________________________________
 |
 |      Intra3D Layer1 :  Algebra ¹¹¼þ×é³ÉÖ®Ò»
 |                                          
 |  ÎÄ¼þ£ºRotation.h
 |
 |  ¹¦ÄÜ£ºËÄÔª×éÐý×ªÔËËã
 |
 |  ¿ª·¢£ºÁÖÈñ £¬1999/01/10
 |
 |      Ô´³ÌÐò²âÊÔ£º½øÐÐÁËµ¥²½¸ú×Ù²âÊÔ
 |_____________________________________________________*/

/*_____________________________________________________
 |  
 |      »ùÓÚ OpenGL µÄ½»»¥Ê½ÈýÎ¬Í¼ÐÎÈí¼þ¿ª·¢¹¤¾ß
 |
 |      **   Intra3D 2.0 for Windows 9X/NT  **
 |
 |                Öø×÷È¨ÈË£ºÁÖÈñ 
 |      
 |      Õã½­´óÑ§ CAD&CG ¹ú¼ÒÖØµãÊµÑéÊÒ (310027)
 |_____________________________________________________*/


#if !defined(ROTATION_H)
#define          ROTATION_H

#include "..\MeshLib\BasicElement.h"

/*______________________ ROTATION Í¨¹ý QUATERNION ÊµÏÖ _____________________*/


// ¶¨ÒåÐý×ª ROTATION ±ä»», ROTATION ¹éÒ»»¯ÊÇÖ¸ axis ¹éÒ»»¯
// ÓÃ ROTATION ½á¹¹±íÊ¾Ðý×ªÊ±£¬(axis, angle) Óë -(axis, angle) µÈ¼Û

class ROTATION
{
public:
        Coord axis ;    // Unit Vector  
        double  angle;  // 0 - 360 degree
        // ¹¹Ôì¹éÒ»»¯µÄ ROTATION 
        ROTATION(Coord Axis, double fAngle=0.0f)
        { axis=Axis; angle=fAngle; axis.normalize(); }
        // ¹¹Ôì¹éÒ»»¯µÄ ROTATION 
        ROTATION(double fX=0.0f, double fY=0.0f, double fZ=1.0f, double fAngle=0.0f)
        { axis[0]=fX; axis[1]=fY; axis[2]=fZ; angle=fAngle; axis.normalize(); }

        
};

// ROTATION Ïà³Ë£¬·µ»ØÖµ = R1 * R2  (ÏÈÖ´ÐÐ R1 Ðý×ª£¬ºóÖ´ÐÐ R2 Ðý×ª)
// ÒªÇó R1 R2 ÏÈ¹éÒ»»¯
ROTATION RotationMultiply( ROTATION R1,  ROTATION R2);

ROTATION operator *      ( ROTATION R1,  ROTATION R2);

// V ¾­¹ý R ±ä»»ºóÎª V', º¯Êý·µ»Ø V'
// ÒªÇó R ÏÈ¹éÒ»»¯

Coord VectorTransform(const Coord V, const ROTATION R);

// ½« ROTATION ½á¹¹±íÊ¾³É Matrix
// ÒªÇó R ÏÈ¹éÒ»»¯
void RotationToMatrix(double M[16], const ROTATION R);


/*_______________ ²»ÏëÁË½âËÄÔª×éÏ¸½ÚµÄ³ÌÐòÔ±²»±ØÍùÏÂ¿´ _____________________*/



// ¶¨ÒåËÄÔª×é QUATERNION
// ÓÃ QUATERNION ½á¹¹±íÊ¾Ðý×ªÊ±£¬(x,y,z,w) Óë -(x,y,z,w) µÈ¼Û

class QUATERNION
{
public:
        double x, y, z, w;
        QUATERNION(double fX=0.0f, double fY=0.0f, double fZ=0.0f, double fW=1.0f)
        { x=fX; y=fY; z=fZ; w=fW; }
};


// ËÄÔª×éÇóÄ££¬·µ»ØÖµ = |A|
double QuaternionMagnitude(const QUATERNION A);

// ËÄÔª×é¹éÒ»»¯
// Èç¹û |A|=0£¬Êä³öÖµ = A£»Êä³öÖµ = A/(|A|)
void QuaternionNormalize(QUATERNION *A);

// ËÄÔª×éÇóÄæ
// Èç¹û |A|=0£¬Êä³öÖµ = A£»·ñÔòÊä³öÖµ = A µÄÄæ
void QuaternionInverse(QUATERNION *A);

// ËÄÔª×é¹²¶ó£¬Êä³öÖµ = A µÄ¹²¶ó
void QuaternionConjugate(QUATERNION *A);

// ËÄÔª×éÏà¼Ó£¬·µ»ØÖµ = A + B
QUATERNION QuaternionAdd(const QUATERNION A, const QUATERNION B);

QUATERNION operator +   (const QUATERNION A, const QUATERNION B);

// ËÄÔª×éÏà¼õ£¬·µ»ØÖµ = A - B
QUATERNION QuaternionSub(const QUATERNION A, const QUATERNION B);

QUATERNION operator -   (const QUATERNION A, const QUATERNION B);

// ËÄÔª×éËõ·Å£¬·µ»ØÖµ = s * A 
QUATERNION QuaternionScale(const QUATERNION A, const double s);

QUATERNION operator *     (const QUATERNION A, const double s);

QUATERNION QuaternionScale(const double s, const QUATERNION A);

QUATERNION operator *     (const double s, const QUATERNION A);

// ËÄÔª×éÏà³Ë£¬·µ»ØÖµ = A * B
QUATERNION QuaternionMultiply(const QUATERNION A, const QUATERNION B);

QUATERNION operator *        (const QUATERNION A, const QUATERNION B);  

// Spherical Linear Interpolation Between two Quaternions, 0 < t < 1
QUATERNION QuaternionSlerp(const QUATERNION from, const QUATERNION to, double t);

/*_____________________________________________________
 |                                                     
 |   ÎªÌá¸ß¼ÆËãÐÔÄÜ£¬ÒÔÏÂ±ä»»º¯Êý¾ù¼Ù¶¨
 |
 |   ÊäÈë²ÎÊý R Q V ÒÑ¾­½øÐÐÁË Normalize ´¦Àí    
 |_____________________________________________________*/


/*------------------------------------------------------------------*/
/*----------------------  QUATERNION ¡ª ROTATION  ------------------*/

// ½« ROTATION ½á¹¹±íÊ¾³É QUATERNION
QUATERNION RotationToQuaternion(const ROTATION R);

// ½« QUATERNION ½á¹¹±íÊ¾³É ROTATION
ROTATION QuaternionToRotation(const QUATERNION Q);

/*------------------------------------------------------------------*/
/*----------------------  QUATERNION ¡ª Matrix  --------------------*/

// ½« QUATERNION ½á¹¹±íÊ¾³É Matrix
void QuaternionToMatrix(double M[16], const QUATERNION Q);
void QuaternionToMatrix(double R[3][3], const QUATERNION quat);

// ½« Matrix ½á¹¹±íÊ¾³É QUATERNION
// ÒªÇó Matrix ÊÇÒ»ÖÖÐý×ª¾ØÕó,·ñÔò²»ÄÜµÃµ½ÕýÈ·½á¹û
void MatrixToQuaternion(QUATERNION *Q, const double M[16]);
void MatrixToQuaternion(QUATERNION *quat, const double R[3][3]);
void MatrixToQuaternion2(QUATERNION& quat, const double R[3][3]);
/*------------------------------------------------------------------*/
/*-----------------------  QUATERNION ¡ª Coord  -------------------*/


// ½«Ê¸Á¿£¨»òÈýÎ¬¿Õ¼äµÄÒ»µã£©±íÊ¾³ÉËÄÔª×é
QUATERNION VectorToQuaternion(const Coord V);

// ½«ËÄÔª×éµÄÐé²¿ÓÃÊ¸Á¿±íÊ¾
Coord QuaternionToVector(const QUATERNION Q);

// V ¾­¹ý Q ±ä»»ºóÎª V', º¯Êý·µ»Ø V'
Coord VectorTransform(const Coord V, const QUATERNION Q);
Coord VectorTransform(const Coord V, const ROTATION Q);

#endif
