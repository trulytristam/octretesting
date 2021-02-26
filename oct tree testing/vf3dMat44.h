#pragma once

#include "olcPixelGameEngine.h"
#include <istream>
#include <ostream>
using namespace std;
struct vf3d {

	float x, y, z;

	float w = 1;

	vf3d() {}

	vf3d(float xx, float yy, float zz)
	{
		x = xx; y = yy; z = zz;
	}

	vf3d operator + (vf3d other) { return { x + other.x, y + other.y, z + other.z }; }
	vf3d operator - (vf3d other) { return { x - other.x, y - other.y, z - other.z }; }
	vf3d operator * (float other) { return { x * other, y * other, z * other }; }
	vf3d operator / (float other) { return { x / other, y / other, z / other }; }

	vf3d operator += (vf3d other) { return { x += other.x, y += other.y, z += other.z }; }
	vf3d operator -= (vf3d other) { return { x -= other.x, y -= other.y, z -= other.z }; }


	float dot(vf3d other) {
		return  x * other.x + y * other.y + z * other.z;
	}

	vf3d cross(vf3d o)
	{
		return {
			y * o.z - z * o.y,
			z * o.x - x * o.z,
			x * o.y - y * o.x
		};
	}

	float dist(vf3d other)
	{
		vf3d t = other - *this;

		return { sqrt((t.x * t.x) + (t.y * t.y) + (t.z * t.z)) };
	}



	float mag()
	{

		return { sqrt((x * x) + (y * y) + (z * z)) };
	}

	vf3d norm()
	{
		float t = mag();
		return { x / t,y / t,z / t };
	}



};

struct vf2d {

	float x, y;



	vf2d() {}

	vf2d(float xx, float yy)
	{
		x = xx; y = yy;
	}

	vf2d operator + (vf2d other) { return { x + other.x, y + other.y }; }
	vf2d operator - (vf2d other) { return { x - other.x, y - other.y }; }
	vf2d operator * (float other) { return { x * other, y * other }; }
	vf2d operator / (float other) { return { x / other, y / other }; }

	vf2d operator += (vf2d other) { return { x += other.x, y += other.y }; }
	vf2d operator -= (vf2d other) { return { x -= other.x, y -= other.y }; }


	float dot(vf2d other) {
		return  x * other.x + y * other.y;
	}



	float dist(vf2d other)
	{
		vf2d t = other - *this;

		return { sqrt((t.x * t.x) + (t.y * t.y)) };
	}



	float mag()
	{

		return { sqrt((x * x) + (y * y)) };
	}

	vf2d norm()
	{
		float t = mag();
		return { x / t,y / t};
	}



};
ostream& operator << (ostream& os, vf3d other)
{
	os << "x: " << other.x << " y: " << other.y << " z: " << other.z;

	return os;
}

struct mat44;
ostream& operator << (ostream& os, mat44 m);
struct mat44 {


	float m[4][4];

	mat44() {


	}


	static mat44 CreateIdentity()
	{
		mat44 temp;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (i == j) {
					temp.m[i][j] = 1;
				}
				else {
					temp.m[i][j] = 0;
				}
			}
		}
		
		return temp;
	}




	vf3d operator * (vf3d o)
	{



		vf3d temp;
		float* a = &temp.x;
		for (int i = 0; i < 4; i++)
		{

			*a = m[i][0] * o.x + m[i][1] * o.y + m[i][2] * o.z + m[i][3] * o.w;

			a++;
		}



		return temp;
	}

	mat44 operator * (mat44 o)
	{
		mat44 out;

		for (int i = 0; i < 4; i++)
		{

			for (int j = 0; j < 4; j++)
			{
				out.m[i][j] = m[i][0] * o.m[0][j] + m[i][1] * o.m[1][j] + m[i][2] * o.m[2][j] + m[i][3] * o.m[3][j];

			}
		}

		return out;
	}

	mat44 Invert()
	{
		mat44 o;

		o.m[0][0] = m[0][0];       o.m[0][1] = m[1][0];     o.m[0][2] = m[2][0];       o.m[0][3] = m[0][3];
		o.m[1][0] = m[0][1];       o.m[1][1] = m[1][1];     o.m[1][2] = m[2][1];      o.m[1][3] = m[1][3];
		o.m[2][0] = m[0][2];       o.m[2][1] = m[1][2];     o.m[2][2] = m[2][2];       o.m[2][3] = m[2][3];
		o.m[3][0] = m[3][0];       o.m[3][1] = m[3][1];     o.m[3][2] = m[3][2];       o.m[3][3] = m[3][3];
		return o;
	}


	static mat44 CreateXRot(float a)
	{
		mat44 o;

		o.m[0][0] = 1; o.m[0][1] = 0; o.m[0][2] = 0; o.m[0][3] = 0;
		o.m[1][0] = 0; o.m[1][1] = cos(a); o.m[1][2] = -sin(a); o.m[1][3] = 0;
		o.m[2][0] = 0; o.m[2][1] = sin(a); o.m[2][2] = cos(a); o.m[2][3] = 0;
		o.m[3][0] = 0; o.m[3][1] = 0; o.m[3][2] = 0; o.m[3][3] = 1;
		return o;
	}
	static mat44 CreateYRot(float a)
	{
		mat44 o;

		o.m[0][0] = cos(a); o.m[0][1] = 0; o.m[0][2] = sin(a); o.m[0][3] = 0;
		o.m[1][0] = 0; o.m[1][1] = 1; o.m[1][2] = 0; o.m[1][3] = 0;
		o.m[2][0] = -sin(a); o.m[2][1] = 0; o.m[2][2] = cos(a); o.m[2][3] = 0;
		o.m[3][0] = 0; o.m[3][1] = 0; o.m[3][2] = 0; o.m[3][3] = 1;
		return o;

	}
	static mat44 CreateZRot(float a)
	{
		mat44 o;

		o.m[0][0] = cos(a); o.m[0][1] = -sin(a); o.m[0][2] = 0; o.m[0][3] = 0;
		o.m[1][0] = sin(a); o.m[1][1] = cos(a); o.m[1][2] = 0; o.m[1][3] = 0;
		o.m[2][0] = 0; o.m[2][1] = 0; o.m[2][2] = 1; o.m[2][3] = 0;
		o.m[3][0] = 0; o.m[3][1] = 0; o.m[3][2] = 0; o.m[3][3] = 1;
		return o;
	}



	


	vf3d GetBasisX()
	{
		return { m[0][0],m[1][0],m[2][0] };
	}
	vf3d GetBasisY()
	{
		return { m[0][1],m[1][1],m[2][1] };
	}
	vf3d GetBasisZ()
	{
		return { m[0][2],m[1][2],m[2][2] };
	}




};

ostream& operator << (ostream& os, mat44 m)
{

	os << "---------------------------------------" << endl;
	for (int i = 0; i < 4; i++)
		os << "-- " << m.m[i][0] << "-- " << m.m[i][1] << "-- " << m.m[i][2] << "-- " << m.m[i][3] << endl;;
	os << "---------------------------------------" << endl;


	return os;
}