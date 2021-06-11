#if !defined(sharedtypes_h)
#define sharedtypes_h

#include <istream>
#include <fstream>
#include <math.h>

/***************************************************************************/
/* Constant definitions of use in calculations                             */

const double PI = 3.141592654;
const double DOUBLE_UNDERFLOW = 1e-25;
const double REALLY_REALLY_ZERO = 1e-40;

/***************************************************************************/
/* Helper functions for double comparison                                  */

/**
 * Checks if a double is zero.  As doubles produce small variations
 * due to rounding this method returns true if
 * -DOUBLE_UNDERFLOW < val < DOUBLE_UNDERFLOW
 * @param val The value to check
 * @return bool True if the value is zero (within the tolerance)
 */
template <class T>
bool isZero(const T val)
{
    if(val < DOUBLE_UNDERFLOW && val > (-DOUBLE_UNDERFLOW))
    {
		return true;
	}
    else
    {
        return false;
    }
}

/**
 * Checks if a double is zero.  As doubles produce small variations
 * due to rounding this method returns true if
 * -tolerance < val < tolerance
 * The tolerance defaults to REALLY_REALLY_ZERO which is substantially more
 * limiting than the value used by isZero()
 * @param val The value to check
 * @param tolerance The tolerance to check to
 * @return bool True if the value is zero (within the tolerance)
 */
template <class T>
inline bool isReallyReallyZero(const T val, const T tolerance = REALLY_REALLY_ZERO)
{
    if(val <= tolerance && val >= (-tolerance))
    {
		return true;
	}
    else
    {
        return false;
    }
}

/**
 * Compares 2 doubles.  As doubles produce small variations due to
 * rounding this method allows the difference to be up to
 * REALLY_REALLY_ZERO (or another user supplied value)
 * @param valA The first value to check
 * @param valB The second value to check
 * @param tolerance The tolerance to check to
 * @return bool True if the values are the same (within the tolerance)
 */
template <class T>
inline bool areEqual(const T valA, const T valB, const T tolerance = REALLY_REALLY_ZERO)
{
    if(!isReallyReallyZero(valB, tolerance))
    {
        // valB is not zero
        return ( fabs(valA/valB - 1.0) < 1e-12 );
    }
    else if(!isReallyReallyZero(valA, tolerance))
    {
        // valA is not zero
        return ( fabs(valB/valA - 1.0) < 1e-12 );
    }
    else
    {
        // Both values are effectivly zero so they are the same
        return true;
    }
}

/***************************************************************************/
/* Class definitions for representation of vectors and points in 3D space  */

/**
 * The CVector class represents a vector in 3D space.
 * Points and vectors differ in the operations that can be performed on them
 */
class CVector
{
public:
	CVector() {m_nX = 0; m_nY = 0; m_nZ = 0;}
    CVector(const CVector &vect);
	CVector(double dx, double dy, double dz) {m_nX = dx; m_nY = dy; m_nZ = dz;}
	virtual ~CVector() {}

 	//functions
	CVector	operator+(const CVector &vector) const;
	void	operator+=(const CVector &vector);
	CVector	operator-(const CVector &vector) const;
    CVector	operator-() const;
    void	operator-=(const CVector &vector);
	CVector	operator*(const double multValue) const;
    void	operator*=(const double multValue);
    CVector	operator/(const double divValue) const;
    void	operator/=(const double divValue);

    CVector	normalise();
    CVector	cross(const CVector &vector) const;
    double  dot(const CVector &vector) const;

    double	length() const { return sqrt(m_nX * m_nX + m_nY * m_nY + m_nZ * m_nZ);}
    double	lengthSquared() const { return m_nX * m_nX + m_nY * m_nY + m_nZ * m_nZ;}

    double	dx() const			{return m_nX;}
    double	dy() const			{return m_nY;}
    double	dz() const			{return m_nZ;}
    void	setdx(double dx)	{m_nX = dx;}
    void	setdy(double dy)	{m_nY = dy;}
    void	setdz(double dz)	{m_nZ = dz;}

	static CVector null;

private:
    virtual void CheckForUnderflow();
    double m_nX, m_nY, m_nZ;
};

/**
 * The CPoint class represents a point in 3D space
 * Points and vectors differ in the operations that can be performed on them
 */
