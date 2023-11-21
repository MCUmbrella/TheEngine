Entity = {}
Entity.__index = Entity

function Entity:new(base)
    local e = {}
    setmetatable(e, Entity)
    e.base = base
    e.hitboxWidth = base.textureWidth
    e.hitboxHeight = base.textureHeight
    e.vx = 0
    e.vy = 0
    return e
end

function Entity:printDebug()
    Runtime.log("ID: " .. self.base:getId())
    Runtime.log("Location: " .. self.base.x .. ", " .. self.base.y)
    Runtime.log("Hixbox size: " .. self.hitboxWidth .. " * " .. self.hitboxHeight)
    Runtime.log("Texture size: " .. self.base.textureWidth .. " * " .. self.base.textureHeight)
    Runtime.log("texture upper-left location: " .. self.base.x - self.base.textureWidth / 2 .. ", " .. self.base.y - self.base.textureHeight / 2)
    Runtime.log("texture bottom-right location: " .. self.base.x + self.base.textureWidth / 2 .. ", " .. self.base.y + self.base.textureHeight / 2)
    Runtime.log("Velocity: " .. self.vx .. ", " .. self.vy)
end

--[[
public class Entity
{
    public final RenderEntity base;
    public Number hitboxWidth, hitboxHeight, vx, vy;
    public Entity(RenderEntity base);
    public Void printDebug();
}
]]--
