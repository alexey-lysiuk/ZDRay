#pragma once

#include "vulkanobjects.h"
#include "framework/zstring.h"
#include <cassert>

class ImageBuilder
{
public:
	ImageBuilder();

	ImageBuilder& Size(int width, int height, int miplevels = 1, int arrayLayers = 1);
	ImageBuilder& Samples(VkSampleCountFlagBits samples);
	ImageBuilder& Format(VkFormat format);
	ImageBuilder& Usage(VkImageUsageFlags imageUsage, VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_GPU_ONLY, VmaAllocationCreateFlags allocFlags = 0);
	ImageBuilder& MemoryType(VkMemoryPropertyFlags requiredFlags, VkMemoryPropertyFlags preferredFlags, uint32_t memoryTypeBits = 0);
	ImageBuilder& LinearTiling();
	ImageBuilder& DebugName(const char* name) { debugName = name; return *this; }

	bool IsFormatSupported(VulkanDevice *device, VkFormatFeatureFlags bufferFeatures = 0);

	std::unique_ptr<VulkanImage> Create(VulkanDevice *device, VkDeviceSize* allocatedBytes = nullptr);
	std::unique_ptr<VulkanImage> TryCreate(VulkanDevice *device);

private:
	VkImageCreateInfo imageInfo = {};
	VmaAllocationCreateInfo allocInfo = {};
	const char* debugName = nullptr;
};

class ImageViewBuilder
{
public:
	ImageViewBuilder();

	ImageViewBuilder& Type(VkImageViewType type);
	ImageViewBuilder& Image(VulkanImage *image, VkFormat format, VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT);
	ImageViewBuilder& DebugName(const char* name) { debugName = name; return *this; }

	std::unique_ptr<VulkanImageView> Create(VulkanDevice *device);

private:
	VkImageViewCreateInfo viewInfo = {};
	const char* debugName = nullptr;
};

class SamplerBuilder
{
public:
	SamplerBuilder();

	SamplerBuilder& AddressMode(VkSamplerAddressMode addressMode);
	SamplerBuilder& AddressMode(VkSamplerAddressMode u, VkSamplerAddressMode v, VkSamplerAddressMode w);
	SamplerBuilder& MinFilter(VkFilter minFilter);
	SamplerBuilder& MagFilter(VkFilter magFilter);
	SamplerBuilder& MipmapMode(VkSamplerMipmapMode mode);
	SamplerBuilder& Anisotropy(float maxAnisotropy);
	SamplerBuilder& MaxLod(float value);
	SamplerBuilder& DebugName(const char* name) { debugName = name; return *this; }

	std::unique_ptr<VulkanSampler> Create(VulkanDevice *device);

private:
	VkSamplerCreateInfo samplerInfo = {};
	const char* debugName = nullptr;
};

class BufferBuilder
{
public:
	BufferBuilder();

	BufferBuilder& Size(size_t size);
	BufferBuilder& Usage(VkBufferUsageFlags bufferUsage, VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_GPU_ONLY, VmaAllocationCreateFlags allocFlags = 0);
	BufferBuilder& MemoryType(VkMemoryPropertyFlags requiredFlags, VkMemoryPropertyFlags preferredFlags, uint32_t memoryTypeBits = 0);
	BufferBuilder& DebugName(const char* name) { debugName = name; return *this; }

	std::unique_ptr<VulkanBuffer> Create(VulkanDevice *device);

private:
	VkBufferCreateInfo bufferInfo = {};
	VmaAllocationCreateInfo allocInfo = {};
	const char* debugName = nullptr;
};

class ShaderBuilder
{
public:
	ShaderBuilder();

	ShaderBuilder& VertexShader(const FString &code);
	ShaderBuilder& FragmentShader(const FString &code);
	ShaderBuilder& RayGenShader(const FString& code);
	ShaderBuilder& IntersectShader(const FString& code);
	ShaderBuilder& AnyHitShader(const FString& code);
	ShaderBuilder& ClosestHitShader(const FString& code);
	ShaderBuilder& MissShader(const FString& code);
	ShaderBuilder& CallableShader(const FString& code);
	ShaderBuilder& DebugName(const char* name) { debugName = name; return *this; }

	std::unique_ptr<VulkanShader> Create(const char *shadername, VulkanDevice *device);

private:
	FString code;
	int stage = 0;
	const char* debugName = nullptr;
};

class AccelerationStructureBuilder
{
public:
	AccelerationStructureBuilder();

