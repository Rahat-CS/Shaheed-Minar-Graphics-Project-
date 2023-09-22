
#include <GL/freeglut.h>
#include "Scene.h"
#include <array>
#include <iostream>
#include <string>
#include <numbers>

constexpr auto texture_count = 16;
extern std::array<unsigned int, texture_count> textures;

unsigned int ID[25];
int wired = 0;
int animat = 0;
const int nt = 40;				//number of slices along x-direction
const int ntheta = 100;

const int L = 5;
GLfloat ctrlpoints[L + 1][3] =
{
	{ 0.0, 0.0, 0.0},
	{ 0.0, 0.54, 0.0},
	{ 0.0, 1.01, 0.0},
	{ 0.54, 1.01, 0.0},
	{ 0.55, 0.55, 0.0},
	{ 0.55, 0.0, 0.0}
};

GLdouble modelview[16]; //var to hold the modelview info

namespace Scene {
	// Global Variables
	static GLfloat Speed = 1;
	// Key States
	static std::array<bool, 256> keyStates = { false };

	// Camera Variable
	static GLfloat cameraX = 3.5f;
	static GLfloat cameraY = 9.5f;
	static GLfloat cameraZ = 98.50f;

	static GLfloat lookX = 3.5f;
	static GLfloat lookY = 9.5f;
	static GLfloat lookZ = 48.5f;

	static GLfloat upX = 0.0f;
	static GLfloat upY = 1.0f;
	static GLfloat upZ = 0.0f;

	// Rotation around the axes
	static GLfloat angleX = 0.0f, angleY = 0.0f, angleZ = 0.0f;
	static GLfloat axisX = 0.0f, axisY = 0.0f, axisZ = 0.0f;
	static bool rotateX = false, rotateY = false, rotateZ = false;
	// Roll, Pitch, Yaw
	static GLdouble anglePitch = 90.0, angleYaw = 90.0, angleRoll = 0.0;

	// Lights Controls
	static bool light0 = true, light1 = false, light2 = false, light3 = true;
	static bool ambient = true, diffuse = true, specular = true;

	// Placements testing
	GLfloat x = 0.0f, y = 0.0f, z = 0.0f;

	static void GetNormal3f(
		GLfloat x1, GLfloat y1, GLfloat z1,
		GLfloat x2, GLfloat y2, GLfloat z2,
		GLfloat x3, GLfloat y3, GLfloat z3)
	{
		GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

		Ux = x2 - x1;
		Uy = y2 - y1;
		Uz = z2 - z1;

		Vx = x3 - x1;
		Vy = y3 - y1;
		Vz = z3 - z1;

		Nx = Uy * Vz - Uz * Vy;
		Ny = Uz * Vx - Ux * Vz;
		Nz = Ux * Vy - Uy * Vx;

		glNormal3f(Nx, Ny, Nz);
	}
	static void setNormal(
		GLfloat x1, GLfloat y1, GLfloat z1,
		GLfloat x2, GLfloat y2, GLfloat z2,
		GLfloat x3, GLfloat y3, GLfloat z3)
	{
		GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

		Ux = x2 - x1;
		Uy = y2 - y1;
		Uz = z2 - z1;

		Vx = x3 - x1;
		Vy = y3 - y1;
		Vz = z3 - z1;

		Nx = Uy * Vz - Uz * Vy;
		Ny = Uz * Vx - Ux * Vz;
		Nz = Ux * Vy - Uy * Vx;

		glNormal3f(-Nx, -Ny, -Nz);
	}
	static GLfloat quad_vertices[8][3] =
	{
		{-1.0, -1.0, -1.0},
		{ 1.0, -1.0, -1.0},
		{-1.0, -1.0,  1.0},
		{ 1.0, -1.0,  1.0},

		{-1.0,  1.0, -1.0},
		{ 1.0,  1.0, -1.0},
		{-1.0,  1.0,  1.0},
		{ 1.0,  1.0,  1.0}
	};

	static GLuint quad_indices[6][4] =
	{
		{0,2,3,1},
		{0,2,6,4},
		{2,3,7,6},
		{1,3,7,5},
		{1,5,4,0},
		{6,7,5,4}
	};

	// Draw Cube
	static void Cube(float R = 255, float G = 255, float B = 255, float alpha = 1)
	{
		float r = R / 255, g = G / 255, b = B / 255;

		GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
		GLfloat mat_ambient[] = { r, g, b, 1.0 };
		GLfloat mat_diffuse[] = { r, g, b, 1.0 };
		GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat mat_shininess[] = { 60 };

		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

		/*if (em)
		{
			glMaterialfv(GL_FRONT, GL_EMISSION, matEm);
		}
		else
		{
			glMaterialfv(GL_FRONT, GL_EMISSION, noMat);
		}*/



		glBegin(GL_QUADS);
		for (GLint i = 0; i < 6; i++)
		{
			GetNormal3f(
				quad_vertices[quad_indices[i][0]][0], quad_vertices[quad_indices[i][0]][1], quad_vertices[quad_indices[i][0]][2],
				quad_vertices[quad_indices[i][1]][0], quad_vertices[quad_indices[i][1]][1], quad_vertices[quad_indices[i][1]][2],
				quad_vertices[quad_indices[i][2]][0], quad_vertices[quad_indices[i][2]][1], quad_vertices[quad_indices[i][2]][2]
			);

			glVertex3fv(&quad_vertices[quad_indices[i][0]][0]); glTexCoord2f(0, 0);
			glVertex3fv(&quad_vertices[quad_indices[i][1]][0]); glTexCoord2f(0, 1);
			glVertex3fv(&quad_vertices[quad_indices[i][2]][0]); glTexCoord2f(1, 1);
			glVertex3fv(&quad_vertices[quad_indices[i][3]][0]); glTexCoord2f(1, 0);
		}
		glEnd();
	}

	static long long nCr(int n, int r)
	{
		if (r > n / 2) r = n - r; // because C(n, r) == C(n, n - r)
		long long ans = 1;
		int i;

		for (i = 1; i <= r; i++)
		{
			ans *= n - r + i;
			ans /= i;
		}

		return ans;
	}

	static void BezierCurve(double t, float xy[2])
	{
		double y = 0;
		double x = 0;
		t = t > 1.0 ? 1.0 : t;
		for (int i = 0; i <= L; i++)
		{
			int ncr = nCr(L, i);
			double oneMinusTpow = pow(1 - t, double(L - i));
			double tPow = pow(t, double(i));
			double coef = oneMinusTpow * tPow * ncr;
			x += coef * ctrlpoints[i][0];
			y += coef * ctrlpoints[i][1];

		}
		xy[0] = float(x);
		xy[1] = float(y);
	}

	static void MinarCircleBezier()
	{
		int i, j;
		float x, y, z, r;				//current coordinates
		float x1, y1, z1, r1;			//next coordinates
		float theta;

		const float startx = 0, endx = ctrlpoints[L][0];
		//number of angular slices
		const float dx = (endx - startx) / nt;	//x step size
		const float dtheta = 2 * 3.1416 / ntheta;		//angular step size

		float t = 0;
		float dt = 1.0 / nt;
		float xy[2];
		BezierCurve(t, xy);
		x = xy[0];
		r = xy[1];
		//rotate about z-axis
		float p1x = 0.0, p1y = 0.0, p1z = 0.0, p2x = 0.0, p2y = 0.0, p2z = 0.0;
		for (i = 0; i < nt; ++i)  			//step through x
		{
			theta = 0;
			t += dt;
			BezierCurve(t, xy);
			x1 = xy[0];
			r1 = xy[1];

			//draw the surface composed of quadrilaterals by sweeping theta
			glBegin(GL_QUAD_STRIP);
			for (j = 0; j <= ntheta; ++j)
			{
				theta += dtheta;
				double cosa = cos(theta);
				double sina = sin(theta);
				y = r * cosa;
				y1 = r1 * cosa;	//current and next y
				z = r * sina;
				z1 = r1 * sina;	//current and next z

				//edge from point at x to point at next x
				glVertex3f(x, y, z);

				if (j > 0)
				{
					setNormal(p1x, p1y, p1z, p2x, p2y, p2z, x, y, z);
				}
				else
				{
					p1x = x;
					p1y = y;
					p1z = z;
					p2x = x1;
					p2y = y1;
					p2z = z1;

				}
				glVertex3f(x1, y1, z1);

				//forms quad with next pair of points with incremented theta value
			}
			glEnd();
			x = x1;
			r = r1;
		} //for i
	}

	static void Curve()
	{
		const double t = glutGet(GLUT_ELAPSED_TIME) / 5000.0;
		const double a = t * 90.0;

		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (wired)
		{
			glPolygonMode(GL_FRONT, GL_LINE);
			glPolygonMode(GL_BACK, GL_LINE);

		}
		else
		{
			glPolygonMode(GL_FRONT, GL_FILL);
			glPolygonMode(GL_BACK, GL_FILL);
		}

		glPushMatrix();

		if (animat)
			glRotated(a, 0, 0, 1);

		//glRotatef(anglex, 1.0, 0.0, 0.0);
		//glRotatef(angley, 0.0, 1.0, 0.0);         	//rotate about y-axis
		//glRotatef(anglez, 0.0, 0.0, 1.0);

		glRotatef(90, 0.0, 0.0, 1.0);
		glTranslated(-3.5, 0, 0);
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //get the modelview info

		// void matColor(float kdr, float kdg, float kdb,  float shiny, int frnt_Back=0, float ambFactor=1.0, float specFactor=1.0);

		//  matColor(0.9,0.5,0.1,20);   // front face color
		// matColor(0.0,0.5,0.8,20,1);  // back face color

		glPushMatrix();
		// glRotatef(90,0,1,0);
		// glScalef(0.5,1,0.5);
		MinarCircleBezier();
		glPopMatrix();

		/*if (shcpt)
		{
			matColor(0.0, 0.0, 0.9, 20);
			showControlPoints();
		}*/

		glPopMatrix();
	}

