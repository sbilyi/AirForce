#include "ModelViewScreen.h"
#include "../../model/Cube.h"
#include "../../model/House.h"
#include <iostream>
#include <math.h>

int rot = 0;
/*
float PlaneDistance(CVector3 a, CVector3 b)
{
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z)); 
}

float Normal(CVector3 vPolygon[])
{
	CVector3 vVector1 = vPolygon[2] - vPolygon[0];
	CVector3 vVector2 = vPolygon[1] - vPolygon[0];
	CVector3 vNormal = Cross(vVector1, vVector2);
	vNormal = Normalize(vNormal);
	return vNormal;
}

bool IntersectedPlane(CVector3 vPoly[], CVector3 vLine[],
	CVector3 &vNormal, float &originDistance)
{
	float distance1 = 0, distance2 = 0;                  vNormal = Normal(vPoly);
	originDistance = PlaneDistance(vNormal, vPoly[0]);

	distance1 = ((vNormal.x * vLine[0].x) + (vNormal.y * vLine[0].y) +
		(vNormal.z * vLine[0].z)) + originDistance;
	distance2 = ((vNormal.x * vLine[1].x) + (vNormal.y * vLine[1].y) +
		(vNormal.z * vLine[1].z)) + originDistance;

	if (distance1 * distance2 >= 0) return false;
	return true;
}

*/

void drawSelected() {
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glEnd();
}

void checkSelected() {

}


ModelViewScreen::ModelViewScreen(int argc, char **argv) : gui::Screen(argc, argv)
{
	this->model = new House();
	this->model->init();

// 	this->terrain = new Terrain();
// 	this->terrain->init();
	
	this->camera.init(
		0, 5, 10, 
		0, 0, 0, 
		0, 1, 0);
}

ModelViewScreen::~ModelViewScreen()
{
}

void ModelViewScreen::init(void){

	std::cout << "init...";
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, this->width, this->height);
	GLfloat aspect = (GLfloat)this->width / this->height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(this->fieldOfViewAngle, aspect, this->zNear, this->zFar);
	glMatrixMode(GL_MODELVIEW);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	GLfloat amb_light[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1 };
	GLfloat specular[] = { 0.7, 0.7, 0.3, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void drawGround2() {
	GLfloat fExtent = 50.f;
	GLfloat fStep = 1.0f;
	GLfloat y = -0.4f;
	GLfloat iLine;

	glBegin(GL_LINES);
	for (iLine = -fExtent; iLine <= fExtent; iLine += fStep) {
		if (iLine < 0)
			glColor3f(1.0f, 0.0f, 0.0f);
		else
			glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(iLine, y, fExtent);
		glVertex3f(iLine, y, -fExtent);
		glVertex3f(fExtent, y, iLine);
		glVertex3f(-fExtent, y, iLine);
	}
	glEnd();
}
void ModelViewScreen::renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//gluLookAt(0, 5, 10, 0, 0, 0, 0, 1, 0);
	camera.applyView();
	glPushMatrix();
	glRotatef(rot % 360, 0, 1, 0);
	glTranslatef(lastIntersect.x, lastIntersect.y, lastIntersect.z);
	// glRotatef(90, 0, 1, 0);
	//	g_rotation++;
	//glutSolidSphere(5, 15, 15);
	glColor3f(1.0f, 0.0f, 1.0f);
	//glutSolidSphere(1, 15, 15);
	glColor3f(1.0f, 1.0f, 1.0f);
	
	this->model->Draw();
	//this->terrain->Draw();
	glPopMatrix();
	 drawGround2();
	glBegin(GL_LINE_STRIP);
	glVertex3f(unprojectedVectorFar.x, unprojectedVectorFar.y, unprojectedVectorFar.z);
	glVertex3f(unprojectedVectorNear.x, unprojectedVectorNear.y, unprojectedVectorNear.z);
	glEnd();
	drawSelected();
	glutSwapBuffers();

}

void ModelViewScreen::reshapeScene(int width, int height) {
	std::cout << "reshapeScene...";
	
	Screen::reshapeScene(width, height);
}

void ModelViewScreen::keyboardAction(unsigned char key, int x, int y) {
	
	std::cout << "Pressed:" << key << "[" << x << "," << y << "]\n";
	switch (key) {
	case 'a': case 'A': //toggle screenmode
		camera.moveLeft();
		break;
	case 's': case 'S': camera.moveBack(); break;
	case 'w': case 'W': camera.moveForward(); break;
	case 'd': case 'D': camera.moveRight(); break;
	case 'q': case 'Q': rot -= 15; // camera.moveLeftSide(); 
		break;
	case 'e': case 'E': rot += 15;
		// camera.moveRightSide(); 
		break;
	default:
		Screen::keyboardAction(key, x, y);
	}
}

void ModelViewScreen::mouseClick(int button, int state, int x, int y) {

	std::cout << "[" << button << ":" << state << "]\n";
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		unProjectMouse(x, y);
	} else if (button == 3 && state == GLUT_DOWN) {
		this->camera.moveUp();
	} else if (button == 4 && state == GLUT_DOWN) {
		this->camera.moveDown();
	} else {
		Screen::mouseClick(button, state, x, y);
	}
}

void ModelViewScreen::unProjectMouse(int x, int y) {
	// mouse_x, mouse_y  - ������� ���������� ������� ����.
	// p1, p2            - ������������ ��������� - ����� �������������� �������,
	//                     ������� �������������� �� ������� � ������� ����������
	//                     ���������.
	GLint    viewport[4];    // ��������� viewport-a.
	GLdouble projection[16]; // ������� ��������.
	GLdouble modelview[16];  // ������� �������.
	GLdouble vx, vy, vz;       // ���������� ������� ���� � ������� ��������� viewport-a.
	GLdouble wx, wy, wz;       // ������������ ������� ����������.

	glGetIntegerv(GL_VIEWPORT, viewport);           // ����� ��������� viewport-a.
	glGetDoublev(GL_PROJECTION_MATRIX, projection); // ����� ������� ��������.
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);   // ����� ������� �������.
	// ��������� ������� ���������� ������� � ������� ��������� viewport-a.
	vx = x;
	vy = this->height - y - 1; // ��� height - ������� ������ ����.

	// ��������� ������� ����� �������������� �������.
	vz = -1;
	gluUnProject(vx, vy, vz, modelview, projection, viewport, &wx, &wy, &wz);

	//p1 = CVector3(wx,wy,wz);
	unprojectedVectorNear.x = wx;
	unprojectedVectorNear.y = wy;
	unprojectedVectorNear.z = wz;
	// ��������� ������� ����� �������������� �������.
	vz = 1;
	gluUnProject(vx, vy, vz, modelview, projection, viewport, &wx, &wy, &wz);

	//p2 = CVector3(wx,wy,wz);
	unprojectedVectorFar.x = wx;
	unprojectedVectorFar.y = wy;
	unprojectedVectorFar.z = wz;
	
	terrain->intersect(unprojectedVectorNear, unprojectedVectorFar, &lastIntersect);

	checkSelected();
}