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
    // the unique ID of the entity.
    const int64_t id;

    // the pointer to the SDL texture used by the entity,
    // assigned by RenderEntity.new(String)
    SDL_Texture* sdlTexture;

    // the location on the screen
    // (the center of the entity's hitbox)
    double x = 0, y = 0;

    // the actual width & height of the entity
    int hitboxWidth = 0, hitboxHeight = 0;

    // the texture size of the entity. can be stretched
    int textureWidth = 0, textureHeight = 0;

    // the texture location offset of the entity
    // (calculated at the hitbox center)
    int textureOffsetX = 0, textureOffsetY = 0;

    // the texture's rotation
    // (calculated at the hitbox center)
    double textureDegree = 0;

    RenderEntity(const int64_t& id, const string& texturePath);

    int64_t getId();

    void setLocation(const double& x_, const double& y_);

    void move(const double& dx, const double& dy);

    void setTextureLocation(const int& x_, const int& y_);

    void moveTexture(const int& dx, const int& dy);

    void rotate(const double& d);

    void resizeHitbox(const int& hx, const int& hy);

    void resizeTexture(const int& tx, const int& ty);
};

#endif //THEENGINE_RENDERENTITY_H