	// Texture Enum to Texture ID
	enum Textures
	{
		earth,
		water,
		cloud_sky,
		green_grass,
		black_road,
		red_brick,
		white_wall,
		wood,
		floor,
		roof_tile,
		leaf
	};


	// Drawing Earth
	static void DrawEarth()
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[earth]);
		glPushMatrix();
		glScalef(1000, 1, 1000);
		Cube(139, 69, 19);  //165, 42, 42
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}

	static void Grass()
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[Textures::green_grass]);
		glPushMatrix();
		glScalef(250, 5, 250);
		Cube(0, 255, 0);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}

	static void Pond()
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[Textures::water]);
		glPushMatrix();
		glTranslatef(-355, 0, 0);
		glScalef(200, 5, 252);
		Cube(255, 255, 255);
		glPopMatrix();
	}

	static void Base()
	{
		// 1st Base
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[Textures::red_brick]);
		glPushMatrix();
		glTranslatef(-100, 7, -141.5);
		glScalef(100, 5, 150);
		Cube(128, 0, 0);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);

		// 2nd Base
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[Textures::red_brick]);
		glPushMatrix();
		glTranslatef(-98, 17, -141.5);
		glScalef(96, 5, 142);
		Cube(40, 40, 40);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);

		// 3rd Base
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[Textures::red_brick]);
		glPushMatrix();
		glTranslatef(-96, 27, -141.5);
		glScalef(90, 5, 132);
		Cube(128, 10, 10);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}

	static void Floors()
	{
		glPushMatrix();
		glTranslatef(16.5, 6.5, -165);
		Grass();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(325, 7.5, -667.5);
		Pond();
		glPopMatrix();

		glPushMatrix();											// Main Base 
		glTranslatef(90, 10, 0);
		glScalef(1, 1, 1.25);
		Base();
		glPopMatrix();
	}
	void circle_3D(GLdouble radius)
	{
		glEnable(GL_COLOR_MATERIAL);
		GLUquadric* qobj = gluNewQuadric();
		gluQuadricTexture(qobj, GL_TRUE);

		glRotatef(270, 1, 0, 0);
		glColor3f(0, 255, 0);
		gluSphere(qobj, radius, 20, 20);
		gluDeleteQuadric(qobj);
		glDisable(GL_COLOR_MATERIAL);

	}
	//r = r/255.0,g = g/255.0,b = b/255.0;
	// glColor3f(r,g,b);
	void cylinder_3D(GLdouble height, GLdouble rad, GLdouble rad2)
	{
		glEnable(GL_COLOR_MATERIAL);
		GLUquadric* qobj = gluNewQuadric();
		gluQuadricTexture(qobj, GL_TRUE);
		glRotatef(90, 1, 0, 0);
		glColor3f(205, 127, 50);
		gluCylinder(qobj, rad, rad2, height, 20, 20);
		gluDeleteQuadric(qobj);
		glDisable(GL_COLOR_MATERIAL);

	}
	void sub_tree()
	{

		glBindTexture(GL_TEXTURE_2D, ID[wood]);
		//glEnable(GL_TEXTURE_2D);
		//glBindTexture(GL_TEXTURE_2D, textures[wood]);
		glPushMatrix();
		glTranslatef(0, 40, 0);
		cylinder_3D(25, 1, 1);//base
		glPopMatrix();
		//glDisable(GL_TEXTURE_2D);

		//glBindTexture(GL_TEXTURE_2D, ID[leaf]);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[leaf]);
		glPushMatrix();
		glTranslatef(0, 40, 0);
		circle_3D(10); //leaf
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);

	}

	void sub_tree_upper()
	{
		glPushMatrix();
		glTranslatef(0, 90, 0);
		glRotatef(90, 0, 1, 0);
		glScalef(2, 1, 2);
		sub_tree();  //1
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 88, 0);
		glRotatef(10, 1, 0, 0);
		glScalef(2, 1, 2);
		sub_tree();  //1
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 88, 0);
		glRotatef(-10, 1, 0, 0);
		glScalef(2, 1, 2);
		sub_tree();  //1
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 90, 0);
		glRotatef(10, 0, 0, 1);
		glScalef(2, 1, 2);
		sub_tree();  //1
		glPopMatrix();


		glPushMatrix();
		glTranslatef(0, 90, 0);
		glRotatef(-10, 0, 0, 1);
		glScalef(2, 1, 2);
		sub_tree();  //1
		glPopMatrix();
	}
	void tree()
	{

		//11111111111111111111111111111111111111


		glPushMatrix();
		glTranslatef(0, 95, -8);
		glRotatef(55, 1, 0, 0);
		glScalef(0.7, 0.7, 0.7);
		sub_tree();  //1
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 95, 8);
		glRotatef(-55, 1, 0, 0);
		glScalef(0.7, 0.7, 0.7);
		sub_tree();  //1
		glPopMatrix();


		//3333333333333333333333333

		glPushMatrix();
		sub_tree_upper();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 10, 0);
		glScalef(0.8, 1, 0.8);
		sub_tree_upper();
		glPopMatrix();
		//glBindTexture(GL_TEXTURE_2D, ID[wood]);
		glPushMatrix();
		glTranslatef(0, 120, 0);
		cylinder_3D(80, 4, 8);  //tree base
		glPopMatrix();

	}


	static void Road()
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[black_road]);
		//glBindTexture(GL_TEXTURE_2D, ID[floor4]);
		glPushMatrix();
		glTranslatef(100,5 , -50);
		glScalef(75, 1, 35);
		Cube(165, 42, 42);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);

		//glBindTexture(GL_TEXTURE_2D, ID[white]);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[red_brick]);
		glPushMatrix();
		glTranslatef(100, 8, -50);
		glScalef(75, 1, 3);
		Cube(255, 255, 255);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);

		//glPushMatrix();
		//glTranslatef(180, -60, -150);
		//glScalef(1.5, 1.5, 1.5);
		//Tree();   //sohid minar tree
		//glPopMatrix();

		//glPushMatrix();
		//glTranslatef(180, -60, 130);
		//glScalef(1.5, 1.5, 1.5);
		//Tree();   //sohid minar tree2
		//glPopMatrix();
	}

	static void MinarBase()
	{
		//glBindTexture(GL_TEXTURE_2D, ID[white]);
		/*glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[white_wall]);*/
		glPushMatrix();							//minar Right
		glTranslatef(-70, 35, -15);
		glScalef(3, 60, 3);
		Cube(255, 255, 255);
		glPopMatrix();
		//glDisable(GL_TEXTURE_2D);

		glPushMatrix();							//minar Left
		glTranslatef(-70, 35, 15);
		glScalef(3, 60, 3);
		Cube(255, 255, 255);
		glPopMatrix();

		glPushMatrix();							//minar upper base
		glTranslatef(-70, 95, 0);
		glScalef(3, 2, 18);
		Cube(255, 255, 255);
		glPopMatrix();

		//glBindTexture(GL_TEXTURE_2D, ID[black]);
		glPushMatrix();							// Minar Middle Column (Right)
		glTranslatef(-68, 35, -2);
		glScalef(0.8, 59, 0.8);
		Cube(255, 255, 255);
		glPopMatrix();

		//glBindTexture(GL_TEXTURE_2D, ID[black]);
		glPushMatrix();							// Minar Middle Column (Left)
		glTranslatef(-68, 35, 6);
		glScalef(0.8, 59, 0.8);
		Cube(255, 255, 255);
		glPopMatrix();
	}

	static void MinarMid()
	{
		glPushMatrix();  //side_mid_base1
		glRotatef(25, 0, 1, 0);
		glTranslatef(0, 0, 12);
		glScalef(1, .9, .9);
		MinarBase();
		glPopMatrix();

		glPushMatrix();  //side_mid_base2
		glRotatef(-25, 0, 1, 0);
		glTranslatef(0, 0, -12);
		glScalef(1, .9, .9);
		MinarBase();
		glPopMatrix();
	}

	static void MinarSmall()
	{
		glPushMatrix(); //small_minar_base1
		glRotatef(-25, 0, 1, 0);
		glTranslatef(0, 0, -47);
		glScalef(1, .8, .8);
		MinarBase();
		glPopMatrix();

		glPushMatrix(); //small_minar_base2
		glRotatef(25, 0, 1, 0);
		glTranslatef(0, 0, 48);
		glScalef(1, .8, .8);
		MinarBase();
		glPopMatrix();
	}

	static void MinarUpperBase()
	{
		//glBindTexture(GL_TEXTURE_2D, ID[white]);

		glPushMatrix();  //minar base1
		glTranslatef(-70, 35, -15);
		glScalef(3, 30, 3);
		Cube(255, 255, 255);
		glPopMatrix();

		glPushMatrix();//minar base2
		glTranslatef(-70, 35, 15);
		glScalef(3, 30, 3);
		Cube(255, 255, 255);
		glPopMatrix();

		glPushMatrix();										//minar upper base
		glTranslatef(-70, 65, 0);
		glScalef(3, 2, 18);
		Cube(255, 255, 255);
		glPopMatrix();

		//  glBindTexture(GL_TEXTURE_2D, ID[black]);
		glPushMatrix();//minar thin_base1
		glTranslatef(-68, 35, -2);
		glScalef(0.8, 29, 0.8);
		Cube(255, 255, 255);
		glPopMatrix();

		//glBindTexture(GL_TEXTURE_2D, ID[black]);
		glPushMatrix();//minar thin_base1
		glTranslatef(-68, 35, 6);
		glScalef(0.8, 29, 0.8);
		Cube(255, 255, 255);
		glPopMatrix();

	}
	void minar_circle_Bezier()
	{
		int i, j;
		float x, y, z, r;				//current coordinates
		float x1, y1, z1, r1;			//next coordinates
		float theta;

		const float startx = 0, endx = ctrlpoints[L][0];
		//number of angular slices
		const float dx = (endx - startx) / nt;	//x step size
		const float dtheta = 2 * 3.14 / ntheta;		//angular step size

		float t = 0;
		float dt = 1.0 / nt;
		float xy[2];
		BezierCurve(t, xy);
		x = xy[0];
		r = xy[1];
		//rotate about z-axis
		float p1x, p1y, p1z, p2x, p2y, p2z;
		for (i = 0; i < nt; ++i)  			//step through x
		{
			theta = 0;
			t += dt;
			BezierCurve(t, xy);
			x1 = xy[0];
			r1 = xy[1];

			//draw the surface composed of quadrilaterals by sweeping theta
			glBegin(GL_QUAD_STRIP);
			for (j = 0; j <= ntheta; ++j)
			{
				theta += dtheta;
				double cosa = cos(theta);
				double sina = sin(theta);
				y = r * cosa;
				y1 = r1 * cosa;	//current and next y
				z = r * sina;
				z1 = r1 * sina;	//current and next z

				//edge from point at x to point at next x
				glVertex3f(x, y, z);

				if (j > 0)
				{
					setNormal(p1x, p1y, p1z, p2x, p2y, p2z, x, y, z);
				}
				else
				{
					p1x = x;
					p1y = y;
					p1z = z;
					p2x = x1;
					p2y = y1;
					p2z = z1;

				}
				glVertex3f(x1, y1, z1);

				//forms quad with next pair of points with incremented theta value
			}
			glEnd();
			x = x1;
			r = r1;
		} //for i

	}


	void curve()
	{
		const double t = glutGet(GLUT_ELAPSED_TIME) / 5000.0;
		const double a = t * 90.0;

		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (wired)
		{
			glPolygonMode(GL_FRONT, GL_LINE);
			glPolygonMode(GL_BACK, GL_LINE);

		}
		else
		{
			glPolygonMode(GL_FRONT, GL_FILL);
			glPolygonMode(GL_BACK, GL_FILL);
		}

		glPushMatrix();

		if (animat)
			glRotated(a, 0, 0, 1);


		glRotatef(90, 0.0, 0.0, 1.0);
		glTranslated(-3.5, 0, 0);
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //get the modelview info

		// void matColor(float kdr, float kdg, float kdb,  float shiny, int frnt_Back=0, float ambFactor=1.0, float specFactor=1.0);

		//  matColor(0.9,0.5,0.1,20);   // front face color
		// matColor(0.0,0.5,0.8,20,1);  // back face color

		glPushMatrix();



		// glRotatef(90,0,1,0);
		// glScalef(0.5,1,0.5);
		minar_circle_Bezier();
		glPopMatrix();






	
		glPopMatrix();

	}

	static void MinarUpper()
	{
		glPushMatrix();
		glRotatef(-45, 0, 0, 1);
		glTranslatef(-50, 10, 0);
		MinarUpperBase();
		glPopMatrix();
	}

	static void Minar()
	{
		glPushMatrix();
		glScalef(1, 1, 1.5);
		MinarBase();
		glPopMatrix();

		glPushMatrix();
		glScalef(1, 1, 1.5);
		MinarUpper();
		glPopMatrix();


		glPushMatrix();
		glScalef(1, 1, 1.5);
		MinarMid();
		glPopMatrix();

		glPushMatrix();
		glScalef(1, 1, 1.5);
		MinarSmall();
		glPopMatrix();

		glEnable(GL_COLOR_MATERIAL);
		//glBindTexture(GL_TEXTURE_2D, ID[red]);
		glPushMatrix();
		glColor3f(1, 0, 0);
		glTranslatef(-130, 90, 0);
		glRotatef(85, 0, 0, 1);
		glScalef(95, 5, 95);               //curve-------------------------
		curve();
		glPopMatrix();
		glColor3f(1, 1, 1);
		glDisable(GL_COLOR_MATERIAL);
	}

	static void ShahidMinar()
	{
		glPushMatrix();
		Floors();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(7.5, 67.5, -178);
		Minar();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(60, 6.7 , -130);
		glScalef(1.22, 1, 1);
		Road();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(351.5, 1, 750);
		glRotatef(90, 0, 1, 0);
		glScalef(10, 1, 1);
		Road();
		glPopMatrix();

		/*glPushMatrix();
		glTranslatef(-10, 0, 0);
		FlowerStage();
		glPopMatrix();*/
	}
	static GLfloat cube[8][3] =
	{
		{-1.0, -1.0, 1.0},
		{1.0, -1.0, 1.0},
		{1.0, 1.0, 1.0},
		{-1.0, 1.0, 1.0},


		{-1.0, -1.0, -1.0},
		{1.0, -1.0, -1.0},
		{1.0, 1.0, -1.0},
		{-1.0, 1.0, -1.0},
	};
	static GLubyte quadIndices[6][4] =
	{
		{0,1,2,3},
		{7,6,5,4},
		{2,6,7,3},

		{0,4,5,1},
		{2,1,5,6},
		{7,4,0,3},
	};

	static void DrawCube(GLint c1, GLint c2, GLint c3, GLboolean emission = false)
	{
		GLfloat r = c1 / 255.0;
		GLfloat g = c2 / 255.0;
		GLfloat b = c3 / 255.0;

		GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
		GLfloat mat_ambient[] = { r, g, b, 1.0 };
		GLfloat mat_diffuse[] = { r, g, b, 1.0 };
		GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat mat_shininess[] = { 60 };

		GLfloat mat_em[] = { r,g,b,1.0 };

		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

		if (emission) glMaterialfv(GL_FRONT, GL_EMISSION, mat_em);
		else glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

		for (GLint i = 0; i < 6; i++)
			for (GLint i = 0; i < 6; i++)
			{
				glBegin(GL_QUADS);

				glVertex3fv(&cube[quadIndices[i][0]][0]);
				glTexCoord2f(1, 1);
				glVertex3fv(&cube[quadIndices[i][1]][0]);
				glTexCoord2f(1, 0);
				glVertex3fv(&cube[quadIndices[i][2]][0]);
				glTexCoord2f(0, 0);
				glVertex3fv(&cube[quadIndices[i][3]][0]);
				glTexCoord2f(0, 1);
				glEnd();
			}
	}

	static void Cylinder(GLint c1, GLint c2, GLint c3, GLboolean emission = false)
	{

		GLfloat r = c1 / 255.0;
		GLfloat g = c2 / 255.0;
		GLfloat b = c3 / 255.0;

		GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
		GLfloat mat_ambient[] = { r, g, b, 1.0 };
		GLfloat mat_diffuse[] = { r, g, b, 1.0 };
		GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat mat_shininess[] = { 60 };

		GLfloat mat_em[] = { r,g,b,1.0 };

		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

		if (emission) glMaterialfv(GL_FRONT, GL_EMISSION, mat_em);
		else glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

		const double PI = 3.14159;

		/* top triangle */
		double i, resolution = 0.1;
		double height = 1;
		double radius = 0.5;

		glPushMatrix();
		glTranslatef(0, -0.5, 0);
		//top Circle
		glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(0.5, 0.5);
		glVertex3f(0, height, 0);  /* center */
		for (i = 2 * PI; i >= 0; i -= resolution)
		{
			glTexCoord2f(0.5f * cos(i) + 0.5f, 0.5f * sin(i) + 0.5f);
			glVertex3f(radius * cos(i), height, radius * sin(i));
		}
		/* close the loop back to 0 degrees */
		glTexCoord2f(0.5, 0.5);
		glVertex3f(radius, height, 0);
		glEnd();

		//bottom Circle
		glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(0.5, 0.5);
		glVertex3f(0, 0, 0);  /* center */
		for (i = 0; i <= 2 * PI; i += resolution)
		{
			glTexCoord2f(0.5f * cos(i) + 0.5f, 0.5f * sin(i) + 0.5f);
			glVertex3f(radius * cos(i), 0, radius * sin(i));
		}
		glEnd();

		//cylinder side
		glBegin(GL_QUAD_STRIP);
		for (i = 0; i <= 2 * PI; i += resolution)
		{
			const float tc = (i / (float)(2 * PI));
			glTexCoord2f(tc, 0.0);
			glVertex3f(radius * cos(i), 0, radius * sin(i));
			glTexCoord2f(tc, 1.0);
			glVertex3f(radius * cos(i), height, radius * sin(i));
		}
		/* close the loop back to zero degrees */
		glTexCoord2f(0.0, 0.0);
		glVertex3f(radius, 0, 0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(radius, height, 0);
		glEnd();

		glPopMatrix();
	}

	static void Bed()
	{
		//bed
		//bed floor
		glPushMatrix();
		glTranslatef(0, 5, 0);
		glScalef(5, .2, 10);
		DrawCube(200, 75, 75);
		glPopMatrix();

		//bed uporer wall
		glPushMatrix();
		glTranslatef(0, 6, -9.9);
		glScalef(5, 1.2, .1);
		DrawCube(200, 75, 75);
		glPopMatrix();

		//bed nicher wall
		glPushMatrix();
		glTranslatef(0, 6, 9.9);
		glScalef(5, .8, .1);
		DrawCube(200, 75, 75);
		glPopMatrix();
		//bed nicher paa
		glPushMatrix();
		glTranslatef(4, 3.1, 9);
		glScalef(.5, 2, .5);
		DrawCube(255, 255, 255);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-4, 3.1, 9);
		glScalef(.5, 2, .5);
		DrawCube(255, 255, 255);
		glPopMatrix();

		//bed uporer paa
		glPushMatrix();
		glTranslatef(4, 3.1, -8);
		glScalef(.5, 2, .5);
		DrawCube(255, 255, 255);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-4, 3.1, -8);
		glScalef(.5, 2, .5);
		DrawCube(255, 255, 255);
		glPopMatrix();

		//bed uporer mattress
		glPushMatrix();
		glTranslatef(0, 5.6, .5);
		glScalef(4.7, .75, 9.3);
		DrawCube(255, 255, 255);
		glPopMatrix();


		//blanket
		glPushMatrix();
		glTranslatef(0, 5.6 + .75 + .2, 7);
		glScalef(4.3, .4, 2);

		DrawCube(255, 255, 0);
		glPopMatrix();

		//pillow
		glPushMatrix();
		glTranslatef(-2.1, 5.6 + .75 + .2, -7);
		glScalef(1.9, .5, 2);
		DrawCube(255, 0, 255);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(2.1, 5.6 + .75 + .2, -7);
		glScalef(1.9, .5, 2);
		DrawCube(255, 0, 255);
		glPopMatrix();
	}

	static void Almirah()
	{
		glPushMatrix();
		glScalef(2, 2, 3.6);
		glTranslatef(-8.99, 1, 3.9);
		DrawCube(255, 255, 255);
		glPopMatrix();

		glPushMatrix();
		glScalef(1.5, 6, 3.5);
		glTranslatef(-12.35, 1.5, 4);
		DrawCube(255, 255, 255);
		glPopMatrix();

		glPushMatrix();
		glScalef(2, .5, 3.7);
		glTranslatef(-9.0, 30, 3.77);
		DrawCube(255, 255, 255);
		glPopMatrix();

		//details for racks
		glPushMatrix();
		glScalef(.2, 1.5, 1.3);
		glTranslatef(-16 / .2, 1.4, 12);
		DrawCube(200, 162, 81);
		glPopMatrix();

		glPushMatrix();
		glScalef(.2, 1.5, 1.3);
		glTranslatef(-16 / .2, 1.4, 9.75);
		DrawCube(200, 162, 81);
		glPopMatrix();

		//strips for racks
		glPushMatrix();
		glTranslatef(-17, 8, 14);
		glScalef(.1, 6, .1);
		DrawCube(200, 180, 161);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-17, 6, 14);
		glRotatef(90, 1, 0, 0);
		glScalef(.1, 3.4, .1);
		DrawCube(200, 180, 161);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-17, 8, 14);
		glRotatef(90, 1, 0, 0);
		glScalef(.1, 3.4, .1);
		DrawCube(200, 180, 161);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-17, 10, 14);
		glRotatef(90, 1, 0, 0);
		glScalef(.1, 3.4, .1);
		DrawCube(200, 180, 161);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-17, 12, 14);
		glRotatef(90, 1, 0, 0);
		glScalef(.1, 3.4, .1);
		DrawCube(200, 180, 161);
		glPopMatrix();

		//handles
		glPushMatrix();
		glTranslatef(-15.75, 2.5, 13.5);
		glRotatef(90, 0, 0, 1);
		glScalef(.8, .15, .25);
		DrawCube(200, 200, 200);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-15.75, 2.5, 14.75);
		glRotatef(90, 0, 0, 1);
		glScalef(.8, .15, .25);
		DrawCube(200, 200, 200);
		glPopMatrix();
	}

	static void WindowFrame()
	{
		//windowframe
		glPushMatrix();
		glTranslatef(0, 10, -20);
		glRotatef(180, 0, 1, 0);
		glScalef(.5, 5, .1);
		DrawCube(173, 123, 7);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 4 * 2.5, -20);
		glRotatef(180, 0, 1, 0);
		glScalef(4, 0.5, .1);
		DrawCube(173, 123, 7);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-4, 10, -20);
		glRotatef(180, 0, 1, 0);
		glScalef(.5, 5, .3);
		DrawCube(173, 123, 7);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(4, 10, -20);
		glRotatef(180, 0, 1, 0);
		glScalef(.5, 5, .3);
		DrawCube(173, 123, 7);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 5 * 3, -20);
		glRotatef(180, 0, 1, 0);
		glScalef(4.9, 0.5, .3);
		DrawCube(173, 123, 7);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 1.5 * 3, -20);
		glRotatef(180, 0, 1, 0);
		glScalef(4.9, 0.5, .3);
		DrawCube(173, 123, 7);
		glPopMatrix();
	}

	static void Door()
	{
		//door
		glPushMatrix();
		glTranslatef(0, 7, 20);
		glScalef(4, 7, .3);
		DrawCube(183, 159, 127);
		glPopMatrix();
	}

	static void Corridor()
	{
		glPushMatrix();
		glTranslatef(-4, 20, 25.2);
		glScalef(66, .6, 5);
		DrawCube(220, 220, 0);
		glPopMatrix();
	}

	static void Roof()
	{

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[roof_tile]);
		glPushMatrix();
		glTranslatef(0, 40.2, 1);
		glScalef(64, .4, 22);
		DrawCube(166, 166, 166);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[roof_tile]);
		glPushMatrix();
		glTranslatef(-5, 41, 5);
		glScalef(70, .6, 30);
		DrawCube(66, 66, 66);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}

	static void FloorBase()
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[floor]);
		glPushMatrix();
		glTranslatef(-5, 0, 5);
		glScalef(66, .5, 30);
		DrawCube(255, 255, 255);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);

		glPushMatrix();
		glTranslatef(-5, -5.5, 5);
		glScalef(70, 5, 33);
		DrawCube(55, 55, 55);
		glPopMatrix();



		//ground stairs
		glPushMatrix();
		glTranslatef(0, 0, 2);

		glPushMatrix();
		glTranslatef(0, -9.9, 48);
		glScalef(10, .4, 3);
		DrawCube(255, 255, 255);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-0, -8.5, 45);
		glScalef(10, 1, 3);
		DrawCube(0, 0, 0);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-0, -6.5, 42);
		glScalef(10, 1, 3);
		DrawCube(255, 255, 255);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-0, -4.5, 39);
		glScalef(10, 1, 3);
		DrawCube(0, 0, 0);
		glPopMatrix();
		glPopMatrix();
	}

	static void DrawRoom()
	{
		///frontwalls
		//front leftwall
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[red_brick]);
		glPushMatrix();
		glTranslatef(-12, 10, 20);
		glScalef(8, 10, .2);
		DrawCube(149, 146, 140);
		glPopMatrix();
		//front rightwall
		glPushMatrix();
		glTranslatef(12, 10, 20);
		glScalef(8, 10, .2);
		DrawCube(149, 146, 140);
		glPopMatrix();

		//front topwall
		glPushMatrix();
		glTranslatef(0, 17, 20);
		glScalef(4, 3, .2);
		DrawCube(149, 146, 140);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);


		///front Door
		//door frame
		glPushMatrix();
		glTranslatef(4.1, 7, 20);
		glScalef(.5, 7, .3);
		DrawCube(173, 123, 7);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-4.1, 7, 20);
		glScalef(.5, 7, .3);
		DrawCube(173, 123, 7);
		glPopMatrix();
		//doorframe top bar
		glPushMatrix();
		glTranslatef(0, 14, 20);
		glScalef(4.8, .5, .3);
		DrawCube(173, 123, 7);
		glPopMatrix();


		//door
		glPushMatrix();
		glTranslatef(-4, 7, 20);
		//glRotatef(door_theta, 0, 1, 0);
		glTranslatef(4, -7, -20);
		Door();
		glPopMatrix();

		//ceil
		glPushMatrix();
		glTranslatef(0, 20, 0);
		glScalef(20, 0.4, 20);
		DrawCube(168, 168, 168);
		glPopMatrix();

		//ceil frame
	//    glPushMatrix();
	//    glTranslatef(0, 20, 20.2);
	//    glScalef(22, .5, .2);
	//    drawcube(25,25,25);
	//    glPopMatrix();


		//room frame
		glPushMatrix();
		glTranslatef(18, 10, 20);
		glScalef(.2, 10, .3);
		DrawCube(30, 30, 30);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-18, 10, 20);
		glScalef(.2, 10, .3);
		DrawCube(30, 30, 30);
		glPopMatrix();

		//floor
		glPushMatrix();
		glScalef(20, 0.1, 20);
		DrawCube(168, 168, 168);
		glPopMatrix();

		//tiles
		glPushMatrix();
		for (GLint hCount = 1; hCount < 40; hCount += 4)
		{
			for (GLint vCount = 1; vCount < 40; vCount += 4)
			{
				if ((hCount + vCount) % 2 == 0)
				{
					glPushMatrix();
					glTranslatef(-19 + hCount, 0.1, -19 + vCount);
					glScalef(1.5, 0.1, 1.5);

					DrawCube(25, 25, 25);
					glPopMatrix();
				}
			}
		}
		glPopMatrix();

		//left wall
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[white_wall]);
		glPushMatrix();
		glTranslatef(-20, 10, 0);
		glScalef(.1, 10, 20);
		DrawCube(104, 139, 153);
		glPopMatrix();

		//right wall
		glPushMatrix();
		glTranslatef(20, 10, 0);
		glScalef(.1, 10, 20);
		DrawCube(104, 139, 153);
		glPopMatrix();

		//back wall left
		glPushMatrix();
		glTranslatef(-12, 10, -20);
		glRotatef(180, 0, 1, 0);
		glScalef(8, 10, .1);
		DrawCube(104 - 25, 139 - 25, 153 - 25);
		glPopMatrix();

		//backwall right
		glPushMatrix();
		glTranslatef(12, 10, -20);
		glRotatef(180, 0, 1, 0);
		glScalef(8, 10, .1);
		DrawCube(104 - 25, 139 - 25, 153 - 25);
		glPopMatrix();


		//backwall bottom
		glPushMatrix();
		glTranslatef(0, 2.5, -20);
		glRotatef(180, 0, 1, 0);
		glScalef(4, 2.5, .1);
		DrawCube(104 - 25, 139 - 25, 153 - 25);
		glPopMatrix();

		//backwall top
		glPushMatrix();
		glTranslatef(0, 17.5, -20);
		glRotatef(180, 0, 1, 0);
		glScalef(4, 2.5, .1);
		DrawCube(104 - 25, 139 - 25, 153 - 25);
		glPopMatrix();

		glPushMatrix();
		WindowFrame();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);

		//bathroom
		glPushMatrix();
		glTranslatef(-10, 10, -10);
		glScalef(0.2, 10, 10);
		DrawCube(200, 200, 125);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-15, 10, 0);
		glScalef(5, 10, .2);
		DrawCube(200, 200, 125);
		glPopMatrix();

		//bathroom door
		glPushMatrix();
		glTranslatef(-15, 6, 0.1);
		glScalef(3, 6, .2);
		DrawCube(200, 75, 75);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-13, 6, 0.1);
		glScalef(.2, .5, .3);
		DrawCube(100, 37, 37);
		glPopMatrix();

		//bed
		glPushMatrix();
		glTranslatef(14, -1.1, -3);
		Bed();
		glPopMatrix();

		//almirah
		glPushMatrix();
		Almirah();
		glPopMatrix();
	}

	static void FirstFloor()
	{
		glPushMatrix();
		DrawRoom();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(40, 0, 0);
		DrawRoom();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-40, 0, 0);
		DrawRoom();
		glPopMatrix();

		//base
		glPushMatrix();
		FloorBase();
		glPopMatrix();

		//ground
	}

	static void SecondFloor()
	{
		//rooms
		glPushMatrix();
		glTranslatef(0, 20, 0);
		DrawRoom();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(40, 20, 0);
		DrawRoom();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-40, 20, 0);
		DrawRoom();
		glPopMatrix();

		//corridor
		glPushMatrix();
		Corridor();
		glPopMatrix();

		//roof
		glPushMatrix();
		Roof();
		glPopMatrix();
	}

	static void Pillars()
	{
		glPushMatrix();
		glTranslatef(20, 20.2, 28.5);
		glScalef(3, 40, 3);
		Cylinder(188, 188, 188);
		glPopMatrix();


		glPushMatrix();
		glTranslatef(-20, 20.2, 28.5);
		glScalef(3, 40, 3);
		Cylinder(188, 188, 188);
		glPopMatrix();


		glPushMatrix();
		glTranslatef(-60, 20.2, 28.5);
		glScalef(3, 40, 3);
		Cylinder(188, 188, 188);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(60, 20.2, 28.5);
		glScalef(3, 40, 3);
		Cylinder(188, 188, 188);
		glPopMatrix();
	}

	static void Stair()
	{
		//halfway base
		glPushMatrix();
		glTranslatef(-65, 10, -16);
		glScalef(7, .5, 4);
		DrawCube(244, 201, 105);
		glPopMatrix();

		//stairs top
		glPushMatrix();
		glTranslatef(-61.5, 10.5, -11);
		glScalef(3.5, .75, 2.5);
		DrawCube(255, 255, 255);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-61.5, 11.5, -7);
		glScalef(3.5, .75, 2.5);
		DrawCube(255, 255, 255);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-61.5, 12.5, -3);
		glScalef(3.5, .75, 2.5);
		DrawCube(255, 255, 255);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-61.5, 13.5, 1);
		glScalef(3.5, .75, 2.5);
		DrawCube(255, 255, 255);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-61.5, 14.5, 5);
		glScalef(3.5, .75, 2.5);
		DrawCube(255, 255, 255);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-61.5, 15.5, 9);
		glScalef(3.5, .75, 2.5);
		DrawCube(255, 255, 255);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-61.5, 16.5, 13);
		glScalef(3.5, .75, 2.5);
		DrawCube(255, 255, 255);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-61.5, 17.5, 17);
		glScalef(3.5, .75, 2.5);
		DrawCube(255, 255, 255);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-61.5, 19, 19);
		glScalef(3.5, .75, 2.5);
		DrawCube(255, 255, 255);
		glPopMatrix();

		//stairs bottom
		glPushMatrix();
		glTranslatef(-68.5, 9.5, -11);
		glScalef(3.5, .75, 2.5);
		DrawCube(255, 255, 255);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-68.5, 8.5, -7);
		glScalef(3.5, .75, 2.5);
		DrawCube(255, 255, 255);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-68.5, 7.5, -3);
		glScalef(3.5, .75, 2.5);
		DrawCube(255, 255, 255);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-68.5, 6.5, 1);
		glScalef(3.5, .75, 2.5);
		DrawCube(255, 255, 255);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-68.5, 5.5, 5);
		glScalef(3.5, .75, 2.5);
		DrawCube(255, 255, 255);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-68.5, 4.5, 9);
		glScalef(3.5, .75, 2.5);
		DrawCube(255, 255, 255);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-68.5, 3.5, 13);
		glScalef(3.5, .75, 2.5);
		DrawCube(255, 255, 255);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-68.5, 2.5, 17);
		glScalef(3.5, .75, 2.5);
		DrawCube(255, 255, 255);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-68.5, 1.5, 21);
		glScalef(3.5, .75, 2.5);
		DrawCube(255, 255, 255);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-68.5, 0.5, 25);
		glScalef(3.5, .75, 2.5);
		DrawCube(255, 255, 255);
		glPopMatrix();

	
	}

	static void FirstFloorFence()
	{
		//horizontal lines
		glPushMatrix();
		glTranslatef(-5.5, 27, 29);
		glScalef(64, .4, .4);
		DrawCube(50, 50, 50);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(58, 27, 24.5);
		glScalef(.4, .4, 4.5);
		DrawCube(50, 50, 50);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-68, 27, 24.5);
		glScalef(.4, .4, 4.5);
		DrawCube(50, 50, 50);
		glPopMatrix();

		//vertical lines
		for (GLint i = 0; i < 64; i++)
		{
			glPushMatrix();
			glTranslatef(58 - i * 2, 23.5, 29);
			glScalef(.4, 3.2, .4);
			DrawCube(255, 255, 255);
			glPopMatrix();
		}
		//fence towards z axis right
		for (GLint i = 1; i < 5; i++)
		{
			glPushMatrix();
			glTranslatef(58, 23.5, 29 - i * 2);
			glScalef(.4, 3.2, .4);
			DrawCube(255, 255, 255);
			glPopMatrix();
		}
		//fence towards z axis left
		for (GLint i = 1; i < 5; i++)
		{
			glPushMatrix();
			glTranslatef(-68, 23.5, 29 - i * 2);
			glScalef(.4, 3.2, .4);
			DrawCube(255, 255, 255);
			glPopMatrix();
		}
	}

	static void GroundFence()
	{
		//horizontal lines
		//right
		glPushMatrix();
		glTranslatef(40.5, 7, 29);
		glScalef(20, .4, .4);
		DrawCube(50, 50, 50);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(58, 7, 24.5);
		glScalef(.4, .4, 4.5);
		DrawCube(50, 50, 50);
		glPopMatrix();

		//left
		glPushMatrix();
		glTranslatef(-45.5, 7, 29);
		glScalef(25, .4, .4);
		DrawCube(50, 50, 50);
		glPopMatrix();

		//vertical fences
		//right
		for (GLint i = 0; i < 20; i++)
		{
			glPushMatrix();
			glTranslatef(58 - i * 2, 3.5, 29);
			glScalef(.4, 3.2, .4);
			DrawCube(255, 255, 255);
			glPopMatrix();
		}
		//left
		for (GLint i = 40; i < 65; i++)
		{
			glPushMatrix();
			glTranslatef(58 - i * 2, 3.5, 29);
			glScalef(.4, 3.2, .4);
			DrawCube(255, 255, 255);
			glPopMatrix();
		}

		//fence towards z axis right
		for (GLint i = 1; i < 5; i++)
		{
			glPushMatrix();
			glTranslatef(58, 3.5, 29 - i * 2);
			glScalef(.4, 3.2, .4);
			DrawCube(255, 255, 255);
			glPopMatrix();
		}
	}

	static void StairFence()
	{
		for (GLint i = 0; i < 14; i++)
		{
			glPushMatrix();
			glTranslatef(-70, 3.5 + i * .75, 25 - i * 3);
			glScalef(.4, 3.2, .4);
			DrawCube(0, 255, 255);
			glPopMatrix();
		}

		glPushMatrix();
		glTranslatef(-70, 3.5 + 13 * .75, 25 - 14 * 3 + 1);
		glScalef(.4, 3.2, .4);
		DrawCube(0, 255, 255);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-70, 3.5 + 13 * .75, 25 - 14 * 3 + 1 - 2);
		glScalef(.4, 3.2, .4);
		DrawCube(0, 255, 255);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-70, 3.5 + 13 * .75, 25 - 14 * 3 + 1 - 3.5);
		glScalef(.4, 3.2, .4);
		DrawCube(0, 255, 255);
		glPopMatrix();

		for (GLint i = 0; i < 5; i++)
		{
			glPushMatrix();
			glTranslatef(-70 + i * 2, 3.5 + 13 * .75, 25 - 14 * 3 + 1 - 3.5);
			glScalef(.4, 3.2, .4);
			DrawCube(0, 255, 255);
			glPopMatrix();
		}

		//inclined line
		glPushMatrix();
		glTranslatef(-70, 11.5, 7);
		glRotatef(13.5, 1, 0, 0);
		glScalef(.6, .6, 20);
		DrawCube(50, 50, 50);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-70, 7, 27.4);
		glScalef(.6, .6, 2);
		DrawCube(40, 40, 0);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-70, 16, -15.8);
		glScalef(.6, .6, 4.5);
		DrawCube(40, 40, 40);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-65.0002, 16, -19.4);
		glScalef(5, .6, .6);
		DrawCube(50, 50, 50);
		glPopMatrix();
	}

	static void Building()
	{
		glPushMatrix();
		FirstFloor();
		SecondFloor();
		Pillars();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-13.5, 0, 0);
		glScalef(.8, 1, 1);
		Stair();
		glPopMatrix();

		glPushMatrix();
		FirstFloorFence();
		glPopMatrix();

		glPushMatrix();
		GroundFence();
		glPopMatrix();

		glPushMatrix();
		StairFence();
		glPopMatrix();

		/*glPushMatrix();
		sky();
		glPopMatrix();

		glPushMatrix();
		ground();
		glPopMatrix();*/
	}

	static void Buildings()
	{
		glPushMatrix();
		glTranslatef(-75, 0, 0);
		glRotatef(40, 0, 1, 0);
		Building();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(75, 0, 0);
		glRotatef(-40, 0, 1, 0);
		glScalef(-1, 1, 1);
		Building();
		glPopMatrix();
	}
	static void Sky()
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[Textures::cloud_sky]);
		glPushMatrix();
		glTranslatef(0, 1000, 0);
		glScalef(100000, 0, 100000);
		Cube(135, 206, 235);
		Cube();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}

	static void Hall()
	{
		//glPushMatrix(); //hall tree
		//glTranslatef(40, -80, 300);
		//glScalef(1.5, 1.5, 1.5);
		//tree();
		//glPopMatrix();
		//
		//glPushMatrix(); //hall tree
		//glTranslatef(40, -80, 360);
		//glScalef(1.5, 1.5, 1.5);
		//tree();
		//glPopMatrix();

		//glPushMatrix(); //hall tree
		//glTranslatef(90, -80, 300);
		//glScalef(1.5, 1.5, 1.5);
		//tree();
		//glPopMatrix();

		//glPushMatrix(); //hall tree2
		//glTranslatef(420, -80, 350);
		//glScalef(1.5, 1.5, 1.5);
		//tree();
		//glPopMatrix();

		//glPushMatrix(); //hall tree2
		//glTranslatef(460, -80, 300);
		//glScalef(1.5, 1.5, 1.5);
		//tree();
		//glPopMatrix();

		glPushMatrix();
		glTranslatef(135.5 + 116.5, 31.5, 173 + 224);
		glRotatef(225, 0, 1, 0);
		glScalef(2, 2, 2);
		Buildings();
		glPopMatrix();
		
		glPushMatrix();
		glTranslatef(-88.5, 4, -126.5);
		Road();
		glPopMatrix();
		glPushMatrix();
		//glBindTexture(GL_TEXTURE_2D, ID[green-grass]);
		glTranslatef(262.5, 0, 180);
		glScalef(325, 6, 475); //floor of the hall
		Cube(0, 255, 0);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(182.5, 0.5, 578.5);
		glRotatef(90, 0, 1, 0);
		glScalef(5, 1, 1);
		Road();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(270, -60, 190);
		glScalef(1.5, 1.5, 1.5);
		tree();   //field tree
		glPopMatrix();

		glPushMatrix();
		glTranslatef(291, -60, -150);
		glScalef(1.5, 1.5, 1.5);
		tree();   //field tree
		glPopMatrix();



		glPushMatrix();
		glTranslatef(291, -60, 30);
		glScalef(1.5, 1.5, 1.5);
		tree();   //field tree
		glPopMatrix();
		//downs side tree
		glPushMatrix();
		glTranslatef(-270, -60, 210);
		glScalef(1.5, 1.5, 1.5);
		tree();   //field tree
		glPopMatrix();

	}
	void goal_bar()
	{
		glPushMatrix();  //goal bar1
		glTranslatef(0, 0, -25);
		glScalef(2, 25, 2);
		Cube(255, 255, 255);
		glPopMatrix();

		glPushMatrix();//goal bar2
		glTranslatef(0, 0, 25);
		glScalef(2, 25, 2);
		Cube(255, 255, 255);
		glPopMatrix();

		glPushMatrix();//goal mid bar
		glTranslatef(0, 50, -24.25);
		glScalef(2, 2, 26.5);
		Cube(255, 255, 255);
		glPopMatrix();

	}

	void debox_line()
	{
		glPushMatrix(); //bar line2
		glTranslatef(-180, 0, 0);
		glTranslatef(0, 6, 0);
		glScalef(4, 5, 55);
		glTranslatef(-1, -1, -1);
		Cube(255, 255, 255);
		glPopMatrix();


		glPushMatrix(); //side line
		glTranslatef(-215, 0, -50);
		glTranslatef(0, 6, 0);
		glScalef(35, 5, 4);
		glTranslatef(-1, -1, -1);
		Cube(255, 255, 255);
		glPopMatrix();

		glPushMatrix(); //side line
		glTranslatef(-215, 0, 50);
		glTranslatef(0, 6, 0);
		glScalef(35, 5, 4);
		glTranslatef(-1, -1, -1);
		Cube(255, 255, 255);
		glPopMatrix();
	}

	void gallery()
	{
		//glBindTexture(GL_TEXTURE_2D, ID[white]);//football field
		glPushMatrix(); //gallery line
		glTranslatef(0, 0, -20);
		glTranslatef(-10, 0, -210);
		glTranslatef(0, 6, 0);
		glScalef(220, 25, 30);
		glTranslatef(-1, -1, -1);
		Cube(255, 255, 255);
		glPopMatrix();

		glPushMatrix(); // gallery line
		glTranslatef(0, 0, -20);
		glTranslatef(0, 0, -20);
		glTranslatef(-10, 25, -210);
		glTranslatef(0, 6, 0);
		glScalef(220, 25, 30);
		glTranslatef(-1, -1, -1);
		Cube(255, 255, 255);
		glPopMatrix();


		glPushMatrix(); //gallery line2
		glTranslatef(0, 0, 20);
		glTranslatef(-10, 0, 210);
		glTranslatef(0, 6, 0);
		glScalef(220, 25, 30);
		glTranslatef(-1, -1, -1);
		Cube(255, 255, 255);
		glPopMatrix();

		glPushMatrix(); // gallery line2
		glTranslatef(0, 0, 20);
		glTranslatef(0, 0, 20);
		glTranslatef(-10, 25, 210);
		glTranslatef(0, 6, 0);
		glScalef(220, 25, 30);
		glTranslatef(-1, -1, -1);
		Cube(255, 255, 255);
		glPopMatrix();
	}
	void football_field()
	{

		//glBindTexture(GL_TEXTURE_2D, ID[grass]);//football field

		glPushMatrix();
		glTranslatef(0, 5, 0);
		glScalef(400, 5, 280);
		glTranslatef(-1, -1, -1);
		Cube(0, 255, 0);
		glPopMatrix();




		//glBindTexture(GL_TEXTURE_2D, ID[field]);//football field
		glPushMatrix(); //field
		glRotatef(90, 0, 1, 0);
		glTranslatef(-14.5, 138, -117);
		glScalef(200, 5, 250);
		glTranslatef(-1, -25.5, -1.5);
		Cube(255, 255, 255);
		glPopMatrix();

		////glBindTexture(GL_TEXTURE_2D, ID[white]);//football field
		//glPushMatrix();
		//glTranslatef(x, y, z);  //bar1
		//goal_bar();
		//glPopMatrix();

		//glPushMatrix();
		//glTranslatef(235, 0, 0); //bar2
		//goal_bar();
		//glPopMatrix();



		//glPushMatrix();

		//gallery();
		//glPopMatrix();


	}





	static void Scene()
	{
		glPushMatrix();
		DrawEarth();
		glPopMatrix();

		glPushMatrix();
		ShahidMinar();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(405, 12, -3.5);
		Hall();
		glPopMatrix();
		football_field();
		glPushMatrix();
		Sky();
		glPopMatrix();
	}

	void cylinder(float w, float h, float l, float r, float g, float b)
	{
		int numberOfCube = 360;
		float width = w;
		float height = h;
		float length = l;
		for (double i = 0; i <= 360; i = i + 1.1)
		{
			glPushMatrix();
			glRotated(i, 0, 1, 0);
			glScalef(width, height, length);
			glTranslatef(-0.5, -0.5, -0.5);
			Cube(r, g, b);
			glPopMatrix();
		}
	}

	void main_floor()
	{
		//glBindTexture(GL_TEXTURE_2D, ID[white]);//main floor
		glPushMatrix();
		glScalef(900, 1, 1040);
		glTranslatef(-1, -1, -1);
		Cube(139, 69, 19);  //165, 42, 42
		glPopMatrix();

	}


	

	static void Light()
	{
		//Light
		glEnable(GL_LIGHTING);

		GLfloat noLight[] = { 0, 0, 0, 1 };
		GLfloat lightAmb[] = { 0.5, 0.5, 0.5, 1 };
		GLfloat lightDif[] = { 1, 1, 1, 1 };
		GLfloat lightSpec[] = { 1, 1, 1, 1 };
		GLfloat light1Pos[] = { 0, -500, 0, 1 };
		GLfloat light4Pos[] = { 0,500,0, 1 };

		// GLfloat light1Pos[] = {90, 90, 90, 1};
		//  GLfloat light4Pos[] = {90, 90, -90, 1};
		GLfloat light2Pos[] = { 683, 300, -350, 1 }; //spot light
		GLfloat light3Pos[] = { -380, 400, -50, 1 }; //spot light  GLfloat light2Pos[] = {15, 40, -45, 1}; //spot light
		// GLfloat light3Pos[] = {15, 40, 45, 1

		glEnable(GL_LIGHT0);  //1
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDif);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);
		glLightfv(GL_LIGHT0, GL_POSITION, light1Pos);


		glEnable(GL_LIGHT1); //2 spot light
		glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmb);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDif);
		glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpec);
		glLightfv(GL_LIGHT1, GL_POSITION, light2Pos);

		glEnable(GL_LIGHT2); //3 spot light
		glLightfv(GL_LIGHT2, GL_AMBIENT, lightAmb);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDif);
		glLightfv(GL_LIGHT2, GL_SPECULAR, lightSpec);
		glLightfv(GL_LIGHT2, GL_POSITION, light3Pos);

		glEnable(GL_LIGHT3); //4
		glLightfv(GL_LIGHT3, GL_AMBIENT, lightAmb);
		glLightfv(GL_LIGHT3, GL_DIFFUSE, lightDif);
		glLightfv(GL_LIGHT3, GL_SPECULAR, lightSpec);
		glLightfv(GL_LIGHT3, GL_POSITION, light4Pos);

		//GLfloat spotDirection[] = {0, -18, 20, 1};

		//2    {90, 36, -120, 1};
		//3    {125, 36, 180, 1};

		GLfloat spotDirection[] = { 0, -1, 0, 1 };   //2
		GLfloat spotCutOff[] = { 60 };

		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDirection);
		glLightfv(GL_LIGHT2, GL_SPOT_CUTOFF, spotCutOff);

		GLfloat spotDirection2[] = { 0, -1, 0, 1 }; //3
		GLfloat spotCutOff2[] = { 60 };

		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDirection2);
		glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, spotCutOff2);


		if (light0)
		{
			glEnable(GL_LIGHT0);
		}
		else
		{
			glDisable(GL_LIGHT0);
		}

		if (light1)
		{
			glEnable(GL_LIGHT1);
		}
		else
		{
			glDisable(GL_LIGHT1);
		}

		if (light2)
		{
			glEnable(GL_LIGHT2);
		}
		else
		{
			glDisable(GL_LIGHT2);
		}
		if (light3)
		{
			glEnable(GL_LIGHT3);
		}
		else
		{
			glDisable(GL_LIGHT3);
		}


		if (ambient)
		{
			glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
			glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmb);
			glLightfv(GL_LIGHT2, GL_AMBIENT, lightAmb);
			glLightfv(GL_LIGHT3, GL_AMBIENT, lightAmb);

		}
		else
		{
			glLightfv(GL_LIGHT0, GL_AMBIENT, noLight);
			glLightfv(GL_LIGHT1, GL_AMBIENT, noLight);
			glLightfv(GL_LIGHT2, GL_AMBIENT, noLight);
			glLightfv(GL_LIGHT3, GL_AMBIENT, noLight);
		}

		if (diffuse)
		{
			glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDif);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDif);
			glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDif);
			glLightfv(GL_LIGHT3, GL_DIFFUSE, lightDif);
		}
		else
		{
			glLightfv(GL_LIGHT0, GL_DIFFUSE, noLight);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, noLight);
			glLightfv(GL_LIGHT2, GL_DIFFUSE, noLight);
			glLightfv(GL_LIGHT3, GL_DIFFUSE, noLight);
		}

		if (specular)
		{
			glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);
			glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpec);
			glLightfv(GL_LIGHT2, GL_SPECULAR, lightSpec);
			glLightfv(GL_LIGHT3, GL_SPECULAR, lightSpec);
		}
		else
		{
			glLightfv(GL_LIGHT0, GL_SPECULAR, noLight);
			glLightfv(GL_LIGHT1, GL_SPECULAR, noLight);
			glLightfv(GL_LIGHT2, GL_SPECULAR, noLight);
			glLightfv(GL_LIGHT3, GL_SPECULAR, noLight);
		}

	}

	static void Draw()
	{
		// Draw the scene
		glPushMatrix();
		glScaled(0.1, 0.1, 0.1);
		glRotated(-90, 0, 1, 0);
		glTranslatef(0, 0, -100);
		Scene();
		glPopMatrix();
	}

	static void Keyboard()
	{
		if (Scene::keyStates['A'])
		{
			Scene::x -= 0.5f;
			std::cout << "Translation: " << Scene::x << ", " << Scene::y << ", " << Scene::z << std::endl;
		}
		if (Scene::keyStates['D'])
		{
			Scene::x += 0.5f;
			std::cout << "Translation: " << Scene::x << ", " << Scene::y << ", " << Scene::z << std::endl;
		}
		if (Scene::keyStates['W'])
		{
			Scene::z -= 0.5f;
			std::cout << "Translation: " << Scene::x << ", " << Scene::y << ", " << Scene::z << std::endl;
		}
		if (Scene::keyStates['S'])
		{
			Scene::z += 0.5f;
			std::cout << "Translation: " << Scene::x << ", " << Scene::y << ", " << Scene::z << std::endl;
		}
		if (Scene::keyStates['Q'])
		{
			Scene::y -= 0.5f;
			std::cout << "Translation: " << Scene::x << ", " << Scene::y << ", " << Scene::z << std::endl;
		}
		if (Scene::keyStates['E'])
		{
			Scene::y += 0.5f;
			std::cout << "Translation: " << Scene::x << ", " << Scene::y << ", " << Scene::z << std::endl;
		}
		GLfloat x1 = lookX - cameraX;
		GLfloat z1 = lookZ - cameraZ;
		GLfloat r = sqrt(x1 * x1 + z1 * z1);

		GLfloat theta = 0.0f;
		if (x1 == 0)
		{
			if (z1 > 0)
				theta = 90;
			else if (z1 < 0)
				theta = -90;
		}
		else
			theta = atan(z1 / x1) * 180 / std::numbers::pi;

		if ((z1 > 0 && theta < 0) || (z1 < 0 && theta>0))
			theta += 180;
		else if (z1 < 0 && theta < 0)
			theta += 360;

		GLfloat turn_angle_step = 10;

		if (Scene::keyStates['u'])
		{
			theta -= turn_angle_step;
			theta = theta * std::numbers::pi / 180;
			lookX = r * cos(theta) + cameraX;
			lookZ = r * sin(theta) + cameraZ;
		}
		if (Scene::keyStates['U'])
		{
			theta += turn_angle_step;
			theta = theta * std::numbers::pi / 180;
			lookX = r * cos(theta) + cameraX;
			lookZ = r * sin(theta) + cameraZ;
		}

		if (Scene::keyStates['r'])
			lookY += 1.0f;

		if (Scene::keyStates['R'])
			lookY -= 1.0f;

		if (Scene::keyStates['p'])
		{
			anglePitch -= 1.0f;
			lookX = r * (cos(anglePitch * std::numbers::pi / 180.0)) * (cos(angleYaw * std::numbers::pi / 180.0));
			lookY = r * (sin(anglePitch * std::numbers::pi / 180.0));
			lookZ = r * (cos(anglePitch * std::numbers::pi / 180.0)) * (sin(angleYaw * std::numbers::pi / 180.0));
		}
		if (Scene::keyStates['P'])
		{
			anglePitch += 1.0f;
			lookX = 50.0 * (cos(anglePitch * std::numbers::pi / 180.0)) * (cos(angleYaw * std::numbers::pi / 180.0));
			lookY = 50.0 * (sin(anglePitch * std::numbers::pi / 180.0));
			lookZ = 50.0 * (cos(anglePitch * std::numbers::pi / 180.0)) * (sin(angleYaw * std::numbers::pi / 180.0));
		}

		GLfloat r1 = 1.0f;
		GLdouble dx = r1 * cos(theta * std::numbers::pi / 180);
		GLdouble dz = r1 * sin(theta * std::numbers::pi / 180);

		GLdouble dx_norm = r1 * cos((theta - 90) * std::numbers::pi / 180);
		GLdouble dz_norm = r1 * sin((theta - 90) * std::numbers::pi / 180);

		/*if (Scene::keyStates['j'])
		{
			cameraX += dx_norm * 3;
			cameraZ += dz_norm * 3;
			lookX += dx_norm * 3;
			lookZ += dz_norm * 3;
		}
		if (Scene::keyStates['i'])
		{
			cameraX += dx * 3;
			cameraZ += dz * 3;
			lookX += dx * 3;
			lookZ += dz * 3;
		}
		if (Scene::keyStates['k'])
		{
			cameraX -= dx * 3;
			cameraZ -= dz * 3;
			lookX -= dx * 3;
			lookZ -= dz * 3;
		}
		if (Scene::keyStates['l'])
		{
			cameraX -= dx_norm * 3;
			cameraZ -= dz_norm * 3;
			lookX -= dx_norm * 3;
			lookZ -= dz_norm * 3;
		}*/
	
	}

	static void Special()
	{
		if (keyStates[GLUT_KEY_LEFT])
		{
			Scene::cameraX -= 0.5f * Speed;
			Scene::lookX -= 0.5f * Speed;
			std::cout << "cameraX=" << cameraX << std::endl;
			std::cout << "lookX=" << lookX << std::endl;

		}
		if (keyStates[GLUT_KEY_RIGHT]) 
		{
			Scene::cameraX += 0.5f * Speed;
			Scene::lookX += 0.5f * Speed;
			/*std::cout << "cameraX=" << cameraX << std::endl;
			std::cout << "lookX=" << lookX << std::endl;*/

		}
		if (keyStates[GLUT_KEY_UP])
		{
			Scene::cameraY += 0.5f * Speed;
			Scene::lookY += 0.5f * Speed;
			/*std::cout << "cameraY=" << cameraY << std::endl;
			std::cout << "lookY=" << lookY << std::endl;*/
		}
		if (keyStates[GLUT_KEY_DOWN])
		{
			Scene::cameraY -= 0.5f * Speed;
			Scene::lookY -= 0.5f * Speed;
			/*std::cout << "cameraY=" << cameraX << std::endl;
			std::cout << "lookY=" << lookX << std::endl;*/
		}

		// Forward and Backward Movement
		if (keyStates[GLUT_KEY_PAGE_UP])
		{
			Scene::cameraZ -= 0.5f * Speed;
			Scene::lookZ -= 0.5f * Speed;
			/*std::cout << "cameraZ=" << cameraZ << std::endl;
			std::cout << "lookZ=" << lookZ << std::endl;*/
		}
		if (keyStates[GLUT_KEY_PAGE_DOWN])
		{
			Scene::cameraZ += 0.5f * Speed;
			Scene::lookZ += 0.5f * Speed;
			/*std::cout << "cameraZ=" << cameraZ << std::endl;
			std::cout << "lookZ=" << lookZ << std::endl;*/
		}
	}
}

