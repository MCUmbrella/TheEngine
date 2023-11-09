//
// Created by mcumbrella on 23-10-16.
//

#ifndef THEENGINE_RENDERMANAGER_H
#define THEENGINE_RENDERMANAGER_H

#include <string>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "FontSize.h"
#include "RenderEntity.h"
#include "RenderLayer.h"

using std::string;

class RenderManager
{
private:
    RenderManager() = default;

    ~RenderManager() = default;

public:
    RenderManager(const RenderManager&) = delete;

    RenderManager& operator=(const RenderManager&) = delete;

    const static RenderManager& getInstance();

    static SDL_Window* getWindow();

    static SDL_Renderer* getRenderer();

    static void init();

    static void shutdown();

    static void render();

    static int getWindowX();

    static int getWindowY();

    static void setWindowLocation(const int& x, const int& y);

    static int getWindowWidth();

    static int getWindowHeight();

    static void setWindowSize(const int& w, const int& h);

    static void setBackgroundColor(unsigned int r, unsigned int g, unsigned int b);

    static string getWindowTitle();

    static void setWindowTitle(const string& title);

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
     * Get the width of a texture.
     */
    static int getTextureWidth(SDL_Texture* texture);

    /**
     * Get the height of a texture.
     */
    static int getTextureHeight(SDL_Texture* texture);

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
     * Convert a text to the texture object.
     * @param text The text that needs to be converted.
     * @param r RGB value (red amount).
     * @param g RGB value (green amount).
     * @param b RGB value (blue amount).
     * @param a Alpha value (transparency amount).
     * @param size FontSize: XS (16pt), S (24pt),
     *             M (32pt), L (40pt), XL (48pt).
     * NOTICE: The RGB and Alpha amount is valid in 0~255.
     */
    static SDL_Texture* text2Texture(
        const string& text,
        const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a,
        const FontSize& size
    );

    static SDL_Texture* surfaceToTexture(SDL_Surface* surface, const bool& destroySurface);
};

#endif //THEENGINE_RENDERMANAGER_H
