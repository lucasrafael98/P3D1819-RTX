///////////////////////////////////////////////////////////////////////
//
// P3D Course
// (c) 2016 by Joao Madeiras Pereira
// TEMPLATE: Whitted Ray Tracing NFF scenes and drawing points with Modern OpenGL
//
///////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <stdio.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Scene.h"
#include "Color.h"
#include "Ray.h"
#include "Camera.h"

#define CAPTION "RTX ON"

#define VERTEX_COORD_ATTRIB 0
#define COLOR_ATTRIB 1

#define GRID_ON true
// 0/1/2: off/jitter/montecarlo
#define AA_MODE 1
// 0/1/2: off/random/area/area2
#define SOFT_SHADOWS 2

#define MAX_DEPTH 6
#define SAMPLES 2
#define AREA_LIGHT 0.1

// NOTE: Edit this to NFF/<your file>.nff to change the nff being parsed.
#define NFF "NFF/balls_low.nff"

// Points defined by 2 attributes: positions which are stored in vertices array and colors which are stored in colors array
float *colors;
float *vertices;

int size_vertices;
int size_colors;

GLfloat m[16];  //projection matrix initialized by ortho function

GLuint VaoId;
GLuint VboId[2];

GLuint VertexShaderId, FragmentShaderId, ProgramId;
GLint UniformId;

Scene* scene = NULL;
int RES_X, RES_Y;

/* Draw Mode: 0 - point by point; 1 - line by line; 2 - full frame */
int draw_mode=1; 

int WindowHandle = 0;

bool draw = true;

///////////////////////////////////////////////////////////////////////  RAY-TRACE SCENE

Ray computePrimaryRay(float x, float y){
	Vector3 rayOrigin = Vector3(scene->getCamera()->getEye());
	Vector3 rayDirection = scene->getCamera()->computeRayDirection(x,y);
	return Ray(rayOrigin.getX(), rayOrigin.getY(), rayOrigin.getZ(),
				rayDirection.getX(), rayDirection.getY(), rayDirection.getZ());
}

static float getShadow(const Vector3 *point, const Light *light, const std::vector<SceneObject*> &objects) {
	Vector3 shadowRayDirection;
	if(SOFT_SHADOWS == 1)
		shadowRayDirection =
			 Vector3(light->getPosition()->getX() * ((double)rand() / (RAND_MAX)), 
			 		light->getPosition()->getY() * ((double)rand() / (RAND_MAX)), 
					light->getPosition()->getZ()) - *point;
	else 
		shadowRayDirection = Vector3(*(light->getPosition()) - *point);
	shadowRayDirection.normalize();
	// 0.0001f to avoid self-intersection
	Vector3 shadowRayOrigin(*point + *(light->getPosition()) * 0.0001f);
	Ray shadowRay(shadowRayOrigin.getX(), shadowRayOrigin.getY(), shadowRayOrigin.getZ(), 
				shadowRayDirection.getX(), shadowRayDirection.getY(), shadowRayDirection.getZ());
	if(GRID_ON){
		float ti = INFINITY;
		if(scene->getGrid()->intersect(shadowRay, ti))
			return 1.0f;
	}
	else{
		for (unsigned int j = 0; j < objects.size(); j++) {
			float ti = INFINITY;
			if (objects[j]->intersect(shadowRay, ti)){
				return 1.0f;
			}
		}
	}
	for(Plane* p: scene->getPlanes()){
		float ti = INFINITY;
		if(p->intersect(shadowRay, ti)) return 1.0f;
	}
	return 0.0f;
}

