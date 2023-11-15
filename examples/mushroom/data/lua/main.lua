local t
local layer0
local wx
local wy
local ww
local wh
local wr = 127
local wg = 127
local wb = 127

local player
local playerId
local PLAYER_TEXTURE = "entity/holy moly.png"

local spaceHoldingTicks = 0

function prepare()
    Runtime.log("prepare() called")
    t = Engine.currentTick()
    layer0 = RenderManager.getLayer(0)
    wx = Window.getX()
    wy = Window.getY()
    ww = Window.getWidth()
    wh = Window.getHeight()

    RenderManager.loadTexture(PLAYER_TEXTURE)

    player = layer0:addEntity(PLAYER_TEXTURE)
    playerId = player:getId()
    Runtime.log("Created player with ID: " .. playerId)

    layer0:getEntity(playerId):setLocation(wx / 2, wh / 2)
    Runtime.log("Player initial location: X=" .. player.x .. ", Y=" .. player.y)

    Runtime.log(1, "it all belongs to the other size")
end

function tick()
    t = Engine.currentTick()
    wx = Window.getX()
    wy = Window.getY()
    ww = Window.getWidth()
    wh = Window.getHeight()
    Window.setTitle("We live we love we lie " .. t)

    if Keyboard.pressed(41) or t == 3600
    then
        Engine.stop()
        return
    end

    if (player.x < 0)
    then
        wx = math.ceil(wx + player.x)
        Window.setLocation(wx, wy)
        player.x = 0
    end

    if (player.y < 0)
    then
        wy = math.ceil(wy + player.y)
        Window.setLocation(wx, wy)
        player.y = 0
    end

    if (player.x >= ww)
    then
        wx = math.floor(wx + player.x - ww)
        Window.setLocation(wx, wy)
        player.x = ww - 1
    end

    if (player.y >= wh)
    then
        wy = math.floor(wy + player.y - wh)
        Window.setLocation(wx, wy)
        player.y = wh - 1
    end

    if Keyboard.holding(26) -- W
    then
        player:move(0, -5)
    end

    if Keyboard.holding(22) -- S
    then
        player:move(0, 5)
    end

    if Keyboard.holding(4) -- A
    then
        player:move(-5, 0)
    end

    if Keyboard.holding(7) -- D
    then
        player:move(5, 0)
    end

    if Keyboard.holding(44) -- space
    then
        spaceHoldingTicks = spaceHoldingTicks + 1
        player:move(math.sin(t / 1.5) * 5, math.cos(t / 1.5) * 5)
    else
        if not (spaceHoldingTicks == 0)
        then
            spaceHoldingTicks = 0
        end
    end

    if Keyboard.pressed(43)
    then
        Runtime.log("Holy moly")
    end

    RenderManager.setBackgroundColor(
            math.floor(wr + math.sin(t / 20) * 127),
            math.floor(wg + math.cos(t / 20) * 127),
            math.floor(wb - math.sin(t / 20) * 127)
    )
end

function cleanup()
    Runtime.log("cleanup() called")
    RenderManager.unloadTexture(PLAYER_TEXTURE)
    Runtime.log(2, "we live, we love, we lie")
end

Runtime.log("Level script is running in state " .. Engine.getState())
