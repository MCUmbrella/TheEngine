function playerOnTop()
    return player.y - player.hitboxHeight / 2 <= 0
end

function playerOnGround()
    return player.y + player.hitboxHeight / 2 >= wh - 1
end

function playerOnLeft()
    return player.x - player.hitboxWidth / 2 <= 0
end

function playerOnRight()
    return player.x + player.hitboxWidth / 2 >= ww - 1
end

vy = vy + G / 60

if Engine.keyHolding(4) -- A
then
    if vx > -5
    then
        vx = vx - 0.5
    end
elseif Engine.keyHolding(7) -- D
then
    if vx < 5
    then
        vx = vx + 0.5
    end
else
    if (vx > 0)
    then
        vx = math.max(vx - 0.5, 0)
    elseif (vx < 0)
    then
        vx = math.min(vx + 0.5, 0)
    end
end

if Engine.keyHolding(44) -- SPACE
then
    if playerOnGround() then -- ground jump
        vy = -5
        SoundManager.playSound(JUMP_SOUND)
    else
        if playerOnLeft() then -- wall jump
            vy = -5
            vx = 5
            SoundManager.playSound(WALLJUMP_SOUND)
        elseif playerOnRight() then
            vy = -5
            vx = -5
            SoundManager.playSound(WALLJUMP_SOUND)
        end
    end
end

player:move(vx, vy)

if playerOnTop()
then
    player.y = player.hitboxHeight / 2
    vy = 0
end

if playerOnGround()
then
    player.y = wh - player.hitboxHeight / 2 - 1
    vy = 0
end

if playerOnLeft()
then
    player.x = player.hitboxWidth / 2
    vx = 0
end

if playerOnRight()
then
    player.x = ww - player.hitboxWidth / 2 - 1
    vx = 0
end
