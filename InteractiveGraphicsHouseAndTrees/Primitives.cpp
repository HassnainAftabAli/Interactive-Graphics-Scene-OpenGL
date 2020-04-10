#include <GL/freeglut.h>
#include <math.h>

void  Cube(float size) {
	float c[4];
	glGetFloatv(GL_CURRENT_COLOR, c);

	glutSolidCube(size);

	glColor3f(c[0] - 0.2, c[1] - 0.2, c[2] - 0.2);
	glutWireCube(size);

	glColor4fv(c);  // restore orginal color
}

void Sphere(float radius, float resX, float resY) {
	float c[4];
	glGetFloatv(GL_CURRENT_COLOR, c);
	glutSolidSphere(radius, resX, resY);

	glColor3f(c[0] - 0.2, c[1] - 0.2, c[2] - 0.2);
	glutWireSphere(radius, resX, resY);

	glColor4fv(c);  // restore orginal color
}

void Cone(float base, float height, float resX, float resY) {
	float c[4];
	glGetFloatv(GL_CURRENT_COLOR, c);
	glutSolidCone(base, height, resX, resY);

	glColor3f(c[0] - 0.2, c[1] - 0.2, c[2] - 0.2);
	glutWireCone(base, height, resX, resY);

	glColor4fv(c);  // restore orginal color
}

void Cylinder(float base, float height, float resX, float resY) {
	float c[4];
	glGetFloatv(GL_CURRENT_COLOR, c);
	//gluS(base, height, resX, resY);
	glutSolidCylinder(base, height, resX, resY);

	glColor3f(c[0] - 0.2, c[1] - 0.2, c[2] - 0.2);
	glutWireCylinder(base, height, resX, resY);

	glColor4fv(c);  // restore orginal color
}

void Torus(float innerRadius, float outerRadius, float resX, float resY) {
	float c[4];
	glGetFloatv(GL_CURRENT_COLOR, c);
	//gluS(base, height, resX, resY);
	glutSolidTorus( innerRadius, outerRadius, resX, resY);

	glColor3f(c[0] - 0.2, c[1] - 0.2, c[2] - 0.2);
	glutWireTorus(innerRadius, outerRadius, resX, resY);

	glColor4fv(c);  // restore orginal color
}

void floor() {
	float c[4];
	glGetFloatv(GL_CURRENT_COLOR, c);

	glColor3f(0.5, 0.8, 1);
	glBegin(GL_LINES);
	for (float i = -20; i <= 20; i += 4) {

		glVertex3f(-20, 0, i);
		glVertex3f(20, 0, i);

		glVertex3f(i, 0, -20);
		glVertex3f(i, 0, 20);
	}
	glEnd();

	glColor4f(0.5, 0.8, 1, 1);
	glBegin(GL_QUADS);
	glVertex3f(-20, 0, -20);
	glVertex3f(-20, 0, 20);
	glVertex3f(20, 0, 20);
	glVertex3f(20, 0, -20);
	glEnd();

	glColor4fv(c);  // restore orginal color
}

void Primitives() {
	// Cube 1
	glColor3f(1, 0.4 , 0.4);
	glPushMatrix();
	glTranslatef(0, 1, -2);
	Cube(0.5);
	glPopMatrix();

	// Cube 2
	glColor3f(1, 0.4, 0.4);
	glPushMatrix();
	glTranslatef(0, 1, 0);
	glScalef(0.2, 0.2, 1);
	Cube(0.5);
	glPopMatrix();

	// Cube 3
	glColor3f(1, 0.4, 0.4);
	glPushMatrix();
	glTranslatef(0, 1, 2);
	glScalef(0.2, 1, 0.2);
	Cube(0.5);
	glPopMatrix();

	// Sphere 1
	glColor3f(0.3,1.0, 0.4);
	glPushMatrix();
	glTranslatef(2, 1, -2);
	Sphere(0.3, 20, 20);
	glPopMatrix();

	// Sphere 2
	glColor3f(0.3, 1.0, 0.4);
	glPushMatrix();
	glTranslatef(2, 1, 0);
	Sphere(0.3, 2, 10);
	glPopMatrix();

	// Sphere 3
	glColor3f(0.3, 1.0, 0.4);
	glPushMatrix();
	glTranslatef(2, 1, 2);
	Sphere(0.3, 10, 2);
	glPopMatrix();

	// Cone 1
	glColor3f(0.3, 0.4, 1.0);
	glPushMatrix();
	glTranslatef(4, 1, -2);
	Cone(0.25, 0.5, 20, 10);
	glPopMatrix();

	// Cone 2
	glColor3f(0.3, 0.4, 1.0);
	glPushMatrix();
	glTranslatef(4, 1, 0);
	Cone(0.25, 0.5, 4, 1);
	glPopMatrix();

	// Cone 3
	glColor3f(0.3, 0.4, 1.0);
	glPushMatrix();
	glTranslatef(4, 1, 2);
	Cone(0.25, 0.5, 6, 1);
	glPopMatrix();

	// Cylinder 1
	glColor3f(1, 1, 0.2);
	glPushMatrix();
	glTranslatef(-2, 1, -2);
	Cylinder(0.25, 0.5, 20, 10);
	glPopMatrix();

	// Cylinder 2
	glColor3f(1, 1, 0.2);
	glPushMatrix();
	glTranslatef(-2, 1, 0);
	Cylinder(0.25, 0.5, 6, 1);
	glPopMatrix();

	// Cylinder 3
	glColor3f(1, 1, 0.2);
	glPushMatrix();
	glTranslatef(-2, 1, 2);
	Cylinder(0.25, 0.5, 3, 5);
	glPopMatrix();

	// Torus 1 
	glColor3f(0.8, 0.6, 0.2);
	glPushMatrix();
	glTranslatef(-4, 1, -2);
	Torus(0.1, 0.25, 20, 30);
	glPopMatrix();

	// Torus 2
	glColor3f(0.8, 0.6, 0.2);
	glPushMatrix();
	glTranslatef(-4, 1, 0);
	Torus(0.1, 0.25, 4, 5);
	glPopMatrix();

}
