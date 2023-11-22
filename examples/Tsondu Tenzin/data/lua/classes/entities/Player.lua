Player = {}
Player.__index = Player
setmetatable(Player, Entity)

function Player:new(base)
    local p = Entity:new(base)
    setmetatable(p, Player)
    return p
end

function Player:onTop()
    return self.base.y - self.hitboxHeight / 2 <= 0
end

function Player:onGround()
    return self.base.y + self.hitboxHeight / 2 >= wh
end

function Player:onLeft()
    return self.base.x - self.hitboxWidth / 2 <= 0
end

function Player:onRight()
    return self.base.x + self.hitboxWidth / 2 >= ww
end

function Player:jump() -- let the player jump
    if self:onGround() then -- ground jump
        self.vy = -5
        JUMP_SOUND:play()
    else
        if self:onLeft() then -- wall jump
            self.vy = -5
            self.vx = 5
            WALLJUMP_SOUND:play()
        elseif self:onRight() then
            self.vy = -5
            self.vx = -5
            WALLJUMP_SOUND:play()
        end
    end
end

function Player:doMove() -- check the key press and do the move
    self.vy = self.vy + G / 60 -- apply the gravity first

    if Keyboard.holding(4) and not Keyboard.holding(7) -- A: move left
    then
        self.base:setFlip(1)
        self.vx = math.max(-5, self.vx - 0.5)
    elseif Keyboard.holding(7) and not Keyboard.holding(4) -- D: move right
    then
        self.base:setFlip(0)
        self.vx = math.min(5, self.vx + 0.5)
    else
        if (self.vx > 0)
        then
            self.vx = math.max(self.vx - 0.5, 0)
        elseif (self.vx < 0)
        then
            self.vx = math.min(self.vx + 0.5, 0)
        end
    end

    if Keyboard.holding(44) -- SPACE
    then
        self:jump()
    end

    self.base:move(self.vx, self.vy) -- do the actual move

    -- if the player has moved out of the window, put it back in
    if self:onTop()
    then
        self.base.y = self.hitboxHeight / 2
        self.vy = 0
    end

    if self:onGround()
    then
        self.base.y = wh - self.hitboxHeight / 2
        self.vy = 0
    end

    if self:onLeft()
    then
        self.base.x = self.hitboxWidth / 2
        self.vx = 0
    end

    if self:onRight()
    then
        self.base.x = ww - self.hitboxWidth / 2
        self.vx = 0
    end
end

--[[
public class Player extends Entity
{
    public Player(RenderEntity base);
    public Boolean onTop();
    public Boolean onGround();
    public Boolean onLeft();
    public Boolean onRight();
    public Void jump();
    public Void doMove();
}
]]--
