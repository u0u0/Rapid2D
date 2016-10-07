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

require("game.button")

local MainScene = class("MainScene", rd.scene)

function MainScene:ctor()
	-- call super.ctor first
	MainScene.super.ctor(self)

	self:setBackgroundColor(250, 248, 239, 255)
	-- init scene objects
	local testcase = {
		"FileSystem",
		"Crypto",
		"Drawvertex",
		"Actions",
		"Audio",
	}

	for i,v in ipairs(testcase) do
		local btn = TestButton.newColorButton(v, function(target)
			local scene = require("game.case.test" .. v).new()
			rd.director.runScene(scene)
		end)
		btn:setPos(0, rd.screen.top - 50 - 100 * (i - 1))
		btn:addTo(self)
		btn.v = v
	end

	-- create success
	return self
end

return MainScene
