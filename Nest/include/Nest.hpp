#pragma once

// Base
#include "Nest/Application/Application.hpp"
#include "Nest/Application/ApplicationStartupSettings.hpp"
#include "Nest/Application/Layer.hpp"
#include "Nest/Platform/PlatformDetection.hpp"
#include "Nest/Renderer/Model3D.hpp"
#include "Nest/Renderer/Texture.hpp"
#include "Nest/GameLogic/Input.hpp"

// Audio
#include "Nest/Audio/AudioBase.hpp"

// SystemTools
#include "Nest/SystemTools/SystemTools.hpp"

// Assets
#include "Nest/Assets/AssetLoader.hpp"

// Game logic
#include "Nest/GameLogic/Components/DynamicMesh.hpp"
#include "Nest/GameLogic/Components/SkyComponent.hpp"
#include "Nest/GameLogic/Components/StaticMesh.hpp"
#include "Nest/GameLogic/Components/TransformComponent.hpp"
#include "Nest/GameLogic/Entity.hpp"

// Third party
#include <glm/ext.hpp>

// Foundation
#include <Foundation/Allocator.hpp>
#include <Foundation/Foundation.hpp>
#include <Foundation/Logger.hpp>
#include <Foundation/Vector.hpp>

// Renderer
#include "Nest/Renderer/Viewport.hpp"
