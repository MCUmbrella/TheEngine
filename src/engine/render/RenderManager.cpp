//
// Created by mcumbrella on 23-10-16.
//

#include <map>
#include <unordered_map>
#include "RenderManager.h"
#include "../util/Logger.h"
#include "../exception/EngineException.h"
#include "RenderLayer.h"
#include "../ConfigManager.h"
#include "../exception/IllegalArgumentException.h"

using std::to_string;

static string placeholderTexturePath;
static SDL_Texture* placeholderTexture = nullptr;

static string windowTitle;
static SDL_Window* window = nullptr;
static int windowX = 0;
static int windowY = 0;
static int windowWidth = 640;
static int windowHeight = 480;
static SDL_Renderer* renderer = nullptr;
static unsigned int bgR = 0U;
static unsigned int bgG = 0U;
static unsigned int bgB = 0U;

static string defaultFontPath;
static TTF_Font* font16 = nullptr;
static TTF_Font* font24 = nullptr;
static TTF_Font* font32 = nullptr;
static TTF_Font* font40 = nullptr;
static TTF_Font* font48 = nullptr;

static std::map<int, RenderLayer> layers;
static std::unordered_map<string, SDL_Texture*> loadedTextures;

const RenderManager& RenderManager::getInstance()
{
    const static RenderManager THE_RENDER_MANAGER;
    return THE_RENDER_MANAGER;
}

SDL_Window* RenderManager::getWindow()
{
    return window;
}

SDL_Renderer* RenderManager::getRenderer()
{
    return renderer;
}

void RenderManager::init()
{
    logInfo << "Initializing render manager";

    // initialize sdl
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        throw EngineException(string("SDL initialization failed: ") + SDL_GetError());
    if(TTF_Init() < 0)
        throw EngineException(string("SDL TTF initialization failed: ") + SDL_GetError());

    // create window and sdl renderer
    logInfo << "Creating window";
    window = SDL_CreateWindow(
        windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        windowWidth, windowHeight, 0
    );
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    renderer = SDL_CreateRenderer(window, -1, ConfigManager::getSdlRendererFlags());
    SDL_SetRenderDrawColor(renderer, bgR, bgG, bgB, 255);
    logInfo << "Window created: " << SDL_GetWindowTitle(window);

    // load placeholder texture
    placeholderTexturePath = ConfigManager::getEngineDataPath() + "/assets/textures/misc/placeholder.png";
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    if((placeholderTexture = loadTexture(placeholderTexturePath)) == nullptr)
        throw EngineException("Is the placeholder texture file \"" + string(placeholderTexturePath) + "\" OK?");
    loadedTextures[placeholderTexturePath] = placeholderTexture;

    // load default font
    defaultFontPath = ConfigManager::getEngineDataPath() + "/assets/fonts/default.ttf";
    if((font16 = TTF_OpenFont(defaultFontPath.c_str(), 16)) == nullptr)
        throw EngineException("Is the default font file \"" + defaultFontPath + "\" OK?");
    font24 = TTF_OpenFont(defaultFontPath.c_str(), 24);
    font32 = TTF_OpenFont(defaultFontPath.c_str(), 32);
    font40 = TTF_OpenFont(defaultFontPath.c_str(), 40);
    font48 = TTF_OpenFont(defaultFontPath.c_str(), 48);

    addLayer(0);

    logInfo << "Render manager initialization completed";
}

void RenderManager::shutdown()
{
    logInfo << "Shutting down render manager";
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    if(loadedTextures.size() != 1)
    {
        logWarn << "WHO FORGOT TO UNLOAD TEXTURES?";
        logWarn << "Loaded textures (" << loadedTextures.size() << "):";
        for(auto& tp : loadedTextures)
            logWarn << "-- " << tp.first;
    }
    logInfo << "Render manager shutted down";
}

void RenderManager::render()
{
    SDL_RenderClear(renderer);
    for(auto& lp : layers)
    {
        RenderLayer& l = lp.second;
        l.apply();
    }
    SDL_RenderPresent(renderer);
}

int RenderManager::getWindowX()
{
    SDL_GetWindowPosition(window, &windowX, &windowY);
    return windowX;
}

int RenderManager::getWindowY()
{
    SDL_GetWindowPosition(window, &windowX, &windowY);
    return windowY;
}

void RenderManager::setWindowLocation(const int& x, const int& y)
{
    windowX = x;
    windowY = y;
    SDL_SetWindowPosition(window, x, y);
}

int RenderManager::getWindowWidth()
{
    return windowWidth;
}

int RenderManager::getWindowHeight()
{
    return windowHeight;
}

void RenderManager::setWindowSize(const int& w, const int& h)
{
    windowWidth = w;
    windowHeight = h;
    //logInfo << "Setting window size to " << w << " * " << h;
    SDL_SetWindowSize(window, w, h);
}

void RenderManager::setBackgroundColor(unsigned int r, unsigned int g, unsigned int b)
{
    bgR = r;
    bgG = g;
    bgB = b;
    SDL_SetRenderDrawColor(renderer, bgR, bgG, bgB, 255);
}

string RenderManager::getWindowTitle()
{
    return windowTitle;
}

