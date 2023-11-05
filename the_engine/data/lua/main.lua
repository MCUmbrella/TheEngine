local t

function prepare()
    Runtime.log("Placeholder level loaded")
end

function tick()
    t = Engine.currentTick()
    if t % 120 == 0
    then
        Runtime.log(1, "No data!")
    end
end

function cleanup()
    Runtime.log("Placeholder level ended")
end 
