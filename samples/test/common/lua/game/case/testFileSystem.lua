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
local TestFileSystem = class("TestFileSystem", testBase)

function TestFileSystem:ctor()
	-- init case first
	self.testcase = {
		"putData",
		"getData",
		"profile",
	}
	-- then call super.ctor
	TestFileSystem.super.ctor(self)

	-- create success
	return self
end

function TestFileSystem:putData()
	self.testNode = rd.node.new():addTo(self)
	rd.label.TTF.new("Write 'Hello, Rapid2D' to dataPath, fileName:fstest.dat",
		25, "Arial", {0, 0, 0xc6, 255}, rd.label.CENTER)
		:setPos(0, 100)
		:addTo(self.testNode)
	
	-- start write
	local string = "Hello, Rapid2D"
	local rtn = rd.fs.putData("fstest.dat", string)
	
	-- display result
	local result
	if rtn then
		result = "putData Success"
	else
		result = "putData Fail"
	end
	rd.label.TTF.new(result,
		25, "Arial", {0, 0, 0xc6, 255}, rd.label.CENTER)
		:setPos(0, -100)
		:addTo(self.testNode)
end

function TestFileSystem:getData()
	self.testNode = rd.node.new():addTo(self)
	rd.label.TTF.new("Get fileName:test.dat from dataPath",
		25, "Arial", {0, 0, 0xc6, 255}, rd.label.CENTER)
		:setPos(0, 100)
		:addTo(self.testNode)
	
	local string = rd.fs.getData(rd.fs.DIR_DATA, "fstest.dat", false)
	rd.label.TTF.new(string,
		25, "Arial", {0, 0, 0xc6, 255}, rd.label.CENTER)
		:setPos(0, -100)
		:addTo(self.testNode)
end

function TestFileSystem:profile()
	self.testNode = rd.node.new():addTo(self)
	rd.label.TTF.new("Save & Load game data from profile:'gamedata.dat'",
		25, "Arial", {0, 0, 0xc6, 255}, rd.label.CENTER)
		:setPos(0, 100)
		:addTo(self.testNode)

	local saveData = {
		string = "world",
		number = 123.5,
	}
	local rtn = rd.fs.profileSave("gamedata.dat", saveData, "encodeKey")	
	if rtn == false then
		rd.label.TTF.new("Test Fail for profileSave",
			25, "Arial", {0, 0, 0xc6, 255}, rd.label.CENTER)
			:setPos(0, -100)
			:addTo(self.testNode)
		return
	end

	local loadData = rd.fs.profileLoad("gamedata.dat", "encodeKey")
	if loadData and loadData.string == "world" then
		rd.label.TTF.new("profileSave & profileLoad test Success",
			25, "Arial", {0, 0, 0xc6, 255}, rd.label.CENTER)
			:setPos(0, -100)
			:addTo(self.testNode)
	end
end

return TestFileSystem
