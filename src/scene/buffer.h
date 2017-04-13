#ifndef BUFFER_H
#define BUFFER_H

#include "../vulkan.h"

#include <cstring>

class StagingBuffer;

class Buffer {
public:
	Buffer(VkDeviceSize size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags);
	~Buffer();

	void *map(VkDeviceSize offset, VkDeviceSize size)
	{
		void *ret;
		VkResult err = vkMapMemory(vulkan::device, deviceMemory, 0, size, 0, &ret);
		assert(err == VK_SUCCESS);
		return ret;
	}

	void unmap()
	{
		vkUnmapMemory(vulkan::device, deviceMemory);
	}

	void uploadMemory(VkDeviceSize offset, void *data, VkDeviceSize size)
	{
		auto mappedUniformMemory = map(offset, size);
		memcpy(mappedUniformMemory, data, (size_t)size);
		unmap();
	}

	VkBuffer getBuffer() const
	{
		return buffer;
	}

	VkDeviceMemory getDeviceMemory() const
	{
		return deviceMemory;
	}

	void uploadFromStagingBuffer(StagingBuffer *stagingBuffer, VkDeviceSize srcOffset, VkDeviceSize dstOffset, VkDeviceSize size);

private:
	VkBuffer buffer;
	VkDeviceMemory deviceMemory;
};

class StagingBuffer : public Buffer {
public:
	StagingBuffer(VkDeviceSize size) : Buffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
	{
	}
};

#endif // 1BUFFER_H
