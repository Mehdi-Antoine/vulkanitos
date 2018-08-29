#define VULKAN_APPLICATION

#ifdef VULKAN_APPLICATION
    #include "VulkanApplication.h"
#else
    #include "GlApplication.h"
#endif

#include <iostream>
#include <stdexcept>

int main()
{
#ifdef VULKAN_APPLICATION
    Application* app = new VulkanApplication();
#else
    Application* app = new GlApplication();
#endif

    const auto modelPath = "models/chalet.obj";
    const auto texturePath = "models/chalet.jpg";

    try
    {
        app->loadModel(modelPath);
        app->loadTexture(texturePath);
        app->run();
        app->releaseTexture();
    }
    catch (const std::exception& e)
    {
        std::cerr << std::endl << "ERROR: " << e.what() << std::endl;

        char c;
        std::cout << "enter key to continue..." << std::endl;
        std::cin >> c;
        return EXIT_FAILURE;
    }

    char c;
    std::cout << "enter key to continue..." << std::endl;
    std::cin >> c;
    return EXIT_SUCCESS;
}