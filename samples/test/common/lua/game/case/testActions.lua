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

local testBase = require("game.testBase")
local TestActions = class("TestActions", testBase)

function TestActions:ctor()
	-- init case first
	self.testcase = {
		"actionMove",
		"actionScale",
		"actionRotate",
		"actionFade",
		"actionBlink",
		"actionTint",
		"actionAnimation",
		"actionBezier",
		"actionEase",
	}
	rd.spriteFrameCache.addFrames("pig.json")
	-- then call super.ctor
	TestActions.super.ctor(self)

	-- create success
	return self
end

function TestActions:actionMove()
	self.testNode = rd.node.new():addTo(self)
	rd.label.TTF.new("Move a sprite from (-100, 0) to (100, 0)",
		25, "Arial", {0, 0, 0xc6, 255}, rd.label.CENTER)
		:setPos(0, 200)
		:addTo(self.testNode)
	rd.sprite.new("pig_1.png")
		:setPos(-100, 0)
		:runAction(rd.actionMove.new({x = 100, y = 0}, 1.5))
		:addTo(self.testNode)
end

function TestActions:actionScale()
	self.testNode = rd.node.new():addTo(self)
	rd.label.TTF.new("Scale a sprite from (1, 1) to (2, 1.5)",
		25, "Arial", {0, 0, 0xc6, 255}, rd.label.CENTER)
		:setPos(0, 200)
		:addTo(self.testNode)
	rd.sprite.new("pig_1.png")
		:setPos(0, 0)
		:runAction(rd.actionScale.new({x = 2, y = 1.5}, 1.5))
		:addTo(self.testNode)
end

function TestActions:actionRotate()
	self.testNode = rd.node.new():addTo(self)
	rd.label.TTF.new("Rotate a sprite from z=0 to z=270",
		25, "Arial", {0, 0, 0xc6, 255}, rd.label.CENTER)
		:setPos(0, 200)
		:addTo(self.testNode)
	rd.sprite.new("pig_1.png")
		:setPos(0, 0)
		:runAction(rd.actionRotate.new({z = 270}, 1.5))
		:addTo(self.testNode)
end

function TestActions:actionFade()
	self.testNode = rd.node.new():addTo(self)
	rd.label.TTF.new("Fade a sprite from 255 to 100",
		25, "Arial", {0, 0, 0xc6, 255}, rd.label.CENTER)
		:setPos(0, 200)
		:addTo(self.testNode)
	rd.sprite.new("pig_1.png")
		:setPos(0, 0)
		:runAction(rd.actionFade.new(100, 1.5))
		:addTo(self.testNode)
end

function TestActions:actionBlink()
	self.testNode = rd.node.new():addTo(self)
	rd.label.TTF.new("Blink a sprite",
		25, "Arial", {0, 0, 0xc6, 255}, rd.label.CENTER)
		:setPos(0, 200)
		:addTo(self.testNode)
	rd.sprite.new("pig_1.png")
		:setPos(0, 0)
		:runAction(rd.actionBlink.new(8, 1.5))
		:addTo(self.testNode)
end

function TestActions:actionTint()
	self.testNode = rd.node.new():addTo(self)
	rd.label.TTF.new("Tint a sprite to red",
		25, "Arial", {0, 0, 0xc6, 255}, rd.label.CENTER)
		:setPos(0, 200)
		:addTo(self.testNode)
	rd.sprite.new("pig_1.png")
		:setPos(0, 0)
		:runAction(rd.actionTint.new({r=255, g=0, b=0, a=255}, 1.5))
		:addTo(self.testNode)
end

function TestActions:actionAnimation()
	self.testNode = rd.node.new():addTo(self)
	rd.label.TTF.new("Frame animation of a sprite, repeatForever",
		25, "Arial", {0, 0, 0xc6, 255}, rd.label.CENTER)
		:setPos(0, 200)
		:addTo(self.testNode)

	local frames = {}
	for i=1, 6 do
		frames[i] = "pig_" .. i .. ".png"
	end

	rd.sprite.new("pig_1.png")
		:setPos(0, 0)
		:runAction(rd.actionRepeat.new(rd.actionAnimation.new(frames, 0.1), -1))
		:addTo(self.testNode)
end

function TestActions:actionBezier()
	self.testNode = rd.node.new():addTo(self)
	rd.label.TTF.new("Bezier , delay, callback, actionSequence",
		25, "Arial", {0, 0, 0xc6, 255}, rd.label.CENTER)
		:setPos(0, 200)
		:addTo(self.testNode)
	
	-- half circle
	local config = {
		{x = -100, y = 100},
		{x = 100, y = 100},
		{x = 100, y = 0}
	}
	rd.sprite.new("pig_1.png")
		:setPos(-100, 0)
		:runAction(
			rd.actionSequence.new(
				rd.actionBezier.new(config, 1.5),
				rd.actionDelay.new(1),
				rd.actionCallback.new(function (target)
					print(target)
				end)
			)
		)
		:addTo(self.testNode)
end

function TestActions:actionEase()
	self.testNode = rd.node.new():addTo(self)
	rd.label.TTF.new("Move, Ease, Rotate, actionConcurrency",
		25, "Arial", {0, 0, 0xc6, 255}, rd.label.CENTER)
		:setPos(0, 200)
		:addTo(self.testNode)
	
	rd.sprite.new("pig_1.png")
		:setPos(-100, 0)
		:runAction(
			rd.actionConcurrency.new(
				rd.actionRotate.new({z=360}, 1.5),
				rd.actionEase.new(
					rd.actionMove.new({x=100, y=0}, 1.5),
					"sineEaseIn"
				)
			)
		)
		:addTo(self.testNode)
end

return TestActions
