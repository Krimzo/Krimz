#include "Types/Camera.h"


Engine::Camera::Camera() : Named(Named::Type::Camera) {}
Engine::Camera::Camera(const std::string& name) : Named(Named::Type::Camera, name) {}
