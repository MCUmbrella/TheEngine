//
// Created by mcumbrella on 2023/10/31.
//

#include "RenderLayer.h"
#include "../util/CommonUtil.h"
#include "../exception/EngineException.h"
#include "RenderManager.h"
#include "../ConfigManager.h"

int RenderLayer::getOrder()
{
    return order;
}

RenderEntity* RenderLayer::addEntity(const string& texturePath)
{
    // can't return & as kaguya only traits * as the same object in Lua
    int64_t id = CommonUtil::snowflakeId();
    auto p = renderEntities.emplace(id, std::make_shared<RenderEntity>(id, texturePath));
    const auto& it = p.first;
    return it->second.get();
}

RenderEntity* RenderLayer::addEntity_l(const string& texturePath)
{
    return addEntity(ConfigManager::getUserDataPath() + "/assets/textures/" + texturePath);
}

TextRenderEntity* RenderLayer::addText(const string& content)
{
    int64_t id = CommonUtil::snowflakeId();
    auto p = renderEntities.emplace(id, std::make_shared<TextRenderEntity>(id, content));
    const auto& it = p.first;
    return dynamic_cast<TextRenderEntity*>(it->second.get());
}

void RenderLayer::removeEntity(const int64_t& id)
{
    auto it = renderEntities.find(id);
    if(it == renderEntities.end())
        throw EngineException("RenderEntity with ID " + std::to_string(id) + " not found");
    else
        renderEntities.erase(it);
}

RenderEntity* RenderLayer::getEntity(const int64_t& id)
{
    auto it = renderEntities.find(id);
    if(it == renderEntities.end())
        throw EngineException("RenderEntity with ID " + std::to_string(id) + " not found");
    else
        return it->second.get();
}

bool RenderLayer::hasEntity(const int64_t& id)
{
    return renderEntities.find(id) != renderEntities.end();
}

void RenderLayer::apply()
{
    for(auto& rep : renderEntities)
        RenderManager::placeTexture(*(rep.second));
}

size_t RenderLayer::size()
{
    return renderEntities.size();
}

void RenderLayer::clear()
{
    renderEntities.clear();
}