static float getShadowFactor(const Vector3 *point, const Light *light, const std::vector<SceneObject*> &objects) {
	if(SOFT_SHADOWS == 3){
		//std::vector<Vector3> r;
		std::vector<Vector3> s;
		for (int p = 0; p < SAMPLES; p++) {
			for (int q = 0; q < SAMPLES; q++) {
				/*float randomFactor = ((float)rand() / (RAND_MAX)); //0 < random < 1
				r.push_back(Vector3(point->getX() + ((p + randomFactor)/SAMPLES*AREA_LIGHT),
										point->getY() + ((q + randomFactor)/SAMPLES*AREA_LIGHT),
										point->getZ()));*/
				float randomFactor = ((float)rand() / (RAND_MAX)); //0 < random < 1
				s.push_back(Vector3(light->getPosition()->getX() + ((p + randomFactor) * AREA_LIGHT),
										light->getPosition()->getY() + ((q + randomFactor) * AREA_LIGHT),
										light->getPosition()->getZ()));
			}
		}
		for(int i = s.size() - 1; i!=-1; i--){
			int j = (double)rand() / RAND_MAX * i;
			Vector3 temp = s.at(i);
			s.at(i) = s.at(j);
			s.at(j) = temp;
		}
		float shadowFactor = 0.0f;
		for(unsigned int i = 0; i != s.size(); i++){
			//point = r.at(i);
			Light sl = Light(s.at(i).getX(), s.at(i).getY(), s.at(i).getZ(),
							light->getColor()->getR(), light->getColor()->getG(), light->getColor()->getB());
			shadowFactor += getShadow(point, &sl, objects);
		}
		shadowFactor /= s.size();
		return shadowFactor;
	}
	else
		return getShadow(point, light, objects);
}

Color getLighting(const SceneObject &object, const Vector3 *point, const Vector3 &normal, const Vector3 &view, const Light *light) {
	Color rayColor;
	// Create diffuse color
	Vector3 N(normal);

	Vector3 L(*(light->getPosition()) - *point);
	float distance = L.length();
	L.normalize();
	float attenuate = light->attenuate(distance);

	float NdotL = N.dot(L);
	float intensity = std::max(0.0f, NdotL);
	Color diffuse = *(object.getMaterial()->getColor()) * *(light->getColor()) * intensity * attenuate;

	// Create specular color
	Vector3 V(view);
	Vector3 H(L + V);
	H.normalize();

	float shininess = object.getMaterial()->getShininess();
	float NdotH = N.dot(H);
	float specularIntensity = pow(std::max(0.0f, NdotH), shininess);
	Color specular = *(object.getMaterial()->getColor()) * *(light->getColor()) * specularIntensity * attenuate;

	rayColor = diffuse * object.getMaterial()->getDiffuse() + specular * object.getMaterial()->getSpecular();
	return rayColor;
}

Color getMLighting(const SceneObject &object, const Vector3 *point, const Vector3 &normal, const Vector3 &view,
					 const std::vector<Light*> &lights, const std::vector<SceneObject*> &objects) {
	Color rayColor;
	// Compute illumination with shadows
	for (unsigned int i = 0; i < lights.size(); i++) {
		float shadowFactor = getShadowFactor(point, lights[i], objects);
		rayColor = rayColor + getLighting(object, point, normal, view, lights[i]) * (1.0 - shadowFactor);
	}
	
	return rayColor;
}