void RenderManager::setWindowTitle(const string& title)
{
    SDL_SetWindowTitle(window, title.c_str());
    windowTitle = title;
}

int RenderManager::getTextureWidth(SDL_Texture* texture)
{
    if(texture == nullptr) throw EngineException("Null pointer passed to getTextureWidth()");
    int w;
    SDL_QueryTexture(texture, nullptr, nullptr, &w, nullptr);
    return w;
}

int RenderManager::getTextureHeight(SDL_Texture* texture)
{
    if(texture == nullptr) throw EngineException("Null pointer passed to getTextureHeight()");
    int h;
    SDL_QueryTexture(texture, nullptr, nullptr, nullptr, &h);
    return h;
}

SDL_Texture* RenderManager::loadTexture(const string& path)
{
    logInfo << "Loading texture: " << path;
    if(loadedTextures.contains(path))
    {
        logWarn << "Texture already loaded: " << path;
        return loadedTextures[path];
    }
    SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
    if(texture == nullptr) // failed: give the caller placeholder texture
    {
        logError << "Failed to load texture \"" << path << "\": " << SDL_GetError();
        loadedTextures[path] = placeholderTexture;
        texture = placeholderTexture;
    }
    else // success: add the pointer to the map
    {
        loadedTextures[path] = texture;
    }
    return texture;
}

bool RenderManager::unloadTexture(const string& path)
{
    bool unloaded = false;
    if(loadedTextures.contains(path))
    {
        if(loadedTextures[path] != placeholderTexture)
        {
            logInfo << "Unloading texture: " << path;
            SDL_DestroyTexture(loadedTextures.at(path));
            loadedTextures.erase(path);
            unloaded = true;
        }
        else // who's trying to unload placeholder texture?
        {
            logWarn << "Placeholder texture will not be unloaded";
            if(path != placeholderTexturePath)
                loadedTextures.erase(path);
        }
    }
    else // texture not found
        logWarn << "Trying to unload a texture that is not loaded: " << path;
    return unloaded;
}

SDL_Texture* RenderManager::getTexture(const string& path)
{
    if(loadedTextures.contains(path))
        return loadedTextures.at(path);
    else
    {
        logWarn << "Trying to get texture \"" << path << "\" which is not loaded. Calling loadTexture()";
        return loadTexture(path);
    }
}

void RenderManager::placeTexture(RenderEntity& re)
{
    SDL_FRect destRect;
    destRect.x = (re.x - re.textureWidth / 2.0) + re.textureOffsetX;
    destRect.y = (re.y - re.textureHeight / 2.0) + re.textureOffsetY;
    destRect.w = re.textureWidth;
    destRect.h = re.textureHeight;
    SDL_SetTextureColorMod(re.sdlTexture, re.textureRed, re.textureGreen, re.textureBlue);
    SDL_SetTextureAlphaMod(re.sdlTexture, re.textureAlpha);
    SDL_RenderCopyExF(
        renderer,
        re.sdlTexture,
        (re.cropRect.x | re.cropRect.y | re.cropRect.w | re.cropRect.h) ? &re.cropRect : nullptr,
        &destRect,
        re.textureDegree,
        nullptr,
        re.flip
    );
}

SDL_Texture* RenderManager::text2Texture(
    const string& text,
    const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a,
    const FontSize& size
)
{
    SDL_Color color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
    SDL_Surface* surface;
    surface = TTF_RenderUTF8_Blended(
        size == FontSize::XL ? font48 :
        size == FontSize::L ? font40 :
        size == FontSize::M ? font32 :
        size == FontSize::S ? font24 :
        font16,
        text.c_str(), color
    );
    return surfaceToTexture(surface, true);
}

SDL_Texture* RenderManager::surfaceToTexture(SDL_Surface* surface, const bool& destroySurface)
{
    SDL_Texture* texture;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(destroySurface)
        SDL_FreeSurface(surface);
    return texture;
}

RenderLayer* RenderManager::addLayer(const int& order)
{
    if(hasLayer(order))
        throw EngineException("Layer exists: " + to_string(order));
    auto p = layers.emplace(order, RenderLayer(order));
    const auto& it = p.first;
    RenderLayer& l = it->second;
    logInfo << "Layer added: " << order;
    return &l;
    // can't return & as kaguya only traits * as the same object in Lua
}

void RenderManager::removeLayer(const int& order)
{
    if(order == 0)
        throw IllegalArgumentException("Layer 0 cannot be removed");
    if(!hasLayer(order))
        throw EngineException("Layer not found: " + to_string(order));
    layers.erase(order);
    logInfo << "Layer removed: " << order;
}

RenderLayer* RenderManager::getLayer(const int& order)
{
    if(!hasLayer(order))
        throw EngineException("Layer not found: " + to_string(order));
    return &(layers.at(0));
}

bool RenderManager::hasLayer(const int& order)
{
    return layers.find(order) != layers.end();
}

RenderLayer* RenderManager::reorderLayer(const int& src, const int& target)
{
    if(src == 0 || target == 0)
        throw IllegalArgumentException("Layer 0 cannot be reordered");
    if(src == target)
        throw IllegalArgumentException("src and target in reorderLayer() cannot be the same: " + to_string(src));
    return &(layers.at(src));
}
