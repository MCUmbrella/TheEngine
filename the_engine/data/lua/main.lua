local CURSOR_TEXTURE = "misc/cursor.png"
local cursor
local t
local splashText

function prepare()
    Runtime.log("Placeholder level loaded")
    RenderManager.loadTexture(CURSOR_TEXTURE)
    local layerCursor = RenderManager.addLayer(2147483647)
    cursor = layerCursor:addEntity(CURSOR_TEXTURE)
    cursor:setTextureSize(32, 32)
    cursor:setTextureOffset(cursor.textureWidth / 2, cursor.textureHeight / 2)
    Mouse.hidden(true)

    splashText = RenderManager.getLayer(0):addText("No data!")
    splashText:setLocation(Window.getWidth() / 2, Window.getHeight() / 2)
    splashText.textureBlue = 64
    splashText.textureGreen = 64
end

function tick()
    t = Engine.currentTick()

    if Keyboard.pressed(41)
    then
        Engine.stop()
        return
    end

    cursor:setLocation(Mouse.getX(), Mouse.getY())
    cursor.textureRed = math.floor(128 + math.sin(t / 20) * 128)
    cursor.textureGreen = math.floor(128 + math.cos(t / 20) * 128)
    cursor.textureBlue = math.floor(128 - math.sin(t / 20) * 128)
    splashText.textureAlpha = math.floor(128 + math.sin(t / 20) * 128)
end

function cleanup()
    RenderManager.unloadTexture(CURSOR_TEXTURE)
    Runtime.log("Placeholder level ended")
end 
