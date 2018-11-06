#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>
#include <math.h>

#define PI 3.14159265

static float viewer[3];
static float torus[36][18][3] = {
	{{0,0,1}}
};

/**
	Rotate 10 degrees on the y axis.
*/
float* MajorRotate(int nCircle, int nPoint, float theta)
{
	float sinTheta = sin(theta*PI / 180.0);
	float cosTheta = cos(theta*PI / 180.0);
	float inputCoordinate[4];
	float resultCoordinate[3];
	float calculateMatrix[4][4] = { 0, };

	for (int i = 0; i < 3; i++)
		inputCoordinate[i] = torus[nCircle][nPoint][i];
	inputCoordinate[3] = 1;

	// init calculation matrix
	calculateMatrix[0][0] = cosTheta;
	calculateMatrix[0][2] = -sinTheta;
	calculateMatrix[1][1] = 1;
	calculateMatrix[2][0] = sinTheta;
	calculateMatrix[2][2] = cosTheta;
	calculateMatrix[3][3] = 1;

	for (int rol = 0; rol < 3; rol++)
	{
		float sum = 0;
		for (int col = 0; col < 4; col++)
		{
			sum += calculateMatrix[rol][col] * inputCoordinate[col];
		}
		resultCoordinate[rol] = sum;
	}

	return resultCoordinate;
}

/**
	Move coordinate by -x, -y and rotate coordinate by theta, move coordinate by x, y.
*/
float* MinorRotate(int nCircle, int nPoint, float theta, float x, float y)
{	
	float sinTheta = sin(theta*PI / 180.0);
	float cosTheta = cos(theta*PI / 180.0);
	float inputCoordinate[3];
	float resultCoordinate[2];
	float calculateMatrix[3][3] = { 0, };

	for(int i = 0; i < 2; i ++)
		inputCoordinate[i] = torus[nCircle][nPoint][i];
	inputCoordinate[2] = 1;

	// init calculation matrix
	calculateMatrix[0][0] = cosTheta;
	calculateMatrix[0][1] = -sinTheta;
	calculateMatrix[0][2] = x * (1 - cosTheta) + y * sinTheta;
	calculateMatrix[1][0] = sinTheta;
	calculateMatrix[1][1] = cosTheta;
	calculateMatrix[1][2] = y * (1 - cosTheta) - x * sinTheta;
	calculateMatrix[2][2] = 1;

	for (int rol = 0; rol < 2; rol++)
	{
		float sum = 0;
		for (int col = 0; col < 3; col++)
		{
			sum += calculateMatrix[rol][col] * inputCoordinate[col];
		}
		resultCoordinate[rol] = sum;
	}
	
	return resultCoordinate;
}

/**
	Calculate torus's coordinates.
*/
void InitDrawTorus(float minorRadius, float majorRadius, float height)
{
	glColor3f(0, 0, 0);
	
	// set start point using minor radius
	float startPoint[3] = { 0,0,0};
	float* nextPoint;
	startPoint[0] = majorRadius + minorRadius;
	startPoint[1] = height;
	memcpy(torus[0][0], startPoint, sizeof(startPoint));

	for (int nPoint = 0; nPoint < 18; nPoint++)
	{	
		for (int nCircle = 0; nCircle < 36; nCircle++)
		{
			// rotate circle's coordinate by y-axis
			nextPoint = MajorRotate(nCircle , nPoint, 10.0);
			memcpy(torus[(nCircle + 1) % 36][nPoint], nextPoint, sizeof(float) * 3);
		}

		// rotate coordinate in circle
		nextPoint = MinorRotate(0, nPoint, 20.0, majorRadius, height);
		memcpy(torus[0][(nPoint + 1) % 18], nextPoint, sizeof(float) * 2);


	}

	startPoint[1] = startPoint[1];
}

/**
	Draw Coodrination system.
*/
void DrawCoordinationSystem(void)
{
	glBegin(GL_LINES);
	{
		// x-axis
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(10, 0, 0);
		// y-axis
		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 10, 0);
		// z-axis
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 10);
	}
	glEnd();
}

/**
	Draw torus as dots
*/
void DrawTorusAsDots(int nCircle, int nPoint)
{
	glColor3f(0, 0, 0);
	glBegin(GL_POINTS);
	{
		for (int circle = 0; circle < nCircle; circle++)
		{
			for (int coordinate = 0; coordinate < nPoint; coordinate++)
			{
				glVertex3f(torus[circle][coordinate][0], torus[circle][coordinate][1], torus[circle][coordinate][2]);
			}
		}
	}
	glEnd();
}

/**
	Draw torus as lines
*/
void DrawTorusAsLines(int nCircle, int nPoint, float epsilon)
{
	glColor3f(0, 0, 0);
	for (int circle = 0; circle < nCircle; circle++)
	{
		glBegin(GL_LINE_STRIP);
		{
			for (int coordinate = 0; coordinate < nPoint; coordinate++)
			{
				glVertex3f(torus[circle][coordinate][0], torus[circle][coordinate][1], torus[circle][coordinate][2]);
			}
			glVertex3f(torus[circle][0][0], torus[circle][0][1], torus[circle][0][2]);
		}
		glEnd();
	}

	for (int coordinate = 0; coordinate < nPoint; coordinate++)
	{
		glBegin(GL_LINE_STRIP);
		{
			for (int circle = 0; circle < nCircle; circle++)
			{
				glVertex3f(torus[circle][coordinate][0], torus[circle][coordinate][1], torus[circle][coordinate][2]);
			}
		}
		glEnd();
	}

}


void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
		
	gluLookAt(viewer[0], viewer[1], viewer[2], 0, 0, 0, 0, 1, 0);

	// draw cooddination system
	DrawCoordinationSystem();

	// draw torus as dots
	DrawTorusAsDots(36, 18);

	// draw torus as lines
	DrawTorusAsLines(36, 18, 0.0);


	glutSwapBuffers();
	glFlush();
}

void init(void)
{
	viewer[0] = 15;
	viewer[1] = 15;
	viewer[2] = 15;
}

void SetupRC(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	// set clear color
	glEnable(GL_DEPTH_TEST);

}

void ChangeSize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// calculate aspect ratio of the window
	gluPerspective(45.f, (GLfloat)w / h, 0.1f, 1000.f);

	//set modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}

void main(int argc, char* argv[])
{
	init();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Torus");
	InitDrawTorus(1.0, 3.0, 5.0);
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	SetupRC();	// Init Function
	glutMainLoop();
}