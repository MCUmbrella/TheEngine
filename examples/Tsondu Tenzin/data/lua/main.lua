local PLAYER_TEXTURE = "entity/dj.png"
local BG_TEXTURE = "background/litang.jpg"
local background
local layer0
local layer1
local tt = 0

function printDebug()
    Runtime.log("Tick: #" .. t)
    Runtime.log("ID: " .. player:getId())
    Runtime.log("Location: " .. player.x .. ", " .. player.y)
    Runtime.log("Hixbox size: " .. player.hitboxWidth .. " * " .. player.hitboxHeight)
    Runtime.log("Texture size: " .. player.textureWidth .. " * " .. player.textureHeight)
    Runtime.log("texture upper-left location: " .. player.x - player.textureWidth / 2 .. ", " .. player.y - player.textureHeight / 2)
    Runtime.log("texture bottom-right location: " .. player.x + player.textureWidth / 2 .. ", " .. player.y + player.textureHeight / 2)
    Runtime.log("v: " .. vx .. ", " .. vy)
end

function prepare()
    G = 9.8
    vx = 0
    vy = 0
    RenderManager.setWindowTitle("The Amazing Adventure of Tsondu Tenzin")
    RenderManager.loadTexture(PLAYER_TEXTURE)
    RenderManager.loadTexture(BG_TEXTURE)

    ww = RenderManager.getWindowWidth()
    wh = RenderManager.getWindowHeight()
    layer0 = RenderManager.getLayer(0)
    layer1 = RenderManager.addLayer(1)
    background = layer0:addEntity(BG_TEXTURE)
    background:setLocation(ww / 2, wh / 2)
    background:resizeTexture(ww, wh)

    JUMP_SOUND = SoundManager.addSound("jump", "sfx/entity/dj/jump.wav")
    WALLJUMP_SOUND = SoundManager.addSound("walljump", "sfx/entity/dj/walljump.wav")

    player = layer1:addEntity(PLAYER_TEXTURE)
    player:setLocation(ww / 2, wh / 2)
end

function tick()
    t = Engine.currentTick()

    if Engine.keyPressed(41) -- ESC
    then
        Engine.stop()
        return
    end

    if Engine.keyHolding(224) -- LCTRL
    then
        player:setTextureOffset(
                math.floor(math.sin(t / 15) * 20),
                math.floor(math.cos(t / 15) * 10)
        )
    else
        player:setTextureOffset(0, 0)
    end

    if Engine.keyHolding(225) -- LSHIFT
    then
        tt = tt + 1
        local m = math.sin(tt / 15) * 10
        player:resizeTexture(
                math.floor(player.hitboxWidth + m),
                math.floor(player.hitboxHeight + m * player.hitboxHeight / player.hitboxWidth)
        )
    else
        tt = 0
        player:resetTextureSize()
    end

    Runtime.switchTo("playerMoveLogics.lua")

    if Engine.keyPressed(60) or Engine.keyRepeated(60) -- F3
    then
        printDebug()
    end
end

function cleanup()
    RenderManager.unloadTexture(PLAYER_TEXTURE)
    RenderManager.unloadTexture(BG_TEXTURE)
    SoundManager.removeSound(JUMP_SOUND:getName())
    SoundManager.removeSound(WALLJUMP_SOUND:getName())
    SoundManager.gc()
end
