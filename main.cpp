#include "main.h"
#include "curve.h"
#include "shader.h"
#include "curveMesh.h"
#include <filesystem>
#include <stack>
#include "PalmTree.h"
// Do not include imgui loader.h!

using namespace std;

/*resize the viewport to the window when the user resizes*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
   glViewport(0, 0, width, height);
}

/* input control callback function*/
void processInput(GLFWwindow *window)
{
   if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
       glfwSetWindowShouldClose(window, true);
}

//void CreateTesselationShader() {
//    util::shaderFilePathBundle filepaths;
//
//    filepaths.vertex = "shaders/curve_vertex.txt";
//    filepaths.geometry = NULL;
//    filepaths.tcs = "shaders/curve_tcs.txt";
//    filepaths.tes = "shaders/curve_tes.txt";
//    filepaths.fragment = "shaders/curve_fragment.txt";
//    curveShader = util::load_shader(filepaths);
//}


int main(void)
{
    GLFWwindow* window;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "L Systems", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }


    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    App app;
    app.init(window);

    glfwSetKeyCallback(window, &app.keyCallback);

    // Initialise GLAD before calling any OpenGL Function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 1920, 1080);
    
   
    //if(glewInit() != GLEW_OK){
    //    cout << "Unable to load glew" << endl;
    //}
    //else{
    //    cout << glGetString(GL_VERSION) << endl;
    //}

   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Created vertices for buffer object
    float vertices[] = {
        // first triangle
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f,
        // second triangle
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };

 // SCENE

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);


   // Created vertex for element buffer object
   float firstSquare[] = {
    0.1f,  0.1f, 0.0f,  // top right
    0.1f, -0.9f, 0.0f,  // bottom right
   -0.9f, -0.9f, 0.0f,  // bottom left
   -4.9f,  15.9f, 0.0f   // top left 
   };
   unsigned int firstSquareIndices[] = {  // note that we start from 0!
       3, 1, 0,   // first triangle
       3, 2, 1   // second triangle
   };


   


// SHADERS


       // Curve Shader


       util::shaderFilePathBundle standardPaths;
       standardPaths.vertex = "shaders/vertex.glsl";
       standardPaths.geometry = nullptr;
       standardPaths.tcs = nullptr;
       standardPaths.tes = nullptr;
       standardPaths.fragment = "shaders/fragment.glsl";

       GLuint standardShader = util::load_shader(standardPaths);


       util::shaderFilePathBundle curvePaths;
       curvePaths.vertex = "shaders/curve_vertex.glsl";
       curvePaths.geometry = nullptr;
       curvePaths.tcs = "shaders/curve_tesselation-control.glsl";
       curvePaths.tes = "shaders/curve_tesselation-evaluation.glsl";
       curvePaths.fragment = "shaders/curve_fragment.glsl";

       GLuint curveShader = util::load_shader(curvePaths);

//SCENE OBJECTS

       Renderer renderer(standardShader, curveShader);
       Camera camera;

       camera.Position = { 0.0f, 0.0f, 10.0f };

	   // Create Cube Mesh

       Mesh mesh;

       mesh.init();
	   mesh.lines = vector<float>(begin(firstSquare), end(firstSquare));
	   mesh.indices = vector<unsigned int>(begin(firstSquareIndices), end(firstSquareIndices));
	   mesh.uploadMeshToGPU();
       
       Transform cubeTransform;

       cubeTransform.Position = {-4.5f, 0.0f, -5.0f };
       cubeTransform.Scale = { 35.5f, 15.5f, 15.5f };
       cubeTransform.Rotation = { -1.0f, 0.0f, 0.0f };

	   //Create Palm Tree Trunk Curve Mesh
       int numLeaves = 7;
       int numLeaflets = 8;
       float radius = 1.5f;
;

       PalmTree palmTree(numLeaves, numLeaflets, radius);

       
// RENDER LOOP
       /* Loop until the user closes the window */
       while (!glfwWindowShouldClose(window))
       {
           //input
           processInput(window);

           /* Poll for and process events, user inputs */
           glfwPollEvents();

           /* Render here */
           glClearColor(0.2f, 0.4f, 0.7f, 1.0f); // background color
           glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

           /*Code To print First Square*/
           // 2. use our shader program when we want to render an object

		   app.gui.draw(palmTree, renderer);

		   /* Draw Cube Mesh*/
		   renderer.Draw(mesh, cubeTransform, camera);

       /* Draw Curve Meshes*/

		   palmTree.update(renderer, camera);

           

           //Draw UI
           app.gui.UIrender();

           /* Swap front and back buffers */
           glfwSwapBuffers(window);
       }

   
   // optional: de-allocate all resources once they've outlived their purpose:
   // ------------------------------------------------------------------------
   app.release();


    glfwTerminate();
    return 0;
}
