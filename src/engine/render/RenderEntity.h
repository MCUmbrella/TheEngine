//
// Created by mcumbrella on 23-10-18.
//

#ifndef THEENGINE_RENDERENTITY_H
#define THEENGINE_RENDERENTITY_H

#include <SDL2/SDL_render.h>
#include <string>

using std::string;

class RenderEntity
{
public:
    // The ID of the entity,
    // assigned by RenderLayer.addEntity(String)
    const int64_t id;

    // The pointer to the entity's SDL texture,
    // assigned by RenderEntity.new(String)
    SDL_Texture* sdlTexture;

    // The location of the entity.
    // This location is the center of the texture.
    double x = 0, y = 0;

    // The 'actual' size of the entity.
    // It's set to the same size as the initial
    // texture and doesn't affect texture rendering.
    double hitboxWidth = 0, hitboxHeight = 0;

    // The size of the texture. changing the value
    // will cause the texture to be stretched.
    int textureWidth = 0, textureHeight = 0;

    // The position offset of the texture.
    int textureOffsetX = 0, textureOffsetY = 0;

    // The angle of the texture rotation.
    double textureDegree = 0;

    SDL_RendererFlip flip = SDL_FLIP_NONE;

    RenderEntity(const int64_t& id, const string& texturePath);

    int64_t getId();

    void setLocation(const double& x_, const double& y_);

    void move(const double& dx, const double& dy);

    void setTextureOffset(const int& x_, const int& y_);

    void moveTexture(const int& dx, const int& dy);

    void rotate(const double& d);

    void resizeHitbox(const int& hx, const int& hy);

    void resizeTexture(const int& tx, const int& ty);

    /**
     * Set the size of the texture to its actual
     * size stored in the file.
     */
    void resetTextureSize();

    /**
     * Set the size of the entity's hitbox to
     * the original size of entity's texture.
     */
    void resetHitboxSize();

    /**
     * Change the entity's texture.
     * The size of the texture and hitbox will
     * retain, if you want to update them, call
     * resetTextureSize() and resetHitboxSize()
     * @param path The path of the texture.
     */
    void changeTexture(const string& path);

    // this function is used in Lua scripts
    void changeTexture_l(const string& path);

    /**
     * Get the mode of the texture's flip.
     * @return 0: none, 1: horizontal, 2: vertical.
     */
    int getFlip();

    /**
     * Get the mode of the texture's flip.
     * @param mode 0: none, 1: horizontal, 2: vertical.
     */
    void setFlip(int mode);
};

#endif //THEENGINE_RENDERENTITY_H
