
#include "Glew_Initialization.h"
#include "shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "ColorPicking.h"

Shaders shader; // main shader program
Shaders pickingshader; // picking color shader program

Color_Picking colorPicking;

Glew_Initialization sys;

std::vector<Vertex> cube_vertex =
{
	// positions          // colors           // normals         // texUV
 { glm::vec3(-0.3,-0.3,-0.3), glm::vec3(1.0,0.0,0.5), glm::vec3(-1,-1,-1), glm::vec2(0, 0) },
 { glm::vec3(-0.3,-0.3,0.3), glm::vec3(1.0,0.0,0.5), glm::vec3(-1,-1, 1), glm::vec2(1, 0) },
 { glm::vec3(-0.3,0.3,-0.3),  glm::vec3(1.0,0.0,0.5), glm::vec3(-1, 1,-1), glm::vec2(0, 1) },
 { glm::vec3(-0.3,0.3,0.3),  glm::vec3(1.0,0.0,0.5), glm::vec3(-1, 1, 1), glm::vec2(1, 1) },
 { glm::vec3(0.3,-0.3,-0.3),  glm::vec3(1.0,0.0,0.5), glm::vec3(1,-1,-1),  glm::vec2(1, 0) },
 { glm::vec3(0.3,-0.3,0.3),   glm::vec3(1.0,0.0,0.5), glm::vec3(1,-1, 1),  glm::vec2(0, 0) },
 { glm::vec3(0.3,0.3,-0.3),  glm::vec3(1.0,0.0,0.5) , glm::vec3(1, 1,-1),  glm::vec2(1, 1) },
 { glm::vec3(0.3,0.3,0.3),    glm::vec3(1.0,0.0,0.5), glm::vec3(1, 1, 1),  glm::vec2(0, 1) }
};
std::vector<unsigned int> indices =
{
   0, 1, 2, 1, 2, 3, 4, 5, 6, 5, 6, 7, // front 
   1, 5, 3, 5, 3, 7, 0, 4, 2, 4, 2, 6, // back 
   0, 1, 4, 1, 4, 5, 2, 3, 6, 3, 6, 7  // sides
};

std::vector<mesh*> All_Mesh;


glm::mat4 CamMatrix;

void Render_Scene();
void Render_Picking_Scene();
int Get_Object(glm::vec4 ColorID);
bool Almost_Equal(glm::vec4 a, glm::vec4 b);
void InitiatePicking();

bool leftclicked = false;
int Cube_ID=0;
int main()
{
	Camera camera;

	
	sys.Init_Glfw();
	shader.Create_Normal_Shaders();
	pickingshader.Create_Picking_Shaders();

	// create custome framebuffer
	colorPicking.Init(sys.width, sys.height, glm::vec3(0, 0, 0)); 

	//create cubes and assign unique ColorIDs
	mesh* cube1 = new mesh;
	cube1->CreateMesh(cube_vertex, indices);
	cube1->position = glm::vec3(0, 0, 0);
	cube1->ColorID = glm::vec4(1, 0, 0, 0);
	All_Mesh.push_back(cube1);

	mesh* cube2 = new mesh;
	cube2->CreateMesh(cube_vertex, indices);
	cube2->position = glm::vec3(1, 0, 0);
	cube2->ColorID = glm::vec4(0, 1, 0, 0);
	All_Mesh.push_back(cube2);

	mesh* cube3 = new mesh;
	cube3->CreateMesh(cube_vertex, indices);
	cube3->position = glm::vec3(0.3, 1, 0);
	cube3->ColorID = glm::vec4(0, 0, 1, 0);
	All_Mesh.push_back(cube3);

	mesh* cube4 = new mesh;
	cube4->CreateMesh(cube_vertex, indices);
	cube4->position = glm::vec3(-02, 0, 0);
	cube4->ColorID = glm::vec4(1, 0, 1, 0);
	All_Mesh.push_back(cube4);

	







	 

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(MainWindow))
	{
		glfwPollEvents();
		CamMatrix = camera.Camera_Matrix();
		camera.Camera_Controls(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		Render_Scene();

		InitiatePicking();

		
		
		
		glfwSwapBuffers(MainWindow);

	}
	glfwDestroyWindow(MainWindow);
	glfwTerminate();
}
void Render_Scene()
{
	for (int i = 0; i < All_Mesh.size(); i++)
	{
		All_Mesh[i]->RenderMesh(shader.shaderprogram, CamMatrix);
		if (Cube_ID == i)
		{
			All_Mesh[i]->scale = glm::vec3(0.5, 0.5, 0.5);
		}
		else
		{
			All_Mesh[i]->scale = glm::vec3(1, 1, 1);
		}
	}
}
void Render_Picking_Scene()
{
	for (int i = 0; i < All_Mesh.size(); i++)
	{
		All_Mesh[i]->RenderMesh(pickingshader.shaderprogram, CamMatrix);
	}
}
void InitiatePicking()
{
	if (glfwGetMouseButton(MainWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS )
	{
		if (!leftclicked)
		{
			Render_Scene();
			Render_Picking_Scene();
			//draw cubes in the custom frame buffer
			colorPicking.EnableWriting();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			
			Render_Picking_Scene();
			colorPicking.DisableWriting();
			//read color values from custom frame buffer
			Cube_ID = Get_Object(colorPicking.Read_Pixels());

			leftclicked = true;
		}

	}
	else if (glfwGetMouseButton(MainWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		if (leftclicked)
		{
			leftclicked = false;
		}

	}
}
int Get_Object(glm::vec4 ColorID)
{
	int e = 0;
	for (int i = 0; i < All_Mesh.size(); i++)
	{
		if (Almost_Equal(All_Mesh[i]->ColorID, ColorID))
		{
			e = i;
			break;

		}
	}
	return e;

}
bool Almost_Equal(glm::vec4 a, glm::vec4 b)
{
	double epsilon = 0.0001;
	bool check_diff = true;
	for (int i = 0; i < 4; i++)
	{
		if (std::abs(a[i] - b[i]) > epsilon)
		{
			check_diff = false;
		}
	}
	return check_diff;
}