Color rayTracing( Ray ray, int depth, float RefrIndex, const std::vector<Light*> lights)
{
	if(depth > MAX_DEPTH) return *(scene->getBGColor());
	Color rayColor;
	SceneObject* hit = nullptr;
	float tnear = INFINITY;
	if(GRID_ON){
		hit = scene->getGrid()->intersect(ray, tnear);
		// tNear gets updated with the grid's BB even if no objects are hit. We don't want that. 
		if(!hit) tnear = INFINITY;
	}
	else{
		std::vector<SceneObject*> objects = scene->getObjectVector();
		for(SceneObject* so: objects){
			float ti = INFINITY;
			if(so->intersect(ray, ti)){
				if (ti < tnear) {
					tnear = ti;
					hit = so;
				}
			}
		}
	}
	// We check plane intersections independently of grid being on or not.
	// This because planes are infinite and can't be contained in a BB for grid traversal.
	std::vector<Plane*> planes = scene->getPlanes();
	for(Plane* so: planes){
		float ti = INFINITY;
		if(so->intersect(ray, ti)){
			if (ti < tnear) {
				tnear = ti;
				hit = so;
			}
		}
	}
	if (!hit) {
		return *(scene->getBGColor());
	}
	Vector3 hitPoint(ray.getOrigin() + ray.getDirection() * tnear);
	Vector3 N(hit->getNormal(hitPoint));
	N.normalize();
	Vector3 V(*(scene->getCamera()->getEye()) - hitPoint);
	V.normalize();
	rayColor = rayColor + getMLighting(*hit, &hitPoint, N, V, lights, scene->getObjectVector());

	Vector3 dir(ray.getDirection());
	float RdotN = dir.dot(N);
	float rIndexBefore = RefrIndex; //not sure about this one...
	float rIndexDest = 1.0;
	if(RefrIndex != 1.0){ //inside, swap refraction index
		N = -N;
	}
	else { //outside
		RdotN = -RdotN;
		rIndexDest = hit->getMaterial()->getRefractionIndex();
	}
	
	// If there's Ks, material is reflective.
	if(hit->getMaterial()->getSpecular() > 0){
		Vector3 R = ray.getDirection() - N * 2 * ray.getDirection().dot(N);
		R.normalize();

		Ray rRay(hitPoint + N * 0.0001f, R);
		//float VdotR =  std::max(0.0f, V.dot(-R)); unused var warning
		Color reflectionColor = rayTracing(rRay,  depth + 1, RefrIndex, lights); //* VdotR;
		rayColor = rayColor + reflectionColor * hit->getMaterial()->getSpecular();
	}

	// If there's transmittance, material is transparent.
	if(hit->getMaterial()->getTransmittance() > 0){
		
		
		float snellResult = rIndexBefore / rIndexDest; //snell law
		float totalInternalReflectionFactor = (1 - pow(snellResult, 2) * (1 - pow(RdotN, 2)));

		if (!(totalInternalReflectionFactor < 0)) { //its not a total internal reflection
			Vector3 R((ray.getDirection() * snellResult) + (N * (snellResult * RdotN - sqrt(totalInternalReflectionFactor))));
			R.normalize();

			Ray rRay(hitPoint + R * 0.0001f, R);
			Color refractionColor = rayTracing(rRay, depth + 1, rIndexDest, lights);

			rayColor = rayColor + refractionColor * hit->getMaterial()->getTransmittance();
		}
	}
	return rayColor;
}

Color jittering(int x, int y) {
	Ray ray;
	Color color = Color(0.0, 0.0, 0.0);
	int i = 0;
	for (int p = 0; p < SAMPLES; p++) {
		for (int q = 0; q < SAMPLES; q++) {
			std::vector<Light*> lights;
			if(SOFT_SHADOWS == 2){
				for(Light* l: scene->getLights()){
					Vector3* altpos = l->getAlternatePos(i);
					lights.push_back(new Light(altpos->getX(), altpos->getY(), altpos->getZ(),
												l->getColor()->getR(), l->getColor()->getG(), l->getColor()->getB()));
				}
			}
			else lights = scene->getLights();
			float randomFactor = ((float)rand() / (RAND_MAX)); //0 < random < 1
			ray = computePrimaryRay(x + ((p + randomFactor) / SAMPLES), y + ((q + randomFactor) / SAMPLES));
			color = color + rayTracing(ray, 1, 1.0, lights);
			if(SOFT_SHADOWS == 2) for(Light* al: lights) delete al;
			i++;
		}
	}
	return Color(color.getR() / i, color.getG() / i, color.getB() / i);
}

/////////////////////////////////////////////////////////////////////// ERRORS

bool isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}

void checkOpenGLError(std::string error)
{
	if(isOpenGLError()) {
		std::cerr << error << std::endl;
		exit(EXIT_FAILURE);
	}
}
/////////////////////////////////////////////////////////////////////// SHADERs
const GLchar* VertexShader =
{
	"#version 330 core\n"

	"in vec2 in_Position;\n"
	"in vec3 in_Color;\n"
	"uniform mat4 Matrix;\n"
	"out vec4 color;\n"

	"void main(void)\n"
	"{\n"
	"	vec4 position = vec4(in_Position, 0.0, 1.0);\n"
	"	color = vec4(in_Color, 1.0);\n"
	"	gl_Position = Matrix * position;\n"

	"}\n"
};

