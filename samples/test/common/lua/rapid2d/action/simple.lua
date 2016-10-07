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


------------------- callback -----------------
local actionCallback = class("rd.actionCallback", rd.action)

function actionCallback:ctor(callback)
	-- call super first
	actionCallback.super.ctor(self, 0)
	self.callback = callback
	return self
end

function actionCallback:update(dt)
	self.callback(self.target)
	self.target = nil
end

rd.actionCallback = actionCallback

------------------- move -----------------
local actionMove = class("rd.actionMove", rd.action)

function actionMove:ctor(endPos, duration)
	-- call super first
	actionMove.super.ctor(self, duration)
	self.endPos = endPos

	return self
end

function actionMove:start(target)
	-- call super first
	actionMove.super.start(self, target)
	self.startPos = self.target:getPos()
end

function actionMove:step(scale)
	local x = self.startPos.x + (self.endPos.x - self.startPos.x) * scale
	local y = self.startPos.y + (self.endPos.y - self.startPos.y) * scale
	self.target:setPos(x, y)
end

rd.actionMove = actionMove

------------------- scale -----------------
local actionScale = class("rd.actionScale", rd.action)

function actionScale:ctor(endScale, duration)
	-- call super first
	actionScale.super.ctor(self, duration)
	self.endScale = endScale

	return self
end

function actionScale:start(target)
	-- call super first
	actionScale.super.start(self, target)
	self.startScale = self.target:getScale()
end

function actionScale:step(scale)
	local x = self.startScale.x + (self.endScale.x - self.startScale.x) * scale
	local y = self.startScale.y + (self.endScale.y - self.startScale.y) * scale
	self.target:setScale(x, y, self.startScale.z)
end

rd.actionScale = actionScale

------------------- rotate -----------------
local actionRotate = class("rd.actionRotate", rd.action)

function actionRotate:ctor(endRotation, duration)
	-- call super first
	actionRotate.super.ctor(self, duration)
	self.endRotation = endRotation

	return self
end

function actionRotate:start(target)
	-- call super first
	actionRotate.super.start(self, target)
	self.startRotation = self.target:getRotation()
end

function actionRotate:step(scale)
	local z = self.startRotation.z + (self.endRotation.z - self.startRotation.z) * scale
	self.target:setRotation(self.startRotation.x, self.startRotation.y, z)
end

rd.actionRotate = actionRotate

------------------- Fade :ONLY work for sprite -----------------
local actionFade = class("rd.actionFade", rd.action)

function actionFade:ctor(endOpacity, duration)
	-- call super first
	actionFade.super.ctor(self, duration)
	self.endOpacity = endOpacity

	return self
end

function actionFade:start(target)
	-- call super first
	actionFade.super.start(self, target)
	self.startOpacity = self.target:getOpacity()
end

function actionFade:step(scale)
	local opacity = self.startOpacity + (self.endOpacity - self.startOpacity) * scale
	self.target:setOpacity(math.floor(opacity))
end

rd.actionFade = actionFade

------------------- Blink :ONLY work for sprite -----------------
local actionBlink = class("rd.actionBlink", rd.action)

function actionBlink:ctor(times, duration)
	-- call super first
	actionBlink.super.ctor(self, duration)
	self.slice = 1.0 / times

	return self
end

function actionBlink:step(scale)
	if 1 == scale then
		self.target:setVisible(true)
		return
	end

	local m = math.fmod(scale, self.slice)
	if m > (self.slice / 2.0) then
		self.target:setVisible(true)
	else
		self.target:setVisible(false)
	end
end

rd.actionBlink = actionBlink

------------------- Tint :ONLY work for sprite -----------------
local actionTint = class("rd.actionTint", rd.action)

function actionTint:ctor(endColor, duration)
	-- call super first
	actionTint.super.ctor(self, duration)
	self.endColor = endColor

	return self
end

function actionTint:start(target)
	-- call super first
	actionTint.super.start(self, target)
	self.startColor = self.target:getColor()
end

function actionTint:step(scale)
	local r = self.startColor.r + (self.endColor.r - self.startColor.r) * scale
	local g = self.startColor.g + (self.endColor.g - self.startColor.g) * scale
	local b = self.startColor.b + (self.endColor.b - self.startColor.b) * scale
	local a = self.startColor.a + (self.endColor.a - self.startColor.a) * scale
	self.target:setColor(math.floor(r), math.floor(g), math.floor(b), math.floor(a))
end

rd.actionTint = actionTint

------------------- Animation :ONLY work for sprite -----------------
local actionAnimation = class("rd.actionAnimation", rd.action)

function actionAnimation:ctor(frames, delay)
	-- call super first
	actionAnimation.super.ctor(self, delay * #frames)
	self.frames = frames
	self.gap = 1.0 / #frames

	return self
end

function actionAnimation:start(target)
	-- call super first
	actionAnimation.super.start(self, target)
	self.frameIndex = 0
end

function actionAnimation:step(scale)
	local frameIndex = math.floor(scale / self.gap)
	if frameIndex ~= self.frameIndex then
		self.target:setFrame(self.frames[frameIndex])
		self.frameIndex = frameIndex
	end
end

rd.actionAnimation = actionAnimation

------------------- Bezier :ONLY work for sprite -----------------
-- config {
--	{x = control1_x, y = control1_y}
--	{x = control2_x, y = control2_y}
--	{x = endx, y = endy}
-- }
local actionBezier = class("rd.actionBezier", rd.action)

function actionBezier:ctor(config, duration)
	-- call super first
	actionBezier.super.ctor(self, duration)
	self.config = config

	return self
end

function actionBezier:start(target)
	-- call super first
	actionBezier.super.start(self, target)
	self.startPos = self.target:getPos()
end

function actionBezier:step(scale)
	local x = math.cubicBezier(self.startPos.x, 
								self.config[1].x,
								self.config[2].x,
								self.config[3].x,
								scale)
	local y = math.cubicBezier(self.startPos.y, 
								self.config[1].y,
								self.config[2].y,
								self.config[3].y,
								scale)
	self.target:setPos(x, y)
end

rd.actionBezier = actionBezier
