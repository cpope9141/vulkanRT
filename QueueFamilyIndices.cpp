#include "QueueFamilyIndices.h"

QueueFamilyIndices::QueueFamilyIndices() {}
QueueFamilyIndices::~QueueFamilyIndices() {}

std::optional<uint32_t> QueueFamilyIndices::getGraphicsFamily() { return graphicsFamily; }
std::optional<uint32_t> QueueFamilyIndices::getPresentFamily() { return presentFamily; }
bool QueueFamilyIndices::requiredFamiliesFound() { return graphicsFamily.has_value() && presentFamily.has_value(); }
void QueueFamilyIndices::setGraphicsFamily(int graphicsFamily) { this->graphicsFamily = graphicsFamily; }
void QueueFamilyIndices::setPresentFamily(int presentFamily) { this->presentFamily = presentFamily; }
