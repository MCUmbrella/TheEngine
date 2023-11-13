local t
local cursor = RenderManager.getLayer(0):addEntity("misc/placeholder.png")

function prepare()
    Runtime.log("Placeholder level loaded")
    cursor:setTextureSize(16, 16)
end

function tick()
    t = Engine.currentTick()
    if t % 120 == 0
    then
        Runtime.log(1, "No data!")
    end

    cursor:setLocation(Mouse.getX(), Mouse.getY())
end

function cleanup()
    Runtime.log("Placeholder level ended")
end 
