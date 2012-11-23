#pragma once

template <typename Type>
class Vector3 {	
private:	
	Type	data[3];	
	public:	
	Vector3() 
	{ 
		data[0] = 0; data[1] = 0; data[2] = 0; 
	}	
	Vector3(const Vector3& v) 
	{ 
		data[0] = v.data[0]; data[1] = v.data[1]; data[2] = v.data[2]; 
	}	
	Vector3(const Type x, const Type y, const Type z) 
	{ 
		data[0] = x; data[1] = y; data[2] = z; 
	}	
	Vector3	operator=(const Type d) 
	{ 
		data[0] = d; data[1] = 0; data[2] = 0; return *this; 
	}	
	Vector3	operator=(const Vector3& v) 
	{
		data[0] = v.data[0]; data[1] = v.data[1]; data[2] = v.data[2]; return *this;	
	}	
	Vector3	operator-() 
	{
		Vector3	vn(-data[0], -data[1], -data[2]);
		return vn;	
	}	
	Vector3	operator+=(const double d)
	{	
		for ( int i=0; i<3; i++ ) data[i] += Type(d);	
		return *this;	
	}	
	Vector3	operator+(const double d)
	{	
		Vector3	vn(Type(data[0]+d), Type(data[1]+d), Type(data[2]+d));	
		return vn;	
	}	
	Vector3	operator+=(const Vector3& v)
	{	
		for ( int i=0; i<3; i++ ) data[i] += v.data[i];	
		return *this;	
	}	
	Vector3	operator+(const Vector3& v) 
	{	
		Vector3	vn(data[0]+v.data[0], data[1]+v.data[1], data[2]+v.data[2]);	
		return vn;	
	}	
	Vector3	operator-=(const double d) 
	{	
		for ( int i=0; i<3; i++ ) data[i] -= Type(d);	
		return *this;	
	}	
	Vector3	operator-(const double d)
	{	
		Vector3	vn(Type(data[0]-d), Type(data[1]-d), Type(data[2]-d));	
		return vn;	
	}	
	Vector3	operator-=(const Vector3& v) 
	{	
		for ( int i=0; i<3; i++ ) data[i] -= v.data[i];	
		return *this;	
	}	
	Vector3	operator-(const Vector3& v) 
	{	
		Vector3	vn(data[0]-v.data[0], data[1]-v.data[1], data[2]-v.data[2]);	
		return vn;	
	}	
	Vector3	operator*=(const double d) 
	{	
		for ( int i=0; i<3; i++ ) data[i] = Type(data[i] * d);	
		return *this;	
	}	
	Vector3	operator*(const double d) 
	{	
		Vector3	vn(Type(data[0] * d), Type(data[1] * d), Type(data[2] * d));	
		return vn;	
	}	
	Vector3	operator*=(const Vector3& v) 
	{	// Note that the order is important for vectors of different types
		for ( int i=0; i<3; i++ ) 
			data[i] *= v.data[i];	
		return *this;	
	}	
	Vector3	operator*(const Vector3& v) 
	{	
		Vector3	vn(data[0]*v.data[0], data[1]*v.data[1], data[2]*v.data[2]);	
		return vn;	
	}	
	Vector3 operator/=(const double d)
	{	
		if ( fabs(d) < 1e-30 ) 
			return *this;	
		double	div = 1/d;	
		for ( int i=0; i<3; i++ ) 
			data[i] *= div;	
		return *this;	
	}	
	Vector3	operator/(const double d) 
	{	
		Vector3	vn(Type(data[0] / d), Type(data[1] / d), Type(data[2] / d));	
		return vn;	
	}	
	Vector3	operator/=(const Vector3& v)
	{	
		for ( int i=0; i<3; i++ )
			data[i] /= v.data[i];	
		return *this;	
	}	
	Vector3	operator/(const Vector3& v) 
	{	
		Vector3	vn(data[0]/v.data[0], data[1]/v.data[1], data[2]/v.data[2]);	
		return vn;	
	}	
	bool	operator==(const Vector3& v) 
	{	
		int	 i, e = 0;	
		for ( i=0; i<3; i++ )
			e += ( data[i] == v.data[i] );	
		return ( e == 3 );	
	}	
	bool	operator==(const double d) 
	{	
		int	 i, e = 0;	
		for ( i=0; i<3; i++ ) 
			e += ( data[i] == d );	
		return ( e == 3 );	
	}	
	bool	operator!=(const Vector3& v)
	{	
		int	 i, e = 0;	
		for ( i=0; i<3; i++ ) 
			e += ( data[i] == v.data[i] );	
		return ( e != 3 );	
	}	
	bool	operator>(const double d) {	
	int	 i, e = 0;	
	for ( i=0; i<3; i++ ) e += ( data[i] > d );	
	return ( e == 3 );	
	}	
	bool	operator<(const double d) {	
	int	 i, e = 0;	
	for ( i=0; i<3; i++ ) e += ( data[i] < d );	
	return ( e == 3 );	
	}	
	Type&	operator[](int i) { if ( i < 0 ) i = 0; if ( i > 2 ) i = 2; return data[i]; }	
	Type	x() { return data[0]; }	
	Type	y() { return data[1]; }	
	Type	z() { return data[2]; }	
	void	x(const double d) { data[0] = (Type)d; }	
	void	y(const double d) { data[1] = (Type)d; }	
	void	z(const double d) { data[2] = (Type)d; }	
	Vector3	Min(const Vector3& v) {	
	Vector3	 vn(*this);	
	for ( int i=0; i<3; i++ ) if ( vn.data[i] > v.data[i] ) vn.data[i] = v.data[i];	
	return vn;	
	}	
	Vector3	Max(const Vector3& v) {	
	Vector3	 vn(*this);	
	for ( int i=0; i<3; i++ ) if ( vn.data[i] < v.data[i] ) vn.data[i] = v.data[i];	
	return vn;	
	}	
	Vector3	abs() {	
	Vector3	 vn(*this);	
	for ( int i=0; i<3; i++ ) if ( data[i] < 0 ) vn.data[i] = -data[i];	
	return vn;	
	}	
	Vector3	floor(int places) {	
	Vector3	 vn;	
	for ( int i=0; i<3; i++ ) vn.data[i] = bfloor(data[i], places);	
	return vn;	
	}	
	Vector3	round(int places) {	
	Vector3	 vn;	
	for ( int i=0; i<3; i++ ) vn.data[i] = bround(data[i], places);	
	return vn;	
	}	
	Vector3	remainder(int divisor) {	
	Vector3	 vn;	
	for ( int i=0; i<3; i++ ) vn.data[i] = fmod(data[i], divisor);	
	return vn;	
	}	
	double	length2() { return (double)data[0]*data[0] + (double)data[1]*data[1] + (double)data[2]*data[2]; }	
	double	length() { return sqrt(length2()); }	
	double	distance(const Vector3& v) { return (*this - v).length(); }	
	double	distance2(const Vector3& v) { return (*this - v).length2(); }	
	Vector3	square_root() {	
	Vector3	 v;	
	for ( int i=0; i<3; i++ ) v.data[i] = sqrt(data[i]);	
	return v;	
	}	
	double	sum() { return (double)data[0] + (double)data[1] + (double)data[2]; }	
	double	volume() { return (double)data[0]*data[1]*data[2]; }	
	double	normalize() {	
	double	len = length();	
	if ( len < 1e-30 ) {	
	data[2] = 1;	
	} else {	
	double	div = 1.0/len;	
	for ( int i=0; i<3; i++ ) data[i] *= div;	
	}	
	return len;	
	}	
	double	scalar(const Vector3& v) {	
	return (double)data[0]*v.data[0] + (double)data[1]*v.data[1] + (double)data[2]*v.data[2];	
	}	
	Vector3	cross(const Vector3& v) {	
	Vector3	 vn((double)data[1]*v.data[2] - (double)data[2]*v.data[1],	
	(double)data[2]*v.data[0] - (double)data[0]*v.data[2],	
	(double)data[0]*v.data[1] - (double)data[1]*v.data[0]);	
	return vn;	
	}	
	double	angle(Vector3& v) {	
	Vector3	 v1(*this);	
	Vector3	 v2(v);	
	v1.normalize();	
	v2.normalize();	
	double	 prod = v1.scalar(v2);	
	if ( prod > 1 ) prod = 1;	
	if ( prod < -1 ) prod = -1;	
	return(acos(prod));	
	}	
	Vector3	normal(Vector3& v1, Vector3& v2) {	
	Vector3 edge1 = *this - v1;	
	Vector3 edge2 = *this - v2;	
	Vector3 normal = edge1.cross(edge2);	
	normal.normalize();	
	return(normal);	
	}	
	bool notfinite() {	
	int	 e = 0;	
	for ( int i=0; i<3; i++ ) e += finite(data[i]);	
	return ( e != 3 );	
	}	
	void print()
	{
		//cout << "x = " << data[0] << ", y = " << data[1] << ",z = " << data[2] << endl;
	}

	Vector3 transformDirection (GLdouble m[16] )
	{
		return Vector3 ( m [0]*data[0] + m [4]*data[1] + m [8]*data[3],
			              m [1]*data[0] + m [5]*data[1] + m [9]*data[3],
		                  m [2]*data[0] + m [6]*data[1] + m [0]*data[3]);
	}
} ;

typedef Vector3<float> Vector3f;