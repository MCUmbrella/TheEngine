//
// Created by mcumbrella on 23-10-16.
//

#ifndef THEENGINE_RENDERMANAGER_H
#define THEENGINE_RENDERMANAGER_H

#include <string>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "RenderEntity.h"
#include "RenderLayer.h"
#include "Window.h"
#include "Font.h"

using std::string;

class RenderManager
{
private:
    RenderManager() = default;

    ~RenderManager() = default;

public:
    RenderManager(const RenderManager&) = delete;

    RenderManager& operator=(const RenderManager&) = delete;

    static Window* getWindow();

    static SDL_Renderer* getRenderer();

    static void init();

    static void shutdown();

    static void render();

    static void setBackgroundColor(unsigned int r, unsigned int g, unsigned int b);

    /**
     * Add a render layer.
     * @param order The order of the layer, from [INT_MIN, INT_MAX].
     *              Layer 0 is automatically created.
     * @return The pointer to the newly added layer.
     * @throw EngineException if the layer with the specified order already exists.
     */
    static RenderLayer* addLayer(const int& order);

    /**
     * Remove a render layer.
     * @param order The order of the layer.
     * @throw EngineException if no layer with this order was found or the order is 0.
     */
    static void removeLayer(const int& order);

    /**
     * Get a layer by its order.
     * @param order The order of the layer.
     * @return The pointer to the layer with the specified order.
     * @throw EngineException if no layer with this order was found.
     */
    static RenderLayer* getLayer(const int& order);

    /**
     * Check if a layer with the specified order exists.
     */
    static bool hasLayer(const int& order);

    static RenderLayer* reorderLayer(const int& src, const int& target); //TODO

    /**
     * Load a texture from file.
     * @param path The path of the texture file.
     * @return The pointer of the texture's SDL_Texture object,
     *         or the pointer of the placeholder texture's if
     *         the texture load fails.
     */
    static SDL_Texture* loadTexture(const string& path);

    /**
     * Unload a texture from the texture list.
     * @param path The path of the texture file.
     * @return true if the texture has been unloaded successfully.
     */
    static bool unloadTexture(const string& path);

    /**
     * Get the loaded texture by its path.
     * @param path The path of the texture file.
     * @return The pointer of the texture, or the pointer of the
     *         placeholder texture if the texture isn't loaded.
     */
    static SDL_Texture* getTexture(const string& path);

    /**
     * Place a RenderEntity object on the screen.
     * @param re The reference to the RenderEntity object.
     */
    static void placeTexture(RenderEntity& re);

    /**
     * Load a font file as Font object.
     * @param name The name of the Font object.
     * @param path The path to the font file that the object uses.
     * @return The reference to the Font object.
     * @throw IllegalArgumentException if 'name' is empty.
     * @throw EngineException if the file has failed to
     * load, or the font with this name already exists.
     */
    static Font& loadFont(const string& name, const string& path);

    /**
     * Unload a font.
     * @param name The name of the Font object.
     * @throw IllegalArgumentException if 'name' is empty.
     * @throw EngineException if there's no Font object with this name.
     */
    static void unloadFont(const string& name);

    /**
     * Get a font.
     * @param name The name of the Font object.
     * @return The loaded font with that name. If
     * the name is empty, return the default font.
     * @throw EngineException if there's no Font object with this name.
     */
    static Font& getFont(const string& name);

    /**
     * Check if a Font object with the specified name exists.
     */
    static bool hasFont(const string& name);

    /**
     * Convert a text to the texture object.
     * @param text The text that needs to be converted.
     * @param r RGB value (red amount).
     * @param g RGB value (green amount).
     * @param b RGB value (blue amount).
     * @param a Alpha value (visibility).
     * @param fontName The name of the Font object to use.
     * @param pt The size of the font.
     * NOTICE: The RGB and Alpha amount is valid in 0~255.
     */
    static SDL_Texture* text2Texture(
        const string& text,
        const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a,
        const string& fontName,
        const int& pt
    );
};

#endif //THEENGINE_RENDERMANAGER_H
