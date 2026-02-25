#include "App.h"



    void App::init (GLFWwindow* window) {
        glfwSetWindowUserPointer(window, this);

        // Set the key callback to our static wrapper
        glfwSetKeyCallback(window, keyCallback);
        //gui.init(window);
    }

    void App::release () {
        gui.cleanup();
        for (int i = 0; i < meshes.size(); i++) {
            meshes[i].release();
        }
	}


    void App::onKey(int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS) {
            // Switch L-System
            if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT) {
                
            }

            if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) {
                
            }

            //Adjust Iterations
            if (key == GLFW_KEY_W || key == GLFW_KEY_UP) {

            }

            //Adjust Iterations
            if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) {
                
            }

            
        }

        selectActiveLSystem(activeIndex);
    }

    void App::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        // Get the instance from the user pointer
        App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        if (app) {
            app->onKey(key, scancode, action, mods);
        }
    }

    void App::selectActiveLSystem(int index) {
        //trees[index].update
    }

    void App::updateActiveLSystem(string Lsystem, float unitLength, float angleDeg) {
        //trees[activeIndex].updateLSystem();
    }
