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
local TestAudioScene = class("TestAudioScene", testBase)

function TestAudioScene:ctor()
	-- init case first
	self.testcase = {"audioTest"}
	-- then call super.ctor
	TestAudioScene.super.ctor(self)

	-- create success
	return self
end

function TestAudioScene:audioTest()
	local bgmcase = {
		"LoadFile",
		"playBGM",
		"playEffect",
		"pauseAll",
		"resumeAll",
		"stopAll",
		"setBGMVolume",
		"setEffectVolume",
	}

	for i,v in ipairs(bgmcase) do
		local btn = TestButton.newColorButton(v, function(target)
			self[target.v](self)
		end)
		btn:setPos(0, rd.screen.top - 50 - 100 * (i - 1))
		btn:addTo(self)
		btn.v = v
	end
end

function TestAudioScene:LoadFile()
	rd.audio.loadFile("audio/bgm.ogg")
	rd.audio.loadFile("audio/effect.ogg")
end

function TestAudioScene:playBGM()
	rd.audio.playBGM("audio/bgm.ogg")
end

function TestAudioScene:playEffect()
	rd.audio.playEffect("audio/effect.ogg")
end

function TestAudioScene:pauseAll()
	rd.audio.pause()
end

function TestAudioScene:resumeAll()
	rd.audio.resume()
end

function TestAudioScene:stopAll()
	rd.audio.stop()
end

function TestAudioScene:setBGMVolume()
	rd.audio.setBGMVolume(0.5)
end

function TestAudioScene:setEffectVolume()
	rd.audio.setEffectVolume(0.5)
end

return TestAudioScene