class CPoint
{
public:
	CPoint() {m_nX = 0; m_nY = 0; m_nZ = 0; }
    CPoint(const CPoint &point);
	CPoint(double in_x, double in_y, double in_z);
	virtual ~CPoint() {}

	CPoint	operator+(const CVector &vect) const;
	void    operator+=(const CVector &vect);
    void    operator-=(const CVector &vect);
    CVector operator-(const CPoint &point) const;
    CPoint  operator-(const CVector &vector) const;
    CPoint	operator*( double multValue) const;
    void    operator*=( double multValue);
    CPoint	operator/( double divValue) const;
    void    operator/=( double divValue);
    bool    operator==(const CPoint &point) const;

    friend std::istream& operator>>(std::istream &stream, CPoint &point);

    double	x() const		{return m_nX;}
    double	y() const		{return m_nY;}
    double	z() const		{return m_nZ;}
    void	setX(double x)	{m_nX = x;}
    void	setY(double y)	{m_nY = y;}
    void	setZ(double z)	{m_nZ = z;}

	double	distance(const CPoint &point) const; // distance to point

private:
    virtual void CheckForUnderflow();
    double m_nX, m_nY, m_nZ;
};

typedef CVector CVelocity;
typedef CVector CAngVel;
typedef CVector CTorque;
typedef CVector CForce;

/***************************************************************************/
/* Inline Implementations of all CPoint methods                            */

CPoint::CPoint(const CPoint &point)
{
    m_nX = point.m_nX;
    m_nY = point.m_nY;
    m_nZ = point.m_nZ;

    CheckForUnderflow();
}

CPoint::CPoint(double in_x, double in_y, double in_z)
{
    m_nX = in_x;
    m_nY = in_y;
    m_nZ = in_z;

	CheckForUnderflow();
}

void CPoint::CheckForUnderflow()
{
    if(isZero(m_nX)) m_nX = 0;
    if(isZero(m_nY)) m_nY = 0;
    if(isZero(m_nZ)) m_nZ = 0;
}

CPoint CPoint::operator +(const CVector &vect) const
{
	CPoint retVal;

	retVal.setX(m_nX + vect.dx());
	retVal.setY(m_nY + vect.dy());
    retVal.setZ(m_nZ + vect.dz());

	return retVal;

}

void CPoint::operator+=(const CVector &vect)
{
	m_nX += vect.dx();
	m_nY += vect.dy();
	m_nZ += vect.dz();

	CheckForUnderflow();
}

void CPoint::operator-=(const CVector &vect)
{
	m_nX -= vect.dx();
	m_nY -= vect.dy();
	m_nZ -= vect.dz();

	CheckForUnderflow();
}

CVector CPoint::operator -(const CPoint &point) const
{
    CVector vector(m_nX - point.m_nX, m_nY - point.m_nY, m_nZ - point.m_nZ);
    return vector;
}

CPoint CPoint::operator -(const CVector &vector) const
{
    CPoint point(m_nX - vector.dx(), m_nY - vector.dy(), m_nZ - vector.dz());
    return point;
}

CPoint CPoint::operator/(double divValue) const
{
	CPoint retVal;

	if(!isZero(divValue))
    {
        retVal.setX (m_nX / divValue);
        retVal.setY (m_nY / divValue);
        retVal.setZ (m_nZ / divValue);
    }
    else
    {
        retVal.setX (9E30);
        retVal.setY (9E30);
        retVal.setZ (9E30);
    }

	return retVal;
}

void CPoint::operator/=(double divValue)
{
	if(!isZero(divValue))
    {
        m_nX /= divValue;
	    m_nY /= divValue;
	    m_nZ /= divValue;
    }
    else
    {
        m_nX = 9E30;
        m_nY = 9E30;
        m_nZ = 9E30;
    }

    CheckForUnderflow();
}

CPoint CPoint::operator*(double multValue) const
{
	CPoint retVal;

    retVal.setX (m_nX * multValue);
    retVal.setY (m_nY * multValue);
    retVal.setZ (m_nZ * multValue);

	return retVal;
}

void CPoint::operator*=(double multValue)
{
    m_nX /= multValue;
	m_nY /= multValue;
	m_nZ /= multValue;

    CheckForUnderflow();
}

