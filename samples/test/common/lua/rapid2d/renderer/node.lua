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

-- Base class for draw
local node = class("rd.node")

function node:ctor(width, height)
	width = width or 0
	height = height or 0

	self.children = {} -- make a node tree
	self.zorder = 0
	self.pos = {x = 0, y = 0, z = 0}
	self.rotation = {x = 0, y = 0, z = 0}
	self.scale = {x = 1, y = 1, z = 1}
	self.frame = {width = width, height = height}
	self.anchorPoint = {x = 0.5, y = 0.5}
	self.visible = true

	-- mat4 for tree rendering
	self.relativeMat4 = Rapid2D_CMat4.new()
	self.absMat4 = Rapid2D_CMat4.new()
	Rapid2D_CMat4.modelView(self.relativeMat4, self.pos, self.rotation, self.scale)
	Rapid2D_CMat4.modelView(self.absMat4, self.pos, self.rotation, self.scale)
	return self
end

function node:setPos(x, y)
	self.pos.x = x or self.pos.x
	self.pos.y = y or self.pos.y
	return self
end

-- return {x = x, y = y}
function node:getPos()
	-- make a new table to return, data independent
	return {x = self.pos.x, y = self.pos.y}
end

function node:setRotation(x, y, z)
	self.rotation.x = x or self.rotation.x
	self.rotation.y = y or self.rotation.y
	self.rotation.z = z or self.rotation.z
	return self
end

-- return {x = x, y = y, z = z}
function node:getRotation()
	-- make a new table to return, data independent
	return {x = self.rotation.x, y = self.rotation.y, z = self.rotation.z}
end

function node:setScale(x, y)
	self.scale.x = x or self.scale.x
	self.scale.y = y or self.scale.y
	self.scale.z = z or self.scale.z
	return self
end

-- return {x = x, y = y, z = z}
function node:getScale()
	-- make a new table to return, data independent
	return {x = self.scale.x, y = self.scale.y, z = self.scale.z}
end

function node:addTo(parent, z)
	if self.parent then
		print("[Error]: node can't add twice")
		return
	end
	parent:add(self, z)
	return self
end

function node:add(child, z)
	if child.parent then
		print("[Error]: node can't add twice")
		return
	end
	child.parent = self
	child.zorder = z or child.zorder
	-- insert by zorder
	local notInserted = true
	for k, v in ipairs(self.children) do
		if child.zorder < v.zorder then
			notInserted = false
			table.insert(self.children, k, child)
			break
		end
	end
	if notInserted then
		table.insert(self.children, child) -- add to tail
	end

	return self
end

-- zorder for 2D rendering
function node:setZorder(z)
	if self.zorder == z then
		return self
	end

	if self.parent then
		local par = self.parent
		par:remove(self)
		par:add(self, z)
	else
		self.zorder = z or self.zorder
	end
	return self
end

function node:getZorder()
	return self.zorder
end

function node:removeSelf()
	self.parent:remove(self)
end

function node:remove(child)
	for k, v in ipairs(self.children) do
		if v == child then
			-- remove pos and move others to front
			child.parent = nil
			table.remove(self.children, k)
			break
		end
	end

	rd.actionManager.removeActions(child)
end

-- return {width = width, height = height}
function node:getSize()
	-- make a new table to return, data independent
	return {width = self.frame.width, height = self.frame.height}
end

-- AnchorPoint
function node:setAnchorPoint(x, y)
	self.anchorPoint = {x = x, y = y}
	return self
end

-- return {x = x, y = y}
function node:getAnchorPoint()
	-- make a new table to return, data independent
	return {x = self.anchorPoint.x, y = self.anchorPoint.y}
end

function node:setVisible(visible)
	self.visible = visible
end

function node:getVisible()
	return self.visible
end

-- ONLY work right after first draw
function node:getWorldPos()
	return Rapid2D_CMat4.extractTranslation(self.absMat4)
end

-- ONLY work right after first draw
function node:getWorldRotation()
	return Rapid2D_CMat4.extractRotation(self.absMat4)
end

-- ONLY work right after first draw
function node:getWorldScale()
	return Rapid2D_CMat4.extractScale(self.absMat4)
end

-- ONLY work right after first draw
function node:convertToNodeSpace(worldPoint)
	return Rapid2D_CMat4.pointToNodeSpace(self.absMat4, worldPoint)
end

-- ONLY work right after first draw
function node:convertToWorldSpace(nodePoint)
	return Rapid2D_CMat4.pointToWorldSpace(self.absMat4, nodePoint)
end

function node:getBoundingBox()
	local anchor = self:getAnchorPoint()
	local size = self:getSize()
	local leftBottomX = - (size.width * anchor.x)
	local leftBottomY = - (size.height * anchor.y)
	return {x = leftBottomX, y = leftBottomY, width = size.width, height = size.height}
end

function node:schedule(callback, interval)
    local seq = rd.actionSequence.new(
		rd.actionDelay.new(interval),
		rd.actionCallback.new(callback)
    )
	local repeatForever = rd.actionRepeat.new(seq, -1)
    self:runAction(repeatForever)
	return repeatForever
end

function node:performWithDelay(callback, delay)
    local seq = rd.actionSequence.new(
        rd.actionDelay.new(delay),
		rd.actionCallback.new(callback)
    )
    self:runAction(seq)
    return seq
end

function node:onFrame(func)
	self.frameCallback = func
	return self
end

function node:onTouch(func)
	self.touchCallback = func
	return self
end

function node:onKey(func)
	self.keyCallback = func
	return self
end

-- traversing children by draw order
-- XXX: never add or remove node while walk, it will break children table
-- callback = function(node)
-- end
function node:walk(callback)
	for k, v in ipairs(self.children) do
		if v.visible then
			callback(v)
			v:walk(callback)
		end
	end
end

function node:draw(dt)
	-- The origin of relative coordinate be fixed to parent's center
	local position = {
		x = self.pos.x + (0.5 - self.parent.anchorPoint.x) * self.parent.frame.width,
		y = self.pos.y + (0.5 - self.parent.anchorPoint.y) * self.parent.frame.height,
		z = 0
	}
	Rapid2D_CMat4.modelView(self.relativeMat4, position, self.rotation, self.scale)
	Rapid2D_CMat4.multiply(self.absMat4, self.parent.absMat4, self.relativeMat4)
end

function node:runAction(action)
	rd.actionManager.addRootAction(action, self)
	return self
end

function node:getActionCounts()
	return rd.actionManager.getRootActionCounts(self)
end

-- remove a action add by runAction
function node:removeAction(action)
	rd.actionManager.remove(action)
	return self
end

rd.node = node
