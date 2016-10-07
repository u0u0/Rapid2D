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

local drawVertex = class("rd.drawVertex", rd.node)

--[[
vertexs is a table like this
{
	-- NOTE: x, y is relative to OpenGL Coordinate origin
	-- move scale rotate use MVP
	{x, y, z, r, g, b, a}, -- first vertex
	{x, y, z, r, g, b, a}, -- second vertex
	...
}
drawType:
	0	GL_POINTS
	1	GL_LINES
	2	GL_LINE_LOOP
	3	GL_LINE_STRIP
	4	GL_TRIANGLES
	5	GL_TRIANGLE_STRIP
	6	GL_TRIANGLE_FAN
pointSize: only work for GL_POINTS
]]--
function drawVertex:ctor(vertexs, drawType, pointSize)
	-- call super.ctor first
	drawVertex.super.ctor(self)
	pointSize = pointSize or 1.0
	self.cdrawvertex = Rapid2D_CDrawVertex.new(vertexs, drawType, pointSize)
	return self
end

-- AnchorPoint
function drawVertex:setAnchorPoint(x, y)
	print("[Worning]: rd.drawVertex never change AnchorPoint")
end

function drawVertex:draw(dt)
	-- do spuer draw
	drawVertex.super.draw(self, dt)
	self.cdrawvertex:draw(self.absMat4)
end

rd.drawVertex = drawVertex

-------------- draw a rect ----------------
local drawRect = class("rd.drawRect", rd.node)

-- if want to assign each vertex's color, use member function
function drawRect:ctor(width, height)
	if width <= 0.0 or height <= 0.0 then
		return nil
	end

	-- call super
	drawRect.super.ctor(self)
	-- init values
	self.frame.width = width
	self.frame.height = height
	self.vertexs = {
		{0, 0, 0, 255, 255, 255, 255},
		{0, 0, 0, 255, 255, 255, 255},
		{0, 0, 0, 255, 255, 255, 255},
		{0, 0, 0, 255, 255, 255, 255}
	}
	-- auto fill vertex
	self:setAnchorPoint(self.anchorPoint.x, self.anchorPoint.y)
	return self
end

-- AnchorPoint
function drawRect:setAnchorPoint(x, y)
	self.anchorPoint = {x = x, y = y}

    -- RightBottom
    self.vertexs[1][1] = self.frame.width * (1.0 - x)
    self.vertexs[1][2] = -(self.frame.height * y)
	self.vertexs[1][3] = 0
    -- RightTop
    self.vertexs[2][1] = self.frame.width * (1.0 - x)
    self.vertexs[2][2] = self.frame.height * (1.0 - y)
	self.vertexs[2][3] = 0
    -- LeftTop
    self.vertexs[3][1] = -(self.frame.width * x)
    self.vertexs[3][2] = self.frame.height * (1.0 - y)
	self.vertexs[3][3] = 0
    -- LeftBottom
    self.vertexs[4][1] = -(self.frame.width * x)
    self.vertexs[4][2] = -(self.frame.height * y)
	self.vertexs[4][3] = 0

	-- update OpenGL vertex
	self.cdrawvertex = Rapid2D_CDrawVertex.new(self.vertexs, 6, 1.0)
	return self	
end

-- if index = nil , set all 4 vertexs to same color
function drawRect:setColor(r, g, b, a, index)
	if index then
		self.vertexs[index][4] = r
		self.vertexs[index][5] = g
		self.vertexs[index][6] = b
		self.vertexs[index][7] = a
	else
		for i = 1, 4 do
			self.vertexs[i][4] = r
			self.vertexs[i][5] = g
			self.vertexs[i][6] = b
			self.vertexs[i][7] = a
		end
	end
	-- update OpenGL vertex
	self.cdrawvertex = Rapid2D_CDrawVertex.new(self.vertexs, 6, 1.0)
	return self
end

function drawRect:draw(dt)
	-- do spuer draw
	drawRect.super.draw(self, dt)
	self.cdrawvertex:draw(self.absMat4)
end

rd.drawRect = drawRect

-------------- draw a circle ----------------
-- local drawCircle = class("rd.drawCircle", rd.drawVertex)

-- function drawCircle:ctor()
-- 	-- call super first
-- 	drawCircle.super.ctor(self)
-- 	return self
-- end

-- rd.drawCircle = drawCircle
