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

local scene = class("rd.scene", rd.node)

function scene:ctor()
	-- call super.ctor first
	scene.super.ctor(self)

	self.backgroundColor = {r = 255, g = 255, b = 255, a = 255}
	-- key is point's identify, value is targeting node
	self.trackingPoints = {}
	-- ONLY scene control multiTouch mode
	-- NEVER set it directory, use set setTouchMode()
	self._isMultiTouch = false
	return self
end

function scene:setBackgroundColor(r, g, b, a)
	self.backgroundColor = {r = r, g = g, b = b, a = a}
end

function scene:drawTree(dt)
	-- make a new table to avoid break the tree while walk
	local frameNodes = {}
	-- check scene
	if self.frameCallback then
		table.insert(frameNodes, self)
	end

	-- scene draw background color
	Rapid2D_COpenGL.clearColor(self.backgroundColor)
	-- draw children
	self:walk(function(node)
		-- check frameEvent
		if node.frameCallback then
			table.insert(frameNodes, node)
		end

		node:draw(dt)
	end)

	-- fire frameEvent
	for k, value in pairs(frameNodes) do
		value:frameCallback(dt)
	end
end

-- true for MultiTouch, false for single touch
function scene:enableMultiTouch(isMultiTouch)
	if self._isMultiTouch == isMultiTouch then
		return
	end

	self:endTouchs()
	-- set new mode
	self._isMultiTouch = isMultiTouch
end

-- use for scene change
function scene:endTouchs()
	local points = {}
	for k, point in pairs(self.trackingPoints) do
		table.insert(points, point)
	end
	self:dispatchTouchEvent("ended", points)
end

function scene:dispatchTouchEvent(t, points)
	if false == self._isMultiTouch and "began" == t then
		if table.nums(self.trackingPoints) > 0 then
			-- have tracking a point
			return
		end
		-- start tracking first point
		points = {points[1]}
	end

	local targetingNodes = {}
	for k, point in ipairs(points) do
		if "began" == t then
			local target = nil
			-- check if scene listen touch
			if self.touchCallback then
				target = self
			end

			-- check children
			self:walk(function(node)
				-- touch point is a world point
				if node.touchCallback and math.rectContainsPoint(node:getBoundingBox(), node:convertToNodeSpace(point)) then
					target = node
				end
			end)

			-- now we found top listened node, set point to node
			if target then
				-- all touched node add to a set
				targetingNodes[target] = true

				-- target will know all it's points
				target.tmpTouchPoints = target.tmpTouchPoints or {}
				table.insert(target.tmpTouchPoints, point) -- add to tail

				-- add to tracking
				point.target = target
				point.prex = point.x
				point.prey = point.y
				self.trackingPoints[point.identify] = point
			end
		else -- moved or ended
			local trackedPoint = self.trackingPoints[point.identify]
			if trackedPoint then
				trackedPoint.prex = trackedPoint.x
				trackedPoint.prey = trackedPoint.y
				trackedPoint.x = point.x
				trackedPoint.y = point.y

				-- all touched node add to a set
				targetingNodes[trackedPoint.target] = true

				-- target will know all it's points
				trackedPoint.target.tmpTouchPoints = trackedPoint.target.tmpTouchPoints or {}
				table.insert(trackedPoint.target.tmpTouchPoints, trackedPoint) -- add to tail
			end

			-- stop tracking
			if "ended" == t then
				self.trackingPoints[point.identify] = nil
			end
		end
	end
	
	-- do dispatch from set
	for node, v in pairs(targetingNodes) do
		node:touchCallback(t, node.tmpTouchPoints)
		node.tmpTouchPoints = nil
	end
end

function scene:dispatchKeyBoardEvent(t, code)
	-- make a new table to avoid break the tree while walk
	local responseNodes = {}

	-- check scene
	if self.keyCallback then
		table.insert(responseNodes, self)
	end

	-- check children
	self:walk(function(node)
		if node.keyCallback then
			table.insert(responseNodes, node)
		end
	end)

	for k, value in pairs(responseNodes) do
		value:keyCallback(t, code)
	end
end

rd.scene = scene
