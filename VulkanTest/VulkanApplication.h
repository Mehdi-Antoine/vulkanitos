#ifndef VulkanApplication_h__
#define VulkanApplication_h__

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Application.h"
#include <array>

struct GLFWwindow;

class VulkanApplication : public Application
{
protected:
    void initWindow() override;
    void initResources() override;
    void mainLoop() override;
    void cleanup() override;

protected:
    const int MAX_FRAMES_IN_FLIGHT = 1;

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_LUNARG_standard_validation",
    };

// #ifdef NDEBUG
#if 0
    const bool enableValidationLayers = false;
    const VkDebugReportFlagsEXT debugFlags = 0;

#else
    const VkDebugReportFlagsEXT debugFlags = 0
        //         | VK_DEBUG_REPORT_INFORMATION_BIT_EXT
        | VK_DEBUG_REPORT_WARNING_BIT_EXT
        | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT
        | VK_DEBUG_REPORT_ERROR_BIT_EXT
        //         | VK_DEBUG_REPORT_DEBUG_BIT_EXT
        | 0;

    const bool enableValidationLayers = true;

#endif

    VkInstance instance;
    VkDebugReportCallbackEXT callback;

    struct QueueFamilyIndices
    {
        int graphicsFamily = -1;
        int presentFamily = -1;
        int computeFamily = -1;
        bool isComplete()
        {
            return graphicsFamily >= 0 
                && presentFamily >= 0
                && computeFamily >= 0;
        }
    };

    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct ComputeData
    {
        float time;
        int vertexCount;
    };

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    VkDevice device;

    VkQueue graphicsQueue;

    VkQueue computeQueue;

    VkQueue presentQueue;

    VkSurfaceKHR surface;

    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;

    std::vector<VkImageView> swapChainImageViews;

    VkRenderPass renderPass = VK_NULL_HANDLE;

    VkDescriptorSetLayout graphicsDescriptorSetLayout;
    VkPipelineLayout graphicsPipelineLayout;
    VkPipeline graphicsPipeline;

    VkDescriptorSetLayout luminanceDescriptorSetLayout;
    VkPipelineLayout luminancePipelineLayout;
    VkPipeline luminancePipeline;

    VkDescriptorSetLayout computeDescriptorSetLayout;
    VkPipelineLayout computePipelineLayout;
    VkPipeline computePipeline;

    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkCommandPool graphicsCommandPool;

    VkCommandPool computeCommandPool;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    VkImage beautyImage;
    VkDeviceMemory beautyImageMemory;
    VkImageView beautyImageView;

    uint32_t mipLevels;
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;

    VkImageView textureImageView;

    VkSampler textureImageSampler;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    VkBuffer quadBuffer;
    VkDeviceMemory quadBufferMemory;

    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    std::vector<VkBuffer> graphicsUniformBuffers;
    std::vector<VkDeviceMemory> graphicsUniformBufferMemories;

    std::vector<VkBuffer> computeUniformBuffers;
    std::vector<VkDeviceMemory> computeUniformBufferMemories;

    VkDescriptorPool descriptorPool;

    std::vector<VkDescriptorSet> graphicsDescriptorSets;
    std::vector<VkDescriptorSet> luminanceDescriptorSets;
    std::vector<VkDescriptorSet> computeDescriptorSets;

    std::vector<VkCommandBuffer> graphicsCommandBuffers;

    std::vector<VkCommandBuffer> computeCommandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    VkFence computeFence;

    size_t currentFrame = 0;
protected:

    void ensureValidationLayerSupport();

    std::vector<const char*> getRequiredExtensions();

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugReportFlagsEXT flags,
        VkDebugReportObjectTypeEXT objType,
        uint64_t obj,
        size_t location,
        int32_t code,
        const char* layerPrefix,
        const char* msg,
        void* userData);

    void createInstance();

    void setupDebugCallback();

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    bool isDeviceSuitable(VkPhysicalDevice device);

    void pickPhysicalDevice();

    void createLogicalDevice();

    void createSurface();

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModes);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    void createSwapChain();

    void createImageViews();

    void createRenderPass();

    VkShaderModule createShaderModule(const std::vector<char>& code);

    void createGraphicsDescriptorSetLayout();

    void createLuminanceDescriptorSetLayout();

    void createComputeDescriptorSetLayout();

    void createGraphicsPipeline();

    void createLuminancePipeline();

    void createComputePipeline();

    void createFramebuffers();

    void createCommandPools();

    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    VkFormat findDepthFormat();

    bool hasStencilComponent(VkFormat format);

    void createDepthResources();

    void createBeautyResources();

    void createImage(uint32_t width,
                     uint32_t height,
                     uint32_t mipLevels,
                     VkFormat format,
                     VkImageTiling tiling,
                     VkImageUsageFlags usage,
                     VkMemoryPropertyFlags properties,
                     VkImage* image,
                     VkDeviceMemory* imageMemory);

    void transitionImageLayout(VkImage image,
                               VkFormat format,
                               VkImageLayout oldLayout,
                               VkImageLayout newLayout,
                               uint32_t mipLevels);

    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    void generateMipmaps(VkImage image, VkFormat format, uint32_t texWidth, uint32_t texHeight, uint32_t mipLevels);

    void createTextureImage();

    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);

    void createTextureImageView();

    void createTextureSampler();

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    void createBuffer(VkDeviceSize size, 
                      VkBufferUsageFlags usage, 
                      VkMemoryPropertyFlags properties, 
                      VkBuffer* buffer, 
                      VkDeviceMemory* bufferMemory);

    VkCommandBuffer beginSingleTimeCommands();

    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    void copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size);

    void createVertexBuffer();

    void createQuadBuffer();

    void createIndexBuffer();

    void createUniformBuffers();

    void createDescriptorPool();

    void updateGraphicsDescriptorSets();

    void createGraphicsDescriptorSets();

    void updateLuminanceDescriptorSets();

    void createLuminanceDescriptorSets();

    void updateComputeDescriptorSets();

    void createComputeDescriptorSets();

    void fillGraphicsCommandBuffers();

    void fillComputeCommandBuffers();

    void createCommandBuffers();

    void createSyncObjects();

    void cleanupSwapChain();

    void recreateSwapChain();

    void updateUniformBuffers(size_t imageIndex);

    void drawFrame();
};

VkVertexInputBindingDescription getVertexBindingDescription();
std::array<VkVertexInputAttributeDescription, 3> getVertexAttributeDescriptions();

VkVertexInputBindingDescription getQuadBindingDescription();
VkVertexInputAttributeDescription getQuadAttributeDescription();

VkResult CreateDebugReportCallbackEXT(VkInstance instance,
                                      const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
                                      const VkAllocationCallbacks* pAllocator,
                                      VkDebugReportCallbackEXT* pCallback
);

void DestroyDebugReportCallbackEXT(VkInstance instance,
                                   VkDebugReportCallbackEXT callback,
                                   const VkAllocationCallbacks* pAllocator);

#endif // VulkanApplication_h__
