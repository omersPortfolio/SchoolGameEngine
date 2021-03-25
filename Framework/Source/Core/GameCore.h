#pragma once

#include "EventSystem/EventListener.h"
#include "Math/Vector.h"

namespace fw {

class ComponentRegistry;
class Event;
class EventManager;
class ResourceManager;
class FWCore;

class GameCore : public EventListener
{
public:
    GameCore(FWCore* pFramework);
    virtual ~GameCore();

    virtual void StartFrame(float deltaTime) = 0;
    virtual void OnEvent(Event* pEvent) = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Draw() = 0;
    
    virtual void GetGameRenderWindowInfo(vec2* windowPos, vec2* windowSize) = 0;

    FWCore* GetFramework() { return m_pFramework; }
    EventManager* GetEventManager() { return m_pEventManager; }
    ResourceManager* GetResourceManager() { return m_pResourceManager; }
    ComponentRegistry* GetComponentRegistry() { return m_pComponentRegistry; }

protected:
    FWCore* m_pFramework = nullptr;

    EventManager* m_pEventManager = nullptr;
    ResourceManager* m_pResourceManager = nullptr;
    ComponentRegistry* m_pComponentRegistry = nullptr;
};

} // namespace fw
