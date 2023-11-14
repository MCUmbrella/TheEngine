local CURSOR_TEXTURE = "misc/cursor.png"
local cursor
local t

function prepare()
    Runtime.log("Placeholder level loaded")
    RenderManager.loadTexture(CURSOR_TEXTURE)
    local layerCursor = RenderManager.addLayer(2147483647)
    cursor = layerCursor:addEntity(CURSOR_TEXTURE)
    cursor:setTextureSize(32, 32)
    cursor:setTextureOffset(cursor.textureWidth / 2, cursor.textureHeight / 2)
    Mouse.hidden(true)
end

function tick()
    t = Engine.currentTick()

    if Keyboard.pressed(41)
    then
        Engine.stop()
        return
    end

    if t % 300 == 0
    then
        Runtime.log(1, "No data!")
    end

    cursor:setLocation(Mouse.getX(), Mouse.getY())
    cursor.textureRed = math.floor(128 + math.sin(t / 20) * 128)
    cursor.textureGreen = math.floor(128 + math.cos(t / 20) * 128)
    cursor.textureBlue = math.floor(128 - math.sin(t / 20) * 128)
end

function cleanup()
    Runtime.log("Placeholder level ended")
    RenderManager.unloadTexture(CURSOR_TEXTURE)
end 
