#pragma once

// Base
#include "Nest/Application/Application.hpp"
#include "Nest/Application/Layer.hpp"
#include "Nest/Platform/PlatformDetection.hpp"
#include "Nest/Application/ApplicationStartupSettings.hpp"
#include "Nest/Renderer/Model3D.hpp"
#include "Nest/Renderer/Texture.hpp"
#include "WindowImpl/GlfwWindow/GlfwWindow.hpp"
#include "EventsImpl/GlfwEvents/GlfwEvents.hpp"

// SystemTools
#include "Nest/SystemTools/SystemTools.hpp"

// Assets
#include "Nest/Assets/AssetLoader.hpp"

// Game logic
#include "Nest/GameLogic/Components/StaticMesh.hpp"
#include "Nest/GameLogic/Components/DynamicMesh.hpp"
#include "Nest/GameLogic/Components/TransformComponent.hpp"
#include "Nest/GameLogic/Components/SkyComponent.hpp"
#include "Nest/GameLogic/Entity.hpp"

// Third party
#include <glm/ext.hpp>

// Foundation
#include <Foundation/Foundation.hpp>
#include <Foundation/Logger.hpp>
#include <Foundation/Allocator.hpp>
#include <Foundation/Vector.hpp>

// Renderer
#include "Nest/Renderer/Viewport.hpp"
