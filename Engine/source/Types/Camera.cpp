#include "Types/Camera.h"


Engine::Camera::Camera() : Named(Named::Type::Camera) {}
Engine::Camera::Camera(const String& name) : Named(Named::Type::Camera, name) {}
