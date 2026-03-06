#include "main.h"
#include "curve.h"
#include "shader.h"
#include "curveMesh.h"
#include <filesystem>
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
    std::cout << "Hello World" << std::endl;
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
   -0.9f,  0.9f, 0.0f   // top left 
   };
   unsigned int firstSquareIndices[] = {  // note that we start from 0!
       3, 1, 0,   // first triangle
       3, 2, 1   // second triangle
   };


   


// SHADERS


       // Curve Shader

        std::cout << "Current working directory: "
            << std::filesystem::current_path() << std::endl;

       util::shaderFilePathBundle standardPaths;
       standardPaths.vertex = "shaders/vertex.txt";
       standardPaths.geometry = nullptr;
       standardPaths.tcs = nullptr;
       standardPaths.tes = nullptr;
       standardPaths.fragment = "shaders/fragment.txt";

       GLuint standardShader = util::load_shader(standardPaths);


       util::shaderFilePathBundle curvePaths;
       curvePaths.vertex = "shaders/curve_vertex.txt";
       curvePaths.geometry = nullptr;
       curvePaths.tcs = "shaders/curve_tesselation-control.txt";
       curvePaths.tes = "shaders/curve_tesselation-evaluation.txt";
       curvePaths.fragment = "shaders/curve_fragment.txt";

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

       cubeTransform.Position = { 0.0f, 0.0f, 0.0f };
       cubeTransform.Scale = { 5.5f, 5.5f, 5.5f };

	   //Create Curve Mesh

       std::vector<glm::vec3> controlPoints = { {
           glm::vec3(0.5f, 0.0f, 0.5f),
           glm::vec3(-0.5f, 0.5f, 0.7f),
           glm::vec3(0.5f, -0.5f, 0.9f),
           glm::vec3(0.0f, 0.0f, 1.1f)
       } };

       Transform curveTransform;

       curveTransform.Position = { 0.0f, 0.0f, 10.0f };
       curveTransform.Scale = { 0.5f, 0.5f, 0.5f };

	   Curve curve(controlPoints, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

       CurveMesh curveMesh;
       curveMesh.init();
	   curveMesh.build(controlPoints);


       
// RENDER LOOP
       /* Loop until the user closes the window */
       while (!glfwWindowShouldClose(window))
       {
           //input
           processInput(window);

           /* Poll for and process events, user inputs */
           glfwPollEvents();

           /* Render here */
           glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // background color
           glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

           /*Code To print First Square*/
           // 2. use our shader program when we want to render an object

		   app.gui.draw(app.activeIndex, app.meshes);

		   /* Draw Cube Mesh*/
		   renderer.Draw(mesh, cubeTransform, camera);

           //camera.Rotation.y += 0.0001;
           //camera.Rotation.x -= 0.0001;

           /* Draw Curve Mesh*/

           renderer.DrawCurve(
               curveMesh,
               curveTransform,
               camera,
               glm::vec4(0.1f, 1.f, 0.1f, 1.0f));

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
