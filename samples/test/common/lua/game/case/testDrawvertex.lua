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
local TestDrawvertex = class("TestDrawvertex", testBase)

function TestDrawvertex:ctor()
	-- init case first
	self.testcase = {
		"drawPoint",
		"drawLine",
		"drawTriangle",
		"drawRect"
	}
	-- then call super.ctor
	TestDrawvertex.super.ctor(self)

	-- create success
	return self
end

function TestDrawvertex:drawPoint()
	self.testNode = rd.node.new():addTo(self)
	rd.label.TTF.new("Draw a black point at screen center",
		25, "Arial", {0, 0, 0xc6, 255}, rd.label.CENTER)
		:setPos(0, 100)
		:addTo(self.testNode)
	rd.drawVertex.new({{0, 0, 0, 0, 0, 0, 255}}, 0, 10)
		:addTo(self.testNode)
end

function TestDrawvertex:drawLine()
	self.testNode = rd.node.new():addTo(self)
	rd.label.TTF.new("Draw a black line at screen center",
		25, "Arial", {0, 0, 0xc6, 255}, rd.label.CENTER)
		:setPos(0, 100)
		:addTo(self.testNode)
	rd.drawVertex.new({
			{-80, 0, 0, 0, 0, 0, 255},
			{80, 0, 0, 0, 0, 0, 255},
		}, 2, 5)
		:addTo(self.testNode)
end

function TestDrawvertex:drawTriangle()
	self.testNode = rd.node.new():addTo(self)
	rd.label.TTF.new("Draw a triangle at screen center",
		25, "Arial", {0, 0, 0xc6, 255}, rd.label.CENTER)
		:setPos(0, 100)
		:addTo(self.testNode)
	rd.drawVertex.new({
			{-80, 0, 0, 0, 0, 0, 255},
			{80, 0, 0, 100, 0, 0, 255},
			{0, 60, 0, 0, 100, 0, 255},
		}, 4, 1)
		:addTo(self.testNode)
end

function TestDrawvertex:drawRect()
	self.testNode = rd.node.new():addTo(self)
	rd.label.TTF.new("Draw a rect at screen center",
		25, "Arial", {0, 0, 0xc6, 255}, rd.label.CENTER)
		:setPos(0, 100)
		:addTo(self.testNode)
	rd.drawRect.new(100, 100)
		:addTo(self.testNode)
		:setColor(10, 20, 30, 255, 1)
		:setColor(100, 20, 90, 255, 2)
		:setColor(200, 20, 30, 255, 3)
		:setColor(10, 150, 30, 255, 4)
end

return TestDrawvertex