void Control::display()
{
	// Count the FPS
	static int frame = 0;
	static int time = 0;
	static int timebase = 0;
	char s[30]{};
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		std::string fps = "FPS: " + std::to_string(frame * 1000.0 / (time - timebase));
		std::string title = "Shahid Minar" + std::string(" - ") + fps;
		glutSetWindowTitle(title.c_str());
		//sprintf(s, "FPS:%4.2f", frame * 1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}

	// Set up background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set up the projection matrix
	gluOrtho2D(0, 1280, 0, 720);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-10, 10, -10, 10, 5, 2000);

	// Set up the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		Scene::cameraX, Scene::cameraY, Scene::cameraZ,
		Scene::lookX, Scene::lookY, Scene::lookZ,
		Scene::upX, Scene::upY, Scene::upZ
	);

	glViewport(0, 0, 1280, 720);

	// Setting the rotation of the scene
	glRotatef(Scene::angleX, Scene::axisX, Scene::axisY, Scene::axisZ);
	glRotatef(Scene::angleY, Scene::axisX, Scene::axisY, Scene::axisZ);
	glRotatef(Scene::angleZ, Scene::axisX, Scene::axisY, Scene::axisZ);

	// Lighting
	Scene::Light();

	// Draw the scene
	Scene::Draw();

	// Keyboard input
	Scene::Keyboard();
	Scene::Special();

	// Swap the buffers
	glutSwapBuffers();

	// Redraw the scene
	glutPostRedisplay();
}

