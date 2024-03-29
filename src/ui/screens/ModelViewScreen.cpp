#include "ModelViewScreen.h"

#include <platform_definitions.h>
#include <string.h>
#include <models/House.h>
#include <iostream>

#include <tga/tga.h>

void drawSelected(CVector3 intersection) {

	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);

	int x = (int)(intersection.x);
	int z = (int)(intersection.z);
	float y = -0.4f;

	glVertex3f(x, y, z);
	glVertex3f(x, y, z + 1.0f);
	glVertex3f(x + 1.0f, y, z + 1.0f);
	glVertex3f(x + 1.0f, y, z);
	glEnd();
}

void checkSelected() {

}


ModelViewScreen::ModelViewScreen(int argc, char **argv) : gui::Screen(argc, argv)
{
	ObjModelReader modelReader;
	modelReader.load("resources/models/cube.obj");

	this->cube = new Model(modelReader);
	this->cube->init();

	// this->model = new House();
	// this->model->init();

 	this->terrain = new Terrain();
 	this->terrain->init();
	
	this->camera.init(
		0, 25, 10, 
		0, 0, 0, 
		0, 1, 0);
}

ModelViewScreen::~ModelViewScreen()
{
}

void DrawGround() {
	TGA *tga;
	TGAData *data;

	data = (TGAData*)malloc(sizeof(TGAData));
	if (!data) {
		TGA_ERROR((TGA*)NULL, TGA_OOM);
		return;
	}

	printf("[open] name=%s, mode=%s\n", "resources/textures/background.tga", "r");
	tga = TGAOpen("resources/maps/ground.tga", "r");
	if (!tga || tga->last != TGA_OK) {
		TGA_ERROR(tga, TGA_OPEN_FAIL);
		return;
	}

	printf("[read] image\n");
	data->flags = TGA_IMAGE_INFO | TGA_IMAGE_DATA;;
	if (TGAReadImage(tga, data) != TGA_OK) {
		TGA_ERROR(tga, TGA_READ_FAIL);
		return;
	}

	if (data->flags & TGA_IMAGE_INFO) {
		printf("[info] width=%i\n", tga->hdr.width);
		printf("[info] height=%i\n", tga->hdr.height);

		printf("[info] color map type=%i\n", tga->hdr.map_t);

		printf("[info] image type=%i\n", tga->hdr.img_t);

		printf("[info] depth=%i\n", tga->hdr.depth);
		printf("[info] x=%i\n", tga->hdr.x);
		printf("[info] y=%i\n", tga->hdr.y);
		printf("[info] orientation=%s-%s\n",
			(tga->hdr.vert == TGA_BOTTOM) ?
			"bottom" : "top",
			(tga->hdr.horz == TGA_LEFT) ?
			"left" : "right");
	}

	for (int i = 0; i < tga->hdr.width; i++) {
		for (int j = 0; j < tga->hdr.height; j++) {
			std::cout << data->img_data[i * tga->hdr.width + j] << " ";
		}
		std::cout << "\n";
	}

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
	//DrawGround();
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

void drawBitmapText(char *string,float x,float y,float z)
{
	char *c;
	glRasterPos3f(x, y,z);

	for (c=string; *c != 0; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);
	}
}

void ModelViewScreen::renderScene() {
	long beginTime = time_system::currentTimeInMillis();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	this->DrawText("FPS: " + std::to_string(previousFrames));

	glLoadIdentity();
	camera.applyView();
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);

	// this->model->Draw();

	glPopMatrix();
	this->cube->Draw();
	//drawGround2();
	 terrain->Draw();
	glBegin(GL_LINE_STRIP);
	glVertex3f(unprojectedVectorFar.x, unprojectedVectorFar.y, unprojectedVectorFar.z);
	glVertex3f(unprojectedVectorNear.x, unprojectedVectorNear.y, unprojectedVectorNear.z);
	glEnd();

	glutSwapBuffers();

	long finishTime = time_system::currentTimeInMillis();

	calcFrames(beginTime, finishTime);


}

void ModelViewScreen::calcFrames(long beginTime, long finishTime) {
	std::cout<<"---------------------------------------Timer------------------------------------------\n";
	std::cout<<" beginTime: "<< beginTime <<" milliseconds\n";
	std::cout<<" finishTime: "<< finishTime <<" milliseconds\n";
	std::cout<<"Render one frame tooks "<< finishTime - beginTime  <<" milliseconds\n";

	// int timeRemaining = 500 - (finishTime - beginTime);
	long timeRemaining = 16 - (finishTime - beginTime);
	std::cout<<"Time reamining "<< timeRemaining  <<" milliseconds\n";

	if( timeRemaining > 0) {
		int err = time_system::sleepInMillis(timeRemaining);
		 if(err) {
			std::cout<<"USLEEP fucked off "<<err<< std::endl;
		 }
	}

	frames++;

	std::cout<<"FPS "<< previousFrames <<" x\n";
	std::cout<<"millisPassed "<< millisPassed <<" x\n";
	if(finishTime  - millisPassed > 1000) {
		millisPassed = finishTime;
		previousFrames = frames;
		frames = 0;

	}
}

void ModelViewScreen::DrawText(std::string text) const {//TEXT
	glMatrixMode( GL_PROJECTION ) ;
	glPushMatrix() ; // save
	glLoadIdentity();// and clear
	glMatrixMode( GL_MODELVIEW ) ;
	glPushMatrix() ;
	glLoadIdentity() ;
/*
    glDisable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
*/
	glDisable( GL_TEXTURE_2D );
    glEnable(GL_DEPTH_TEST);

	gluOrtho2D(-100, 100, -100, 100);
	glRasterPos2i( -90,90 ) ; // center of screen. (-1,0) is center left.
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	for (int i = 0; i < text.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)text[i]);


    // glRasterPos2f( 0.0f,0.0f ) ; // center of screen. (-1,0) is center left.
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
        glVertex2d(-1.0f, -1.0f);
        glVertex2d(1.0f, 1.0f);
        glVertex2d(-1.0f, 1.0f);

    glEnd();

	glEnable( GL_DEPTH_TEST ) ; // Turn depth testing back on


	glMatrixMode( GL_PROJECTION ) ;
	glPopMatrix() ; // revert back to the matrix I had before.
	glMatrixMode( GL_MODELVIEW ) ;
	glPopMatrix() ;
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
	case 'q': case 'Q':
		this->model->rotate(-90);
		break;
	case 'e': case 'E':

		this->model->rotate(+90);
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

	//this->model->translate(lastIntersect); -- this cause exception in rendering
 	//checkSelected();
}
