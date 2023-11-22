local PLAYER_TEXTURE = "entity/dj.png"
local BG_TEXTURE = "background/litang.jpg"
local background
local layer0
local layer1
local tt = 0

function prepare()
    G = 9.8

    -- set up window
    Window.setTitle("The Amazing Adventure of Tsondu Tenzin")
    ww = Window.getWidth()
    wh = Window.getHeight()

    -- set up background
    layer0 = RenderManager.getLayer(0)
    RenderManager.loadTexture(BG_TEXTURE)
    background = layer0:addEntity(BG_TEXTURE)
    background:setLocation(ww / 2, wh / 2)
    background:setTextureSize(ww, wh)

    -- set up classes
    Runtime.execute("classes/entities/Entity.lua") -- load the Entity class
    Runtime.execute("classes/entities/Player.lua") -- load the Player class

    -- set up player
    RenderManager.loadTexture(PLAYER_TEXTURE)
    layer1 = RenderManager.addLayer(1)
    player = Player:new(layer1:addEntity(PLAYER_TEXTURE))
    player.base:setLocation(ww / 2, wh / 2)

    -- set up music & sfx
    JUMP_SOUND = SoundManager.addSound("jump", "sfx/entity/dj/1.ogg")
    WALLJUMP_SOUND = SoundManager.addSound("walljump", "sfx/entity/dj/5.ogg")
    CTRL_SOUND = SoundManager.addSound("ctrl", "sfx/entity/dj/ltc.ogg")
    SHIFT_SOUND = SoundManager.addSound("shift", "sfx/entity/dj/gm.ogg")
    BGM = SoundManager.addMusic("bgm", "music/bgm.ogg")
    BGM:play()
end

function tick()
    t = Engine.currentTick()

    -- exit
    if Keyboard.pressed(41) -- ESC
    then
        Engine.stop()
        return
    end

    -- print debug info
    if Keyboard.pressed(60) or Keyboard.repeated(60) -- F3
    then
        Runtime.log("Tick: #" .. t)
        BGM:play()
        player:printDebug()
    end

    -- player move logics (see "classes/entities/Player.lua")
    player:doMove()

    -- texture offset demo
    if Keyboard.holding(224) -- LCTRL
    then
        playingCtrlSound = playingCtrlSound or CTRL_SOUND:play()
        player.base:setTextureOffset(
                math.floor(math.sin(t / 15) * 20),
                math.floor(math.cos(t / 15) * 10)
        )
    else
        if playingCtrlSound
        then
            playingCtrlSound:stop()
            playingCtrlSound = nil
        end
        player.base:setTextureOffset(0, 0)
    end

    -- texture resizing demo
    if Keyboard.holding(225) -- LSHIFT
    then
        playingShiftSound = playingShiftSound or SHIFT_SOUND:play()
        tt = tt + 1
        local m = math.sin(tt / 15) * 10
        player.base:setTextureSize(
                math.floor(player.hitboxWidth + m),
                math.floor(player.hitboxHeight + m * player.hitboxHeight / player.hitboxWidth)
        )
    else
        if playingShiftSound
        then
            playingShiftSound:stop()
            playingShiftSound = nil
        end
        tt = 0
        player.base:resetTextureSize()
    end

    -- texture rotation demo
    if Keyboard.holding(80) -- ARROW LEFT
    then
        player.base:rotate(-0.1)
    end
    if Keyboard.holding(79) -- ARROW RIGHT
    then
        player.base:rotate(0.1)
    end

    -- music volume adjustment
    if Keyboard.holding(82) -- ARROW UP
    then
        BGM.volume(BGM.volume() + 1)
    end
    if Keyboard.holding(81) -- ARROW DOWN
    then
        BGM.volume(BGM.volume() - 1)
    end
end

function cleanup()
    BGM:stop()
    RenderManager.unloadTexture(PLAYER_TEXTURE)
    RenderManager.unloadTexture(BG_TEXTURE)
    SoundManager.removeSound(JUMP_SOUND:getName())
    SoundManager.removeSound(WALLJUMP_SOUND:getName())
    SoundManager.removeSound(CTRL_SOUND:getName())
    SoundManager.removeSound(SHIFT_SOUND:getName())
    SoundManager.removeMusic(BGM:getName())
    SoundManager.gc()
end