void Control::reshape(int width, int height) {}

void Control::keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		// Scene Rotation (Positive Direction)
	case 'x':
		Scene::rotateX = !Scene::rotateX;
		Scene::axisX = 1.0f;
		break;
	case 'y':
		Scene::rotateY = !Scene::rotateY;
		Scene::axisY = 1.0f;
		break;
	case 'z':
		Scene::rotateZ = !Scene::rotateZ;
		Scene::axisZ = 1.0f;
		break;
		// Scene Rotation (Negative Direction)
	case 'X':
		Scene::rotateX = !Scene::rotateX;
		Scene::axisX = -1.0f;
		break;
	case 'Y':
		Scene::rotateY = !Scene::rotateY;
		Scene::axisY = -1.0f;
		break;
	case 'Z':
		Scene::rotateZ = !Scene::rotateZ;
		Scene::axisZ = -1.0f;
		break;

		// Light Control
	case '0':
		Scene::light0 = !Scene::light0;
		break;
	case '1':
		Scene::light1 = !Scene::light1;
		break;
	case '2':
		Scene::light2 = !Scene::light2;
		break;
	case '3':
		Scene::light3 = !Scene::light3;
		break;

		// Placement testing
	case 'A':
		Scene::keyStates['A'] = true;
		break;
	case 'D':
		Scene::keyStates['D'] = true;
		break;
	case 'W':
		Scene::keyStates['W'] = true;
		break;
	case 'S':
		Scene::keyStates['S'] = true;
		break;
	case 'Q':
		Scene::keyStates['Q'] = true;
		break;
	case 'E':
		Scene::keyStates['E'] = true;
		break;
	case 'r':
		Scene::keyStates['r'] = true;
		break;
	case 'p':
		Scene::keyStates['p'] = true;
		break;
	case 'u':
		Scene::keyStates['u'] = true;
		break;

		// Roll, Pitch, Yaw (Negative Direction)
	case 'R':
		Scene::keyStates['R'] = true;
		break;
	case 'P':
		Scene::keyStates['P'] = true;
		break;
	case 'U':
		Scene::keyStates['U'] = true;
		break;

		// Front-Back, Left-Right Movement
	/*case 'j':
		Scene::keyStates['j'] = true;
		break;
	case 'i':
		Scene::keyStates['i'] = true;
		break;
	case 'k':
		Scene::keyStates['k'] = true;
		break;
	case 'l':
		Scene::keyStates['l'] = true;
		break;*/

		// Exit the program

	case 27:
		exit(EXIT_SUCCESS);
	default:
		break;
	}
}