const GLchar* FragmentShader =
{
	"#version 330 core\n"

	"in vec4 color;\n"
	"out vec4 out_Color;\n"

	"void main(void)\n"
	"{\n"
	"	out_Color = color;\n"
	"}\n"
};

void createShaderProgram()
{
	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShaderId, 1, &VertexShader, 0);
	glCompileShader(VertexShaderId);

	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderId, 1, &FragmentShader, 0);
	glCompileShader(FragmentShaderId);

	ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);

	glBindAttribLocation(ProgramId, VERTEX_COORD_ATTRIB, "in_Position");
	glBindAttribLocation(ProgramId, COLOR_ATTRIB, "in_Color");
	
	glLinkProgram(ProgramId);
	UniformId = glGetUniformLocation(ProgramId, "Matrix");

	checkOpenGLError("ERROR: Could not create shaders.");
}

void destroyShaderProgram()
{
	glUseProgram(0);
	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);

	glDeleteShader(FragmentShaderId);
	glDeleteShader(VertexShaderId);
	glDeleteProgram(ProgramId);

	checkOpenGLError("ERROR: Could not destroy shaders.");
}
/////////////////////////////////////////////////////////////////////// VAOs & VBOs
void createBufferObjects()
{
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	glGenBuffers(2, VboId);
	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);

	/* Nao e necessario fazer glBufferData, ou seja o envio dos pontos para a placa grafica pois isso 
	e feito na drawPoints em tempo de execucao com GL_DYNAMIC_DRAW */

	glEnableVertexAttribArray(VERTEX_COORD_ATTRIB);
	glVertexAttribPointer(VERTEX_COORD_ATTRIB, 2, GL_FLOAT, 0, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
	glEnableVertexAttribArray(COLOR_ATTRIB);
	glVertexAttribPointer(COLOR_ATTRIB, 3, GL_FLOAT, 0, 0, 0);
	
	// unbind the VAO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// commented due to GPU problems (?)
	//glDisableVertexAttribArray(VERTEX_COORD_ATTRIB);
	//glDisableVertexAttribArray(COLOR_ATTRIB);
	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void destroyBufferObjects()
{
	glDisableVertexAttribArray(VERTEX_COORD_ATTRIB);
	glDisableVertexAttribArray(COLOR_ATTRIB);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteBuffers(1, VboId);
	glDeleteVertexArrays(1, &VaoId);
	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

void drawPoints()
{
	glBindVertexArray(VaoId);
	glUseProgram(ProgramId);

	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
	glBufferData(GL_ARRAY_BUFFER, size_vertices, vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
	glBufferData(GL_ARRAY_BUFFER, size_colors, colors, GL_DYNAMIC_DRAW);

	glUniformMatrix4fv(UniformId, 1, GL_FALSE, m);
	glDrawArrays(GL_POINTS, 0, RES_X*RES_Y);
	glFinish();

	glUseProgram(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	checkOpenGLError("ERROR: Could not draw scene.");
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

// Render function by primary ray casting from the eye towards the scene's objects

void renderScene()
{
	if(!draw) return;
	int index_pos=0;
	int index_col=0;
	Color color;

	for (int y = 0; y < RES_Y; y++)
	{
		for (int x = 0; x < RES_X; x++)
		{
			if(AA_MODE == 1)
				color = jittering(x, y);
			else{
				Ray ray = computePrimaryRay(x, y);
				color = rayTracing(ray, 1, 1.0, scene->getLights());
			}	
			vertices[index_pos++]= (float)x;
			vertices[index_pos++]= (float)y;
			colors[index_col++]= (float)color.getR();
			colors[index_col++]= (float)color.getG();
			colors[index_col++]= (float)color.getB();	

			if(draw_mode == 0) {  // desenhar o conteudo da janela ponto a ponto
				drawPoints();
				index_pos=0;
				index_col=0;
			}
		}
		//printf("lineno %d; ", y);
		if(draw_mode == 1) {  // desenhar o conteudo da janela linha a linha
				drawPoints();
				index_pos=0;
				index_col=0;
		}
	}

	if(draw_mode == 2) //preenchar o conteudo da janela com uma imagem completa
		 drawPoints();

	Sphere::printTotalIntersections();
	printf("All done!\n"); 
	draw = false;
}

void cleanup()
{
	delete scene;
	destroyShaderProgram();
	destroyBufferObjects();
}

void ortho(float left, float right, float bottom, float top, 
			float nearp, float farp)
{
	m[0 * 4 + 0] = 2 / (right - left);
	m[0 * 4 + 1] = 0.0;
	m[0 * 4 + 2] = 0.0;
	m[0 * 4 + 3] = 0.0;
	m[1 * 4 + 0] = 0.0;
	m[1 * 4 + 1] = 2 / (top - bottom);
	m[1 * 4 + 2] = 0.0;
	m[1 * 4 + 3] = 0.0;
	m[2 * 4 + 0] = 0.0;
	m[2 * 4 + 1] = 0.0;
	m[2 * 4 + 2] = -2 / (farp - nearp);
	m[2 * 4 + 3] = 0.0;
	m[3 * 4 + 0] = -(right + left) / (right - left);
	m[3 * 4 + 1] = -(top + bottom) / (top - bottom);
	m[3 * 4 + 2] = -(farp + nearp) / (farp - nearp);
	m[3 * 4 + 3] = 1.0;
}

void reshape(int w, int h)
{
    glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, w, h);
	ortho(0, (float)RES_X, 0, (float)RES_Y, -1.0, 1.0);
}

/////////////////////////////////////////////////////////////////////// SETUP
void setupCallbacks() 
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshape);
}

void setupGLEW() {
	glewExperimental = GL_TRUE;
	GLenum result = glewInit() ; 
	if (result != GLEW_OK) { 
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	} 
	//GLenum err_code = glGetError(); unused var warning
	printf ("Vendor: %s\n", glGetString (GL_VENDOR));
	printf ("Renderer: %s\n", glGetString (GL_RENDERER));
	printf ("Version: %s\n", glGetString (GL_VERSION));
	printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
}

void setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);
	
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	
	glutInitWindowPosition(640,100);
	glutInitWindowSize(RES_X, RES_Y);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glDisable(GL_DEPTH_TEST);
	WindowHandle = glutCreateWindow(CAPTION);
	if(WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	createShaderProgram();
	createBufferObjects();
	setupCallbacks();
}

int main(int argc, char* argv[])
{
	scene = new Scene(std::string(NFF));
	RES_X = scene->getCamera()->getResX();
	RES_Y = scene->getCamera()->getResY();
	if(SOFT_SHADOWS == 2)
		for(Light* l: scene->getLights())
			l->computeAreaLight(SAMPLES, AREA_LIGHT);

	if(draw_mode == 0) { // desenhar o conteudo da janela ponto a ponto
		size_vertices = 2*sizeof(float);
		size_colors = 3*sizeof(float);
		printf("DRAWING MODE: POINT BY POINT\n");
	}
	else if(draw_mode == 1) { // desenhar o conteudo da janela linha a linha
		size_vertices = 2*RES_X*sizeof(float);
		size_colors = 3*RES_X*sizeof(float);
		printf("DRAWING MODE: LINE BY LINE\n");
	}
	else if(draw_mode == 2) { // preencher o conteudo da janela com uma imagem completa
		size_vertices = 2*RES_X*RES_Y*sizeof(float);
		size_colors = 3*RES_X*RES_Y*sizeof(float);
		printf("DRAWING MODE: FULL IMAGE\n");
	}
	else {
		printf("Draw mode not valid \n");
		exit(0);
	}
	printf("resx = %d  resy= %d.\n", RES_X, RES_Y);

	vertices = (float*)malloc(size_vertices);
    if (vertices==NULL) exit (1);

	colors = (float*)malloc(size_colors);
    if (colors==NULL) exit (1);

	init(argc, argv);
	glutMainLoop();	
	exit(EXIT_SUCCESS);
}
///////////////////////////////////////////////////////////////////////
