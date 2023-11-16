//
// Created by mcumbrella on 2023/11/14.
//

#ifndef THEENGINE_TEXTRENDERENTITY_H
#define THEENGINE_TEXTRENDERENTITY_H

#include "RenderEntity.h"

/**
 * The render entity used to render texts.
 * This is a subclass of RenderEntity, means that
 * the functions in RenderEntity can be used in
 * TextRenderEntity instances.
 * <br>
 * CAUTION: calling setTexture() is unavailable due to the
 *  properties of this class. Use setContent() to change
 *  the content and corresponding texture. The texture will
 *  be unloaded automatically as the entity gets removed.
 */
class TextRenderEntity : public RenderEntity
{
private:
    string content;
    string fontName;
    int pt;
public:

    TextRenderEntity(const int64_t& id, const string& content, const string& fontName, const int& pt);

    ~TextRenderEntity();

    /**
     * Don't do this. You will regret that.
     */
    void setTexture(const string&) override;

    /**
     * Get the content of the text entity.
     */
    string getContent();

    /**
     * Set the content of the text entity.
     * The texture size and hitbox size will be updated automatically.
     */
    TextRenderEntity* setContent(const string& newContent);
};

#endif //THEENGINE_TEXTRENDERENTITY_H
