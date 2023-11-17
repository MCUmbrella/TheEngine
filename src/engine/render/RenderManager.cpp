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

// basics
static ConfigManager::EngineConfig& cfgE = ConfigManager::getEngineConfig();
static ConfigManager::RenderManagerConfig& cfgM = ConfigManager::getRenderManagerConfig();
static Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static unsigned int bgR = 0U;
static unsigned int bgG = 31U;
static unsigned int bgB = 63U;

// font
static string defaultFontPath;
static Font* defaultFont = nullptr;
static std::unordered_map<string, Font*> externalFonts;

// texture
static string placeholderTexturePath;
static SDL_Texture* placeholderTexture = nullptr;
static std::unordered_map<string, SDL_Texture*> loadedTextures;
static std::map<int, RenderLayer> layers;

Window* RenderManager::getWindow()
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
    window = new Window();
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    renderer = SDL_CreateRenderer(
        window->getSdlWindow(),
        -1,
        cfgM.rendererFlags()
    );
    SDL_SetRenderDrawColor(renderer, bgR, bgG, bgB, 255);
    logInfo << "Window created: " << window->getTitle();

    // load placeholder texture
    placeholderTexturePath = cfgE.engineDataPath + "/assets/textures/misc/placeholder.png";
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    if((placeholderTexture = IMG_LoadTexture(renderer, placeholderTexturePath.c_str())) == nullptr)
        throw EngineException(
            "Failed to load placeholder texture file \"" + placeholderTexturePath + "\": " + SDL_GetError()
        );

    // load default font
    defaultFontPath = cfgE.engineDataPath + "/assets/fonts/default.ttf";
    defaultFont = new Font("", defaultFontPath);

    addLayer(0);

    logInfo << "Render manager initialization completed";
}

void RenderManager::shutdown()
{
    logInfo << "Shutting down render manager";
    // font
    delete defaultFont;
    for(auto& fp : externalFonts)
    {
        logWarn << "WHO FORGOT TO UNLOAD FONT? " << fp.second->toString();
        delete fp.second;
    }
    TTF_Quit();
    for(auto& l : layers)
        l.second.clear();
    // texture
    layers.clear();
    for(auto& tp : loadedTextures)
    {
        logWarn << "WHO FORGOT TO UNLOAD TEXTURE? File: " << tp.first << ", &: " << tp.second;
        if(tp.second != placeholderTexture)
            SDL_DestroyTexture(tp.second);
    }
    SDL_DestroyTexture(placeholderTexture);
    IMG_Quit();
    // basics
    SDL_DestroyRenderer(renderer);
    delete window;
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

void RenderManager::setBackgroundColor(unsigned int r, unsigned int g, unsigned int b)
{
    bgR = r;
    bgG = g;
    bgB = b;
    SDL_SetRenderDrawColor(renderer, bgR, bgG, bgB, 255);
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
        if(loadedTextures.at(path) != placeholderTexture)
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
    const string& fontName,
    const int& pt
)
{
    SDL_Surface* surface = TTF_RenderUTF8_Blended(
        getFont(fontName).getSdlFont(pt),
        text.c_str(),
        SDL_Color{r, g, b, a}
    );
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
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
        throw IllegalArgumentException("Parameter 'order' in RenderManager.removeLayer(order) cannot be 0");
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

RenderLayer* RenderManager::reorderLayer(const int& src, const int& target) //TODO
{
    if(src == 0 || target == 0)
        throw IllegalArgumentException("Parameter 'order' in RenderManager.reorderLayer(order) cannot be 0");
    if(src == target)
        throw IllegalArgumentException(
            "Parameter 'src' and 'target' in RenderManager.reorderLayer(src, target) cannot be the same"
        );
    return &(layers.at(target));
}

Font& RenderManager::loadFont(const string& name, const string& path)
{
    if(name.empty())
        throw IllegalArgumentException("Parameter 'name' in RenderManager.loadFont(name, path) cannot be empty");
    if(hasFont(name))
        throw EngineException("Font already loaded: " + name);
    Font* font = externalFonts.emplace(name, new Font(name, path)).first->second;
    logInfo << "Loaded " << font->toString();
    return *font;
}

void RenderManager::unloadFont(const string& name)
{
    if(name.empty())
        throw IllegalArgumentException("Parameter 'name' in RenderManager.unloadFont(name, path) cannot be empty");
    if(!hasFont(name))
        throw EngineException("Font not found: " + name);
    logInfo << "Unloading " << externalFonts.at(name)->toString();
    delete externalFonts.at(name);
    externalFonts.erase(name);
}

Font& RenderManager::getFont(const string& name)
{
    if(!hasFont(name))
        throw EngineException("Font not found: " + name);
    return name.empty() ? *defaultFont : *externalFonts.at(name);
}

bool RenderManager::hasFont(const string& name)
{
    return name.empty() || externalFonts.contains(name);
}
