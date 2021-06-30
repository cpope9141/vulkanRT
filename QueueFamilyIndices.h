#pragma once
#include <optional>

class QueueFamilyIndices
{
public:
	QueueFamilyIndices();
	~QueueFamilyIndices();

	std::optional<uint32_t> getGraphicsFamily();
	std::optional<uint32_t> getPresentFamily();
	bool requiredFamiliesFound();
	void setGraphicsFamily(int graphicsFamily);
	void setPresentFamily(int presentFamily);

private:
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
};