void Control::keyboardup(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'A':
		Scene::keyStates['A'] = false;
		break;
	case 'D':
		Scene::keyStates['D'] = false;
		break;
	case 'W':
		Scene::keyStates['W'] = false;
		break;
	case 'S':
		Scene::keyStates['S'] = false;
		break;
	case 'Q':
		Scene::keyStates['Q'] = false;
		break;
	case 'E':
		Scene::keyStates['E'] = false;
		break;
	case 'r':
		Scene::keyStates['r'] = false;
		break;
	case 'p':
		Scene::keyStates['p'] = false;
		break;
	case 'u':
		Scene::keyStates['u'] = false;
		break;

	case 'R':
		Scene::keyStates['R'] = false;
		break;
	case 'P':
		Scene::keyStates['P'] = false;
		break;
	case 'U':
		Scene::keyStates['U'] = false;
		break;

		/*case 'j':
			Scene::keyStates['j'] = false;
			break;
		case 'i':
			Scene::keyStates['i'] = false;
			break;
		case 'k':
			Scene::keyStates['k'] = false;
			break;
		case 'l':
			Scene::keyStates['l'] = false;
			break;*/
	default:
		break;
	}
}

void Control::special(int key, int x, int y)
{
	//std::cout << "Special Key Down Function called for " << key << std::endl;

	// Panning the camera
	switch (key)
	{
	case GLUT_KEY_LEFT:
		Scene::keyStates[GLUT_KEY_LEFT] = true;
		break;
	case GLUT_KEY_RIGHT:
		Scene::keyStates[GLUT_KEY_RIGHT] = true;
		break;
	case GLUT_KEY_UP:
		Scene::keyStates[GLUT_KEY_UP] = true;
		break;
	case GLUT_KEY_DOWN:
		Scene::keyStates[GLUT_KEY_DOWN] = true;
		break;

		// Zooming the camera
	case GLUT_KEY_PAGE_UP:
		Scene::keyStates[GLUT_KEY_PAGE_UP] = true;
		break;
	case GLUT_KEY_PAGE_DOWN:
		Scene::keyStates[GLUT_KEY_PAGE_DOWN] = true;
		break;
	default:
		break;
	}
}

