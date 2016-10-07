-- Copyright 2016 KeNan Liu
--
-- Licensed under the Apache License, Version 2.0 (the "License");
-- you may not use this file except in compliance with the License.
-- You may obtain a copy of the License at
--
--    http://www.apache.org/licenses/LICENSE-2.0
--
-- Unless required by applicable law or agreed to in writing, software
-- distributed under the License is distributed on an "AS IS" BASIS,
-- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
-- See the License for the specific language governing permissions and
-- limitations under the License.

local sprite = class("rd.sprite", rd.node)

function sprite:ctor(filename)
	-- call super.ctor first
	sprite.super.ctor(self)

	if nil == filename then
		return nil
	end

	local frame = rd.spriteFrameCache.getFrame(filename)
	self.csprite = Rapid2D_CSprite.new(frame.cframe)
	self.frame = frame
	return self
end

function sprite:draw(dt)
	-- do spuer draw
	sprite.super.draw(self, dt)
	self.csprite:draw(self.absMat4)
end

function sprite:setFrame(filename)
	local frame = rd.spriteFrameCache.getFrame(filename)
	if frame then
		self.csprite:setFrame(frame.cframe)
		self.frame = frame
	else
		print("Error:frame " .. filename .. " not found!")
	end
end

-- AnchorPoint
function sprite:setAnchorPoint(x, y)
	sprite.super.setAnchorPoint(self, x, y)
	self.csprite:setAnchorPoint(x, y)
	return self
end

-- set color for 4 vertex
function sprite:setColor(r, g, b, a)
	r = r or 255
	g = g or 255
	b = b or 255
	a = a or 255
	self.csprite:setColor(r, g, b, a)

	return self
end

-- return table
function sprite:getColor()
	local r, g, b, a = self.csprite:getColor()
	return {r = r, g = g, b = b, a = a}
end

-- flip
function sprite:setFlip(flipx, flipy)
	flipx = flipx or false
	flipy = flipy or false
	self.csprite:setFlip(flipx, flipy)

	return self
end

-- return flipx, flipy
function sprite:getFlip()
	local x, y= self.csprite:getFlip()
	return {x = x, y = y}
end

-- opacity
function sprite:setOpacity(opacity)
	opacity = opacity or 255
	self.csprite:setOpacity(opacity)

	return self
end

-- return opacity
function sprite:getOpacity()
	return self.csprite:getOpacity()
end

rd.sprite = sprite
