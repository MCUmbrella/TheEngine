//
// Created by mcumbrella on 2023/10/31.
//

#include "RenderLayer.h"
#include "../util/CommonUtil.h"
#include "../exception/EngineException.h"
#include "../RenderManager.h"
#include "../ConfigManager.h"

int RenderLayer::getOrder()
{
    return order;
}

RenderEntity* RenderLayer::addEntity(const string& texturePath)
{
    // can't return & as kaguya only traits * as the same object in Lua
    int64_t id = CommonUtil::snowflakeId();
    auto p = renderEntities.emplace(id, RenderEntity(id, texturePath));
    const auto& it = p.first;
    RenderEntity& e = it->second;
    return &e;
}

RenderEntity* RenderLayer::addEntity_l(const string& texturePath)
{
    return addEntity(ConfigManager::getUserDataPath() + "/assets/textures/" + texturePath);
}

RenderEntity RenderLayer::removeEntity(const int64_t& id)
{
    auto it = renderEntities.find(id);
    if(it == renderEntities.end())
        throw EngineException("RenderEntity with ID " + std::to_string(id) + " not found");
    else
    {
        RenderEntity removed = it->second;
        renderEntities.erase(it);
        return removed;
    }
}

RenderEntity* RenderLayer::getEntity(const int64_t& id)
{
    auto it = renderEntities.find(id);
    if(it == renderEntities.end())
        throw EngineException("RenderEntity with ID " + std::to_string(id) + " not found");
    else
        return &(it->second);
}

bool RenderLayer::hasEntity(const int64_t& id)
{
    return renderEntities.find(id) != renderEntities.end();
}

void RenderLayer::apply()
{
    for(auto& rep : renderEntities)
    {
        RenderEntity& re = rep.second;
        RenderManager::placeTexture(
            re.sdlTexture,
            (re.x - re.textureWidth / 2.0) + re.textureOffsetX,
            (re.y - re.textureHeight / 2.0) + re.textureOffsetY,
            re.textureWidth,
            re.textureHeight
        );
    }
}

size_t RenderLayer::size()
{
    return renderEntities.size();
}

void RenderLayer::clear()
{
    renderEntities.clear();
}
