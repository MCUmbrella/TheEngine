//
// Created by mcumbrella on 2023/10/31.
//

#ifndef THEENGINE_RENDERLAYER_H
#define THEENGINE_RENDERLAYER_H


#include <map>
#include <memory>
#include "RenderEntity.h"
#include "TextRenderEntity.h"

class RenderLayer
{
private:
    int order;
    std::map<int64_t, std::shared_ptr<RenderEntity>> renderEntities;
public:
    explicit RenderLayer(const int& order) : order(order)
    {
    }

    int getOrder();

    /**
     * Add a new entity to the layer.
     * @param texturePath The path of the texture that the entity used.
     * @return The pointer to the newly added entity.
     */
    RenderEntity* addEntity(const string& texturePath);

    /**
     * Add a new text entity to the layer.
     * @param content The content of the text entity.
     * @param fontName The name of the font to use.
     * @param pt The size of the text.
     * @return The pointer to the newly added entity.
     */
    TextRenderEntity* addText(const string& content, const string& fontName, const int& pt);

    TextRenderEntity* addText(const string& content, const string& fontName);

    TextRenderEntity* addText(const string& content, const int& pt);

    TextRenderEntity* addText(const string& content);

    /**
     * Remove an entity from the layer.
     * @param id The ID of the entity.
     * @throw EngineException if the entity with this ID is not found.
     */
    void removeEntity(const int64_t& id);

    /**
     * Get render entity by its ID.
     * @param id The ID of the entity.
     * @return The pointer to the entity with the specified ID.
     * @throw EngineException if the entity with this ID is not found.
     */
    RenderEntity* getEntity(const int64_t& id);

    /**
     * Check if an entity with the specified ID exists.
     * @param id The ID of the entity.
     * @return true if the entity exists, false otherwise.
     */
    bool hasEntity(const int64_t& id);

    /**
     * Get the count of entities in this layer.
     */
    size_t size();

    /**
     * Remove all entities in this layer.
     */
    void clear();

    /**
     * Call RenderManager.placeTexture() to render all entities in this layer.
     * Should only be called by RenderManager.render()
     */
    void apply();
};


#endif //THEENGINE_RENDERLAYER_H
