if Engine.keyHolding(4) -- A
then
    vx = -2
end

if Engine.keyHolding(7) -- D
then
    vx = 2
end

if Engine.keyPressed(44) -- SPACE
then
    vy = -5
end

player:move(vx, vy)

if (player.x - player.hitboxWidth / 2 < 0)
then
    player.x = player.hitboxWidth / 2
end

if (player.y - player.hitboxHeight / 2 < 0)
then
    player.y = player.hitboxHeight / 2
    vy = 0
end

if (player.x + player.hitboxWidth / 2 >= ww)
then
    player.x = ww - player.hitboxWidth / 2 - 1
end

if (player.y + player.hitboxHeight / 2 >= wh)
then
    player.y = wh - player.hitboxHeight / 2 - 1
end
