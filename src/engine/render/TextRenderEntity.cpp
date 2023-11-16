//
// Created by mcumbrella on 2023/11/14.
//

#include "TextRenderEntity.h"
#include "../exception/UnsupportedOperationException.h"
#include "RenderManager.h"

TextRenderEntity::TextRenderEntity(const int64_t& id, const string& content) : RenderEntity(id), content(content)
{
    sdlTexture = RenderManager::text2Texture(content, 255, 255, 255, 255, 32);
    SDL_QueryTexture(sdlTexture, nullptr, nullptr, &textureWidth, &textureHeight);
    hitboxWidth = textureWidth;
    hitboxHeight = textureHeight;
}

TextRenderEntity::~TextRenderEntity()
{
    SDL_DestroyTexture(sdlTexture);
}

void TextRenderEntity::setTexture(const string&)
{
    throw UnsupportedOperationException(
        "Explicitly setting texture for TextRenderEntity is not supported. Use setContent() instead"
    );
}

string TextRenderEntity::getContent()
{
    return content;
}

TextRenderEntity* TextRenderEntity::setContent(const string& newContent)
{
    SDL_DestroyTexture(sdlTexture);
    sdlTexture = RenderManager::text2Texture(newContent, 255, 255, 255, 255, 32);
    resetTextureSize();
    resetHitboxSize();
    resetCrop();
    content = newContent;
    return this;
}
