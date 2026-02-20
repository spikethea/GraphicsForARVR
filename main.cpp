#include "main.h"
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
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

//    // Created vertices for buffer object
//    float vertices[] = {
//        // first triangle
//        -0.5f, -0.5f, 0.0f,
//        0.5f, -0.5f, 0.0f,
//        0.0f,  0.5f, 0.0f,
//        // second triangle
//        0.5f, -0.5f, 0.0f,  // bottom right
//        -0.5f, -0.5f, 0.0f,  // bottom left
//        -0.5f,  0.5f, 0.0f   // top left
//    };

 // SCENE

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);

   // Set the number of vertices per patch for tessellation
   glPatchParameteri(GL_PATCH_VERTICES, 4);

   // Created vertex for element buffer object
   float firstSquare[] = {
    0.1f,  0.1f, 0.0f,  // top right
    0.1f, -0.9f, 0.0f,  // bottom right
   -0.9f, -0.9f, 0.0f,  // bottom left
   -0.9f,  0.9f, 0.0f   // top left 
   };
   unsigned int firstSquareIndices[] = {  // note that we start from 0!
       0, 1, 3,   // first triangle
       1, 2, 3    // second triangle
   };


   // VAO, VBOs and EBOs Array for the number of objects in the scene.
   unsigned int VBOs[2], VAOs[2], EBOs[2];
   glGenVertexArrays(2, VAOs); 
   glGenBuffers(2, VBOs);
   glGenBuffers(2, EBOs);


   
// First Square

    //Bind Vertex array object
    glBindVertexArray(VAOs[0]);
    // 0. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    // Linking Vertex Attributes location = 0, vec3
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstSquare), firstSquare, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(firstSquareIndices), firstSquareIndices, GL_STATIC_DRAW);

    // 1. then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);


   


       // SHADERS
       const char *vertexShaderSource = "#version 330 core\n"
           "layout (location = 0) in vec3 aPos;\n"
           "void main()\n"
           "{\n"
           "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
       "}\0";

       const char *fragmentShaderSource = "#version 330 core\n"
           "out vec4 FragColor;\n"
           "void main()\n"
           "{\n"
           "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
       "}\0";

       

       // initiate vertex shader
       unsigned int vertexShader;
       vertexShader = glCreateShader(GL_VERTEX_SHADER);

       glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
       glCompileShader(vertexShader);

       // check vertex compilation was successful
       int  success;
       char infoLog[512];
       glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

       if(!success)
       {
           glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
           std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
       }

       // initiate fragment shader and equivalent shader program
       unsigned int fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER); // the first fragment shader that outputs the color orange
       unsigned int shaderProgramOrange = glCreateProgram();
       
       // attach shader program to fragment shader & vertex shader
       glShaderSource(fragmentShaderOrange, 1, &fragmentShaderSource, NULL);
       glCompileShader(fragmentShaderOrange);
       glAttachShader(shaderProgramOrange, vertexShader);
       glAttachShader(shaderProgramOrange, fragmentShaderOrange);
       
       
       glLinkProgram(shaderProgramOrange);


       // COMPILE LOG CHECKS FOR SHADER TEST WORKING
       // check vertex compilation was successful
       int  fragmentSuccess;
       char fragmentInfoLog[512];
       glGetShaderiv(fragmentShaderOrange, GL_COMPILE_STATUS, &fragmentSuccess);

       if(!fragmentSuccess)
       {
           glGetShaderInfoLog(vertexShader, 512, NULL, fragmentInfoLog);
           std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragmentInfoLog << std::endl;
       } else {
           glGetShaderInfoLog(vertexShader, 512, NULL, fragmentInfoLog);
           std::cout << "SHADER::FRAGMENT::SUCCESS\n" << fragmentInfoLog << std::endl;
       }



       // Remove from memory as we dont need the shader object(s) once weve linked in the the program
       glDeleteShader(vertexShader);
       glDeleteShader(fragmentShaderOrange);

       app.gui.init(window);

//SCENE OBJECTS

       Renderer renderer(shaderProgramOrange);
       Camera camera;

       camera.Position = { 0.0f, 0.0f, 3.0f };

       Mesh mesh;

       mesh.init();
	   mesh.lines = vector<float>(begin(firstSquare), end(firstSquare));
	   mesh.indices = vector<unsigned int>(begin(firstSquareIndices), end(firstSquareIndices));
	   mesh.uploadMeshToGPU();

       Transform cubeTransform;

       cubeTransform.Position = { 0.0f, 0.0f, 0.0f };
       cubeTransform.Scale = { 0.5f, 0.5f, 0.5f };
       
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

           

		   renderer.Draw(mesh, cubeTransform, camera);
               
           //// 3. now draw the object
           glBindVertexArray(VAOs[0]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

           glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

           mesh.DrawMesh();

		   // UI NOT WORKING ATM
           app.gui.UIrender();

           /* Swap front and back buffers */
           glfwSwapBuffers(window);
       }

   
   // optional: de-allocate all resources once they've outlived their purpose:
   // ------------------------------------------------------------------------
   glDeleteVertexArrays(2, VAOs);
   glDeleteBuffers(2, VBOs);
   glDeleteBuffers(2, EBOs);
   app.release();
   glDeleteProgram(shaderProgramOrange);


    glfwTerminate();
    return 0;
}