bool CPoint::operator==(const CPoint &point)  const
{
    if(isZero(m_nX - point.m_nX) && isZero(m_nY - point.m_nY) && isZero(m_nZ - point.m_nZ))
        return true;
    else
        return false;
}

std::istream& operator>>(std::istream &stream, CPoint &point)
{
	stream >> point.m_nX >> point.m_nY >> point.m_nZ;

	return stream;
}

double CPoint::distance(const CPoint &point) const
{
	return sqrt((m_nX-point.x())*(m_nX-point.x()) + (m_nY-point.y())*(m_nY-point.y()) + (m_nZ-point.z())*(m_nZ-point.z()));
}

/***************************************************************************/
/* Inline Implementations of all CVector methods                           */

CVector CVector::null = CVector(0,0,0);

CVector::CVector(const CVector &vect)
{
    m_nX = vect.m_nX;
    m_nY = vect.m_nY;
    m_nZ = vect.m_nZ;

    CheckForUnderflow();
}

CVector CVector::normalise()
{
    double nLength = length();

    if(!isZero(nLength)) //only normalise if length isn't zero.
    {
        m_nX /= nLength;
        m_nY /= nLength;
        m_nZ /= nLength;
    }

    CheckForUnderflow();

    return *this;
}

void CVector::CheckForUnderflow()
{
    if(isZero(m_nX)) m_nX = 0;
    if(isZero(m_nY)) m_nY = 0;
    if(isZero(m_nZ)) m_nZ = 0;
}

CVector CVector::operator+(const CVector &vector) const
{
	CVector retVal;

	retVal.setdx( vector.m_nX + m_nX);
    retVal.setdy( vector.m_nY + m_nY);
    retVal.setdz( vector.m_nZ + m_nZ);

	return retVal;
}

void CVector::operator+=(const CVector &vector)
{
    m_nX += vector.m_nX;
    m_nY += vector.m_nY;
    m_nZ += vector.m_nZ;

    CheckForUnderflow();
}

void CVector::operator-=(const CVector &vector)
{
    m_nX -= vector.m_nX;
    m_nY -= vector.m_nY;
    m_nZ -= vector.m_nZ;

	CheckForUnderflow();
}

CVector CVector::operator-() const
{
    return  CVector(-m_nX, -m_nY, -m_nZ);
}

CVector CVector::operator-(const CVector &vector) const
{
	CVector retVal;

	retVal.setdx( m_nX - vector.m_nX);
    retVal.setdy( m_nY - vector.m_nY);
    retVal.setdz( m_nZ - vector.m_nZ);

	return retVal;
}

CVector CVector::operator*(const double multValue) const
{
	CVector retVal;

    retVal.setdx( m_nX * multValue);
    retVal.setdy( m_nY* multValue);
    retVal.setdz( m_nZ * multValue);

	return retVal;
}

void CVector::operator*=(const double multValue)
{
	m_nX *= multValue;
	m_nY *= multValue;
	m_nZ *= multValue;

    CheckForUnderflow();
}

CVector CVector::operator/(const double divValue) const
{
	CVector retVal;

	if(!isZero(divValue))
    {
        retVal.setdx( m_nX / divValue);
        retVal.setdy( m_nY  / divValue);
        retVal.setdz( m_nZ  / divValue);
    }
    else
    {
        retVal.setdx( 9E30);
        retVal.setdy( 9E30);
        retVal.setdz( 9E30);
    }

	return retVal;
}

void CVector::operator/=(const double divValue)
{
	if(!isZero(divValue))
    {
        m_nX /= divValue;
	    m_nY /= divValue;
	    m_nZ /= divValue;
    }
    else
    {
        m_nX = 9E30;
        m_nY = 9E30;
        m_nZ = 9E30;
    }

    CheckForUnderflow();
}

CVector CVector::cross(const CVector &vector) const
{
    CVector retVect;

	retVect.setdx( m_nY * vector.m_nZ - m_nZ * vector.m_nY );
    retVect.setdy( m_nZ * vector.m_nX - m_nX * vector.m_nZ );
    retVect.setdz( m_nX * vector.m_nY - m_nY * vector.m_nX );

    return retVect;
}

double CVector::dot(const CVector &vector) const
{
    return m_nX * vector.m_nX + m_nY * vector.m_nY + m_nZ * vector.m_nZ;
}

#endif
