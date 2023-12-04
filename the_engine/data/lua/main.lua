local CURSOR_TEXTURE = "misc/cursor.png"
local cursorLayer
local cursor
local t
local splashText

function prepare()
    Runtime.log("Placeholder level loaded")

    -- set up window
    Window.resizable(true)

    -- set up cursor
    RenderManager.loadTexture(CURSOR_TEXTURE)
    cursorLayer = RenderManager.addLayer(2147483647)
    cursor = cursorLayer:addEntity(CURSOR_TEXTURE)
    cursor:setTextureSize(32, 32)
    cursor:setTextureOffset(cursor.textureWidth / 2, cursor.textureHeight / 2)
    Mouse.hidden(true)

    -- set up splash text
    splashText = RenderManager.getLayer(0):addText("No data!")
    splashText.textureBlue = 64
    splashText.textureGreen = 64
end

function tick()
    t = Engine.currentTick()

    if Keyboard.pressed(41) -- ESC
    then
        Engine.stop()
        return
    end

    if Keyboard.pressed(68) -- F11
    then
        Window.fullscreen(not Window.fullscreen())
    end

    -- cursor logics
    cursor:setLocation(Mouse.getX(), Mouse.getY())
    cursor.textureRed = math.floor(128 + math.sin(t / 20) * 128)
    cursor.textureGreen = math.floor(128 + math.cos(t / 20) * 128)
    cursor.textureBlue = math.floor(128 - math.sin(t / 20) * 128)

    -- splash text logics
    splashText:setLocation(Window.getWidth() / 2, Window.getHeight() / 2)
    splashText.textureAlpha = math.floor(128 + math.sin(t / 20) * 128)
end

function cleanup()
    RenderManager.unloadTexture(CURSOR_TEXTURE)
    Runtime.log("Placeholder level ended")
end
