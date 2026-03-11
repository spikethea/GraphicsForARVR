#include "main.h"
#include "curve.h"
#include "shader.h"
#include "curveMesh.h"
#include <filesystem>
#include <stack>
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

glm::vec3 EvaluateBezier(
    const glm::vec3& P0,
    const glm::vec3& P1,
    const glm::vec3& P2,
    const glm::vec3& P3,
    float t)
{
    float u = 1.0f - t;

    float b0 = u * u * u;
    float b1 = 3 * u * u * t;
    float b2 = 3 * u * t * t;
    float b3 = t * t * t;

    return b0 * P0 +
        b1 * P1 +
        b2 * P2 +
        b3 * P3;
}

// evaluate the tangent of the leaf curve
glm::vec3 BezierTangent(
    const glm::vec3& P0,
    const glm::vec3& P1,
    const glm::vec3& P2,
    const glm::vec3& P3,
    float t)
{
    float u = 1.0f - t;

    return
        3.0f * u * u * (P1 - P0) +
        6.0f * u * t * (P2 - P1) +
        3.0f * t * t * (P3 - P2);
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

       cubeTransform.Position = { 0.0f, 0.0f, 0.0f };
       cubeTransform.Scale = { 5.5f, 5.5f, 5.5f };

	   //Create Palm Tree Trunk Curve Mesh

       std::vector<glm::vec3> controlPoints = { {
           glm::vec3(0.0f, 0.0f, 0.5f),
           glm::vec3(1.0f, 2.0f, 0.7f),
           glm::vec3(3.5f, 3.5f, 0.9f),
           glm::vec3(3.5f, 5.0f, 1.1f)
       } };

       glm::vec3 endPoint = controlPoints[1];

       // Palm Leaves

       Transform palmCrown;
       palmCrown.Position = endPoint;
       std::cout <<
           "x: " << palmCrown.Position.x <<
           "y: " << palmCrown.Position.y <<
           "z: " << palmCrown.Position.z <<
           std::endl;
       palmCrown.Scale = { 1.0f, 1.0f, 1.0f };

       std::vector<CurveMesh> leaves;

       std::vector <std::vector<CurveMesh>> leaflets;
       std::vector < std::vector<Transform>> leafletTransforms;

       int numLeaves = 7;
       int numLeaflets = 8;
       float radius = 1.5f;

       leaves.reserve(numLeaves);

       for (int i = 0; i < numLeaves; i++)
       {
           float angle = (float)i / (float)numLeaves * 2.0f * M_PI;

           float x = cos(angle) * radius;
           float z = sin(angle) * radius;

           std::vector<glm::vec3> leafcontrolPoints = { {
               glm::vec3(0.0f, 0.0f, 0.0f),// origin
               glm::vec3(x*0.5, 0.7f, z*0.7f),
               glm::vec3(x*0.5f, 1.3f, z*0.9f),
               glm::vec3(x*1.0f, 0.7f, z*1.1f) //endpoint
            } };

           
           leaves.emplace_back();
           leaves.back().build(leafcontrolPoints);

           //Leaflets Memory reserve
           leaflets.push_back({});
           leaflets.back().reserve(numLeaflets);
           leafletTransforms.push_back({});
           leafletTransforms.back().reserve(numLeaflets);

           

           for (int j = 0; j < numLeaflets; j++)
           {
               float t = (float)j / (numLeaflets - 1);

			   cout << "t: " << t << endl;

               glm::vec3 leafletStart = EvaluateBezier(
                   leafcontrolPoints[0],
                   leafcontrolPoints[1],
                   leafcontrolPoints[2],
                   leafcontrolPoints[3],
                   t
               );

               // find the tangent
               glm::vec3 tangent = glm::normalize(BezierTangent(
                   leafcontrolPoints[0],
                   leafcontrolPoints[1],
                   leafcontrolPoints[2],
                   leafcontrolPoints[3],
                   t
               ));

               glm::vec3 up = glm::vec3(0, 1, 0);

               glm::vec3 binormal = glm::normalize(glm::cross(up, tangent));
               glm::vec3 normal = glm::normalize(glm::cross(tangent, binormal));


               Transform transform;
               transform.Position = endPoint + leafletStart;
               transform.Rotation = binormal;
               transform.Scale = { 1.0f, 1.0f, 1.0f };
               leafletTransforms[i].emplace_back(transform);

               std::vector<glm::vec3> leafletControlPoints = { {
                   glm::vec3(-0.5, -0.7f, -0.7f),
                   glm::vec3(0.0f, 0.0f, 0.0f),// origin (using B-Splin)
                   glm::vec3(0.5f, 1.7f,  0.9f),
                   glm::vec3(1.5f, 0.3f, 1.1f) //endpoint
            } };

                leaflets[i].emplace_back();
                leaflets[i].back().build(leafletControlPoints);


           }

       }

       Transform curveTransform;

       curveTransform.Position = { 0.0f, 0.0f, 0.0f };
       curveTransform.Scale = { 1.0f, 1.0f, 1.0f };

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
           glClearColor(0.2f, 0.4f, 0.7f, 1.0f); // background color
           glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

           /*Code To print First Square*/
           // 2. use our shader program when we want to render an object

		   app.gui.draw(app.activeIndex, app.meshes);

		   /* Draw Cube Mesh*/
		   renderer.Draw(mesh, cubeTransform, camera);

           //camera.Rotation.y += 0.0001;
           //camera.Rotation.x -= 0.0001;

           /* Draw Curve Mesh*/

           float bezier[16] = {
            -1, 3,-3,1,
             3,-6, 3,0,
            -3, 3, 0,0,
             1, 0, 0,0
           };

           float hermite[16] = {
             2,-2, 1, 1,
            -3, 3,-2,-1,
             0, 0, 1, 0,
             1, 0, 0, 0
           };

           float bspline[16] = {
            -1.0 / 6,  3.0 / 6,-3.0 / 6,1.0 / 6,
             3.0 / 6, -6.0 / 6, 3.0 / 6,0,
            -3.0 / 6,  0,     3.0 / 6,0,
             1.0 / 6,  4.0 / 6, 1.0 / 6,0
           };

           renderer.DrawCurve(
               curveMesh,
               curveTransform,
               camera,
               glm::vec4(0.709f, 0.39f, 0.1f, 1.0f),
               hermite
           );

           for (const CurveMesh& leaf : leaves) {
               renderer.DrawCurve(
                   leaf,
                   palmCrown,
                   camera,
                   glm::vec4(0.2f, 1.0f, 0.2f, 1.0f),
                   bezier
               );

               for (const std::vector<CurveMesh>& leafletStem : leaflets) {
                   for (const CurveMesh& leaflet : leafletStem) {



                       for (int i = 0; i < leaves.size(); i++) {
                           for (int j = 0; j < leaflets.size(); j++) {

                               Transform transform = leafletTransforms[i][j];
                               
                               //Left Leaflets
                               renderer.DrawCurve(
                                   leaflet,
                                   transform,
                                   camera,
                                   glm::vec4(0.1f, 0.7f, 0.1f, 1.0f),
                                   bspline
                               );

                                //Right Leaflets

                               Transform flippedTransform = leafletTransforms[i][j];

                               glm::vec3 up = glm::vec3(0, 1, 0);

							   // Flip the binormal scale to get the opposite side of the leaf
                               flippedTransform.Scale.x = -flippedTransform.Scale.x;

                                //Left Leaflets
                                renderer.DrawCurve(
                                    leaflet,
                                    flippedTransform,
                                    camera,
                                    glm::vec4(0.1f, 0.7f, 0.1f, 1.0f),
                                    bspline
                                );

                            }
                       }
                       


                   }
               }
           }

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
