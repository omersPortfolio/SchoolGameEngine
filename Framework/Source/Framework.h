#pragma once

// This header includes all files inside the framework project.
// It is meant for use by game projects ONLY.
// As new files are created, include them here so the game has easy access to them.
// It should not be included by any files inside the framework project.

#include "FrameworkPCH.h"

#include "Components/Component.h"
#include "Components/ComponentManager.h"
#include "Components/MeshComponent.h"
#include "Components/AABBComponent.h"
#include "Core/GameCore.h"
#include "Core/FWCore.h"
#include "EventSystem/Event.h"
#include "EventSystem/EventManager.h"
#include "Math/Vector.h"
#include "Objects/Camera.h"
#include "Objects/GameObject.h"
#include "Objects/Material.h"
#include "Objects/Mesh.h"
#include "Objects/ResourceManager.h"
#include "Objects/Texture.h"
#include "Particles/ParticleEmitter.h"
#include "Physics/PhysicsBody.h"
#include "Physics/PhysicsBody2D.h"
#include "Physics/PhysicsWorld.h"
#include "Physics/PhysicsWorld2D.h"
#include "Objects/AABB.h"
#include "Scene/Scene.h"
#include "UI/ImGuiManager.h"
#include "Utility/Color.h"
#include "Utility/FrameBufferObject.h"
#include "Utility/Helpers.h"
#include "Utility/JSONHelpers.h"
#include "Utility/ShaderProgram.h"
#include "TextureManager.h"
