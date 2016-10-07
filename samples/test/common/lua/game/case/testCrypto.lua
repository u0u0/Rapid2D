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
local TestCrypto = class("TestCrypto", testBase)

function TestCrypto:ctor()
	-- init case first
	self.testcase = {
		"md5",
		"encryptXXTEA",
		"decryptXXTEA",
	}
	-- then call super.ctor
	TestCrypto.super.ctor(self)

	-- create success
	return self
end

function TestCrypto:md5()
	self.testNode = rd.node.new():addTo(self)
	rd.label.TTF.new("MD5 hash 'Hello, Rapid2D'\nCheck the hex result below",
		25, "Arial", {0, 0, 0xc6, 255}, rd.label.CENTER)
		:setPos(0, 100)
		:addTo(self.testNode)
	
	local rtn = rd.crypto.md5("Hello, Rapid2D")
	rd.label.TTF.new(rtn,
		25, "Arial", {0, 0, 0xc6, 255}, rd.label.CENTER)
		:setPos(0, -100)
		:addTo(self.testNode)
end

function TestCrypto:encryptXXTEA()
	self.testNode = rd.node.new():addTo(self)
	rd.label.TTF.new([==[encryptXXTEA 'Hello, Rapid2D' with key 'this is a key'
		Then encodeBase64 the buffer,
		Compare base64 string to check result]==],
		25, "Arial", {0, 0, 0xc6, 255}, rd.label.CENTER)
		:setPos(0, 100)
		:addTo(self.testNode)
	
	local rtn = rd.crypto.encryptXXTEA("Hello, Rapid2D", "this is a key")
	local display = "encryptXXTEA fail"
	if rd.crypto.encodeBase64(rtn) == "A+k18p7M6jnloA9jgjI617tIpdw=" then
		display = "encryptXXTEA Success"
	end
	rd.label.TTF.new(display,
		25, "Arial", {0, 0, 0xc6, 255}, rd.label.CENTER)
		:setPos(0, -100)
		:addTo(self.testNode)
end

function TestCrypto:decryptXXTEA()
	self.testNode = rd.node.new():addTo(self)
	rd.label.TTF.new([==[decodeBase64 'A+k18p7M6jnloA9jgjI617tIpdw='
		Then send then result to decryptXXTEA
		with key 'this is a key'
		Result string must be 'Hello, Rapid2D']==],
		25, "Arial", {0, 0, 0xc6, 255}, rd.label.CENTER)
		:setPos(0, 100)
		:addTo(self.testNode)
	
	local rtn = rd.crypto.decodeBase64("A+k18p7M6jnloA9jgjI617tIpdw=")
	rtn = rd.crypto.decryptXXTEA(rtn, "this is a key")
	rd.label.TTF.new(rtn,
		25, "Arial", {0, 0, 0xc6, 255}, rd.label.CENTER)
		:setPos(0, -100)
		:addTo(self.testNode)
end

return TestCrypto
