//
// Created by mcumbrella on 23-10-18.
//

#include "RenderEntity.h"
#include "RenderManager.h"
#include "../exception/IllegalArgumentException.h"

RenderEntity::RenderEntity(const int64_t& id) : id(id)
{
    sdlTexture = nullptr;
}

RenderEntity::RenderEntity(const int64_t& id, const string& texturePath) : id(id)
{
    sdlTexture = RenderManager::getTexture(texturePath);
    SDL_QueryTexture(sdlTexture, nullptr, nullptr, &textureWidth, &textureHeight);
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

void RenderEntity::setTextureOffset(const int& x_, const int& y_)
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

void RenderEntity::setTextureSize(const int& tx, const int& ty)
{
    textureWidth = tx;
    textureHeight = ty;
}

void RenderEntity::resetTextureSize()
{
    SDL_QueryTexture(sdlTexture, nullptr, nullptr, &textureWidth, &textureHeight);
}

void RenderEntity::setTexture(const string& path)
{
    sdlTexture = RenderManager::getTexture(path);
}

int RenderEntity::getFlip()
{
    return flip == SDL_FLIP_VERTICAL ? 2 :
           flip == SDL_FLIP_HORIZONTAL ? 1 :
           0;
}

void RenderEntity::setFlip(int mode)
{
    flip = mode == 2 ? SDL_FLIP_VERTICAL :
           mode == 1 ? SDL_FLIP_HORIZONTAL :
           SDL_FLIP_NONE;
}

void RenderEntity::setCrop(const int& startX, const int& startY, const int& dx, const int& dy)
{
    if(startX < 0 || startY < 0 || dx < 0 || dy < 0)
        throw IllegalArgumentException("Invalid coordinate or width/height in RenderEntity.setCrop()");
    cropRect.x = startX;
    cropRect.y = startY;
    cropRect.w = dx;
    cropRect.h = dy;
}

void RenderEntity::resetCrop()
{
    memset(&cropRect, 0, sizeof(cropRect));
}