	AccelerationStructureBuilder& Type(VkAccelerationStructureTypeKHR type);
	AccelerationStructureBuilder& Buffer(VulkanBuffer* buffer, VkDeviceSize size);
	AccelerationStructureBuilder& Buffer(VulkanBuffer* buffer, VkDeviceSize offset, VkDeviceSize size);
	AccelerationStructureBuilder& DebugName(const char* name) { debugName = name; return *this; }

	std::unique_ptr<VulkanAccelerationStructure> Create(VulkanDevice* device);

private:
	VkAccelerationStructureCreateInfoKHR createInfo = { VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR };
	const char* debugName = nullptr;
};

class RayTracingPipelineBuilder
{
public:
	RayTracingPipelineBuilder();

	RayTracingPipelineBuilder& Layout(VulkanPipelineLayout* layout);
	RayTracingPipelineBuilder& MaxPipelineRayRecursionDepth(int depth);
	RayTracingPipelineBuilder& AddShader(VkShaderStageFlagBits stage, VulkanShader* shader);
	RayTracingPipelineBuilder& AddRayGenGroup(int rayGenShader);
	RayTracingPipelineBuilder& AddMissGroup(int missShader);
	RayTracingPipelineBuilder& AddTrianglesHitGroup(int closestHitShader, int anyHitShader = VK_SHADER_UNUSED_KHR);
	RayTracingPipelineBuilder& AddProceduralHitGroup(int intersectionShader, int closestHitShader, int anyHitShader);
	RayTracingPipelineBuilder& DebugName(const char* name) { debugName = name; return *this; }

	std::unique_ptr<VulkanPipeline> Create(VulkanDevice* device);

private:
	VkRayTracingPipelineCreateInfoKHR pipelineInfo = {};
	std::vector<VkPipelineShaderStageCreateInfo> stages;
	std::vector<VkRayTracingShaderGroupCreateInfoKHR> groups;
	const char* debugName = nullptr;
};

class ComputePipelineBuilder
{
public:
	ComputePipelineBuilder();

	ComputePipelineBuilder& Layout(VulkanPipelineLayout *layout);
	ComputePipelineBuilder& ComputeShader(VulkanShader *shader);
	ComputePipelineBuilder& DebugName(const char* name) { debugName = name; return *this; }

	std::unique_ptr<VulkanPipeline> Create(VulkanDevice *device);

private:
	VkComputePipelineCreateInfo pipelineInfo = {};
	VkPipelineShaderStageCreateInfo stageInfo = {};
	const char* debugName = nullptr;
};

class DescriptorSetLayoutBuilder
{
public:
	DescriptorSetLayoutBuilder();

	DescriptorSetLayoutBuilder& AddBinding(int binding, VkDescriptorType type, int arrayCount, VkShaderStageFlags stageFlags);
	DescriptorSetLayoutBuilder& DebugName(const char* name) { debugName = name; return *this; }

	std::unique_ptr<VulkanDescriptorSetLayout> Create(VulkanDevice *device);

private:
	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	TArray<VkDescriptorSetLayoutBinding> bindings;
	const char* debugName = nullptr;
};

class DescriptorPoolBuilder
{
public:
	DescriptorPoolBuilder();

	DescriptorPoolBuilder& MaxSets(int value);
	DescriptorPoolBuilder& AddPoolSize(VkDescriptorType type, int count);
	DescriptorPoolBuilder& DebugName(const char* name) { debugName = name; return *this; }

	std::unique_ptr<VulkanDescriptorPool> Create(VulkanDevice *device);

private:
	std::vector<VkDescriptorPoolSize> poolSizes;
	VkDescriptorPoolCreateInfo poolInfo = {};
	const char* debugName = nullptr;
};

class QueryPoolBuilder
{
public:
	QueryPoolBuilder();

	QueryPoolBuilder& QueryType(VkQueryType type, int count, VkQueryPipelineStatisticFlags pipelineStatistics = 0);
	QueryPoolBuilder& DebugName(const char* name) { debugName = name; return *this; }

	std::unique_ptr<VulkanQueryPool> Create(VulkanDevice *device);

private:
	VkQueryPoolCreateInfo poolInfo = {};
	const char* debugName = nullptr;
};

class FramebufferBuilder
{
public:
	FramebufferBuilder();

	FramebufferBuilder& RenderPass(VulkanRenderPass *renderPass);
	FramebufferBuilder& AddAttachment(VulkanImageView *view);
	FramebufferBuilder& AddAttachment(VkImageView view);
	FramebufferBuilder& Size(int width, int height, int layers = 1);
	FramebufferBuilder& DebugName(const char* name) { debugName = name; return *this; }

	std::unique_ptr<VulkanFramebuffer> Create(VulkanDevice *device);

private:
	VkFramebufferCreateInfo framebufferInfo = {};
	std::vector<VkImageView> attachments;
	const char* debugName = nullptr;
};

