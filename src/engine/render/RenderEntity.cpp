//
// Created by mcumbrella on 23-10-18.
//

#include "RenderEntity.h"
#include "../RenderManager.h"

RenderEntity::RenderEntity(const int64_t& id, const string& texturePath) : id(id)
{
    sdlTexture = RenderManager::getTexture(texturePath);
    SDL_QueryTexture(sdlTexture, nullptr, nullptr, &textureWidth, &textureHeight);
    hitboxWidth = textureWidth;
    hitboxHeight = textureHeight;
}

int64_t RenderEntity::getId()
{
    return id;
}

void RenderEntity::setLocation(const double& x_, const double& y_)
{
    x = x_;
    y = y_;
}

void RenderEntity::move(const double& dx, const double& dy)
{
    x += dx;
    y += dy;
}

void RenderEntity::setTextureLocation(const int& x_, const int& y_)
{
    textureOffsetX = x_;
    textureOffsetY = y_;
}

void RenderEntity::moveTexture(const int& dx, const int& dy)
{
    textureOffsetX += dx;
    textureOffsetY += dy;
}

void RenderEntity::rotate(const double& degree)
{
    textureDegree += degree;
}

void RenderEntity::resizeHitbox(const int& hx, const int& hy)
{
    hitboxWidth = hx;
    hitboxHeight = hy;
}

void RenderEntity::resizeTexture(const int& tx, const int& ty)
{
    textureWidth = tx;
    textureHeight = ty;
}