void Control::specialup(int key, int x, int y)
{
	//std::cout << "Special Key Up Function called for " << key << std::endl;

	switch (key)
	{
	case GLUT_KEY_LEFT:
		Scene::keyStates[GLUT_KEY_LEFT] = false;
		break;
	case GLUT_KEY_RIGHT:
		Scene::keyStates[GLUT_KEY_RIGHT] = false;
		break;
	case GLUT_KEY_UP:
		Scene::keyStates[GLUT_KEY_UP] = false;
		break;
	case GLUT_KEY_DOWN:
		Scene::keyStates[GLUT_KEY_DOWN] = false;
		break;
		// Forward and Backward Movement
	case GLUT_KEY_PAGE_UP:
		Scene::keyStates[GLUT_KEY_PAGE_UP] = false;
		break;
	case GLUT_KEY_PAGE_DOWN:
		Scene::keyStates[GLUT_KEY_PAGE_DOWN] = false;
		break;
	default:
		break;
	}
}

void Control::mouse(int button, int state, int x, int y) {}

void Control::motion(int x, int y) {}

void Control::idle()
{
	if (Scene::rotateX)
	{
		Scene::angleX += 1.0f;
		if (Scene::angleX >= 360.0f)
			Scene::angleX = 0.0f;
	}
	if (Scene::rotateY)
	{
		if (Scene::angleY >= 360.0f)
			Scene::angleY = 0.0f;
		Scene::angleY += 1.0f;
	}
	if (Scene::rotateZ)
	{
		if (Scene::angleZ >= 360.0f)
			Scene::angleZ = 0.0f;
		Scene::angleZ += 1.0f;
	}

	// Redraw the scene
	glutPostRedisplay();
}