class GraphicsPipelineBuilder
{
public:
	GraphicsPipelineBuilder();

	GraphicsPipelineBuilder& Subpass(int subpass);
	GraphicsPipelineBuilder& Layout(VulkanPipelineLayout *layout);
	GraphicsPipelineBuilder& RenderPass(VulkanRenderPass *renderPass);
	GraphicsPipelineBuilder& Topology(VkPrimitiveTopology topology);
	GraphicsPipelineBuilder& Viewport(float x, float y, float width, float height, float minDepth = 0.0f, float maxDepth = 1.0f);
	GraphicsPipelineBuilder& Scissor(int x, int y, int width, int height);
	GraphicsPipelineBuilder& RasterizationSamples(VkSampleCountFlagBits samples);

	GraphicsPipelineBuilder& Cull(VkCullModeFlags cullMode, VkFrontFace frontFace);
	GraphicsPipelineBuilder& DepthStencilEnable(bool test, bool write, bool stencil);
	GraphicsPipelineBuilder& DepthFunc(VkCompareOp func);
	GraphicsPipelineBuilder& DepthClampEnable(bool value);
	GraphicsPipelineBuilder& DepthBias(bool enable, float biasConstantFactor, float biasClamp, float biasSlopeFactor);
	GraphicsPipelineBuilder& ColorWriteMask(VkColorComponentFlags mask);
	GraphicsPipelineBuilder& Stencil(VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp, uint32_t compareMask, uint32_t writeMask, uint32_t reference);

	GraphicsPipelineBuilder& AdditiveBlendMode();
	GraphicsPipelineBuilder& AlphaBlendMode();
	GraphicsPipelineBuilder& BlendMode(VkBlendOp op, VkBlendFactor src, VkBlendFactor dst);
	GraphicsPipelineBuilder& SubpassColorAttachmentCount(int count);

	GraphicsPipelineBuilder& AddVertexShader(VulkanShader *shader);
	GraphicsPipelineBuilder& AddFragmentShader(VulkanShader *shader);

	GraphicsPipelineBuilder& AddVertexBufferBinding(int index, size_t stride);
	GraphicsPipelineBuilder& AddVertexAttribute(int location, int binding, VkFormat format, size_t offset);

	GraphicsPipelineBuilder& AddDynamicState(VkDynamicState state);

	GraphicsPipelineBuilder& DebugName(const char* name) { debugName = name; return *this; }

	std::unique_ptr<VulkanPipeline> Create(VulkanDevice *device);

private:
	VkGraphicsPipelineCreateInfo pipelineInfo = { };
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = { };
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = { };
	VkViewport viewport = { };
	VkRect2D scissor = { };
	VkPipelineViewportStateCreateInfo viewportState = { };
	VkPipelineRasterizationStateCreateInfo rasterizer = { };
	VkPipelineMultisampleStateCreateInfo multisampling = { };
	VkPipelineColorBlendAttachmentState colorBlendAttachment = { };
	VkPipelineColorBlendStateCreateInfo colorBlending = { };
	VkPipelineDepthStencilStateCreateInfo depthStencil = { };
	VkPipelineDynamicStateCreateInfo dynamicState = {};

	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
	std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments;
	std::vector<VkVertexInputBindingDescription> vertexInputBindings;
	std::vector<VkVertexInputAttributeDescription> vertexInputAttributes;
	std::vector<VkDynamicState> dynamicStates;

	const char* debugName = nullptr;
};

class PipelineLayoutBuilder
{
public:
	PipelineLayoutBuilder();

	PipelineLayoutBuilder& AddSetLayout(VulkanDescriptorSetLayout *setLayout);
	PipelineLayoutBuilder& AddPushConstantRange(VkShaderStageFlags stageFlags, size_t offset, size_t size);

	PipelineLayoutBuilder& DebugName(const char* name) { debugName = name; return *this; }

	std::unique_ptr<VulkanPipelineLayout> Create(VulkanDevice *device);

private:
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	std::vector<VkDescriptorSetLayout> setLayouts;
	std::vector<VkPushConstantRange> pushConstantRanges;
	const char* debugName = nullptr;
};

class RenderPassBuilder
{
public:
	RenderPassBuilder();

	RenderPassBuilder& AddAttachment(VkFormat format, VkSampleCountFlagBits samples, VkAttachmentLoadOp load, VkAttachmentStoreOp store, VkImageLayout initialLayout, VkImageLayout finalLayout);
	RenderPassBuilder& AddDepthStencilAttachment(VkFormat format, VkSampleCountFlagBits samples, VkAttachmentLoadOp load, VkAttachmentStoreOp store, VkAttachmentLoadOp stencilLoad, VkAttachmentStoreOp stencilStore, VkImageLayout initialLayout, VkImageLayout finalLayout);

