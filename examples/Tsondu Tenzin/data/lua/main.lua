local PLAYER_TEXTURE = "entity/dj.png"
local BG_TEXTURE = "background/litang.jpg"
JUMP_SOUND = "sfx/entity/dj/jump.wav"
WALLJUMP_SOUND = "sfx/entity/dj/walljump.wav"
local background
local layer0
local layer1
local tt = 0

function printPlayer()
    Runtime.log("ID: " .. player:getId())
    Runtime.log("Location: " .. player.x .. ", " .. player.y)
    Runtime.log("Hixbox size: " .. player.hitboxWidth .. " * " .. player.hitboxHeight)
    Runtime.log("Texture size: " .. player.textureWidth .. " * " .. player.textureHeight)
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

    SoundManager.loadSound(JUMP_SOUND, JUMP_SOUND)
    SoundManager.loadSound(WALLJUMP_SOUND, WALLJUMP_SOUND)

    player = layer1:addEntity(PLAYER_TEXTURE)
    player:setLocation(ww / 2, wh / 2)

    printPlayer()
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
end

function cleanup()
    RenderManager.unloadTexture(PLAYER_TEXTURE)
    RenderManager.unloadTexture(BG_TEXTURE)
    SoundManager.unloadSound(JUMP_SOUND)
    SoundManager.unloadSound(WALLJUMP_SOUND)
    SoundManager.gc()
end