	RenderPassBuilder& AddExternalSubpassDependency(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask);

	RenderPassBuilder& AddSubpass();
	RenderPassBuilder& AddSubpassColorAttachmentRef(uint32_t index, VkImageLayout layout);
	RenderPassBuilder& AddSubpassDepthStencilAttachmentRef(uint32_t index, VkImageLayout layout);

	RenderPassBuilder& DebugName(const char* name) { debugName = name; return *this; }

	std::unique_ptr<VulkanRenderPass> Create(VulkanDevice *device);

private:
	VkRenderPassCreateInfo renderPassInfo = { };

	std::vector<VkAttachmentDescription> attachments;
	std::vector<VkSubpassDependency> dependencies;
	std::vector<VkSubpassDescription> subpasses;

	struct SubpassData
	{
		std::vector<VkAttachmentReference> colorRefs;
		VkAttachmentReference depthRef = { };
	};

	std::vector<std::unique_ptr<SubpassData>> subpassData;

	const char* debugName = nullptr;
};

class PipelineBarrier
{
public:
	PipelineBarrier& AddMemory(VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask);
	PipelineBarrier& AddBuffer(VulkanBuffer *buffer, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask);
	PipelineBarrier& AddBuffer(VulkanBuffer *buffer, VkDeviceSize offset, VkDeviceSize size, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask);
	PipelineBarrier& AddImage(VulkanImage *image, VkImageLayout oldLayout, VkImageLayout newLayout, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, int baseMipLevel = 0, int levelCount = 1);
	PipelineBarrier& AddImage(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, int baseMipLevel = 0, int levelCount = 1);
	PipelineBarrier& AddQueueTransfer(int srcFamily, int dstFamily, VulkanBuffer *buffer, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask);
	PipelineBarrier& AddQueueTransfer(int srcFamily, int dstFamily, VulkanImage *image, VkImageLayout layout, VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, int baseMipLevel = 0, int levelCount = 1);

	void Execute(VulkanCommandBuffer *commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags = 0);

private:
	std::vector<VkMemoryBarrier> memoryBarriers;
	std::vector<VkBufferMemoryBarrier> bufferMemoryBarriers;
	std::vector<VkImageMemoryBarrier> imageMemoryBarriers;
};

class QueueSubmit
{
public:
	QueueSubmit();

	QueueSubmit& AddCommandBuffer(VulkanCommandBuffer *buffer);
	QueueSubmit& AddWait(VkPipelineStageFlags waitStageMask, VulkanSemaphore *semaphore);
	QueueSubmit& AddSignal(VulkanSemaphore *semaphore);
	void Execute(VulkanDevice *device, VkQueue queue, VulkanFence *fence = nullptr);

private:
	VkSubmitInfo submitInfo = {};
	std::vector<VkSemaphore> waitSemaphores;
	std::vector<VkPipelineStageFlags> waitStages;
	std::vector<VkSemaphore> signalSemaphores;
	std::vector<VkCommandBuffer> commandBuffers;
};

class WriteDescriptors
{
public:
	WriteDescriptors& AddBuffer(VulkanDescriptorSet *descriptorSet, int binding, VkDescriptorType type, VulkanBuffer *buffer);
	WriteDescriptors& AddBuffer(VulkanDescriptorSet *descriptorSet, int binding, VkDescriptorType type, VulkanBuffer *buffer, size_t offset, size_t range);
	WriteDescriptors& AddStorageImage(VulkanDescriptorSet *descriptorSet, int binding, VulkanImageView *view, VkImageLayout imageLayout);
	WriteDescriptors& AddCombinedImageSampler(VulkanDescriptorSet *descriptorSet, int binding, VulkanImageView *view, VulkanSampler *sampler, VkImageLayout imageLayout);
	WriteDescriptors& AddAccelerationStructure(VulkanDescriptorSet* descriptorSet, int binding, VulkanAccelerationStructure* accelStruct);
	void Execute(VulkanDevice *device);

private:
	struct WriteExtra
	{
		VkDescriptorImageInfo imageInfo;
		VkDescriptorBufferInfo bufferInfo;
		VkBufferView bufferView;
		VkWriteDescriptorSetAccelerationStructureKHR accelStruct;
	};

	std::vector<VkWriteDescriptorSet> writes;
	std::vector<std::unique_ptr<WriteExtra>> writeExtras;
};
