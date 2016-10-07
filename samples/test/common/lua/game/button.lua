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

rd.global.TestButton = {}

function TestButton.newColorButton(text, clicked)
	local btn = rd.drawRect.new(300, 80)
		:setColor(242, 150, 83, 255)
		:onTouch(function (target, name, points)
			-- single touch mode
			if name == "began" then
				target.children[1]:setColor(0, 0, 0, 255)
			end
			if name == "ended" then
				target.children[1]:setColor(255, 255, 255, 255)
				clicked(target)
			end
		end)
	local labelInfo = rd.label.TTF.new(text, 40, "Arial", {255, 255, 255, 255}, rd.label.CENTER)
		:addTo(btn)
	return btn
end

function TestButton.homeButton()
	local btnWidth = 100
	local btnHeight = 60
	local btn = rd.drawRect.new(btnWidth, btnHeight)
		:setColor(242, 150, 83, 255)
		:onTouch(function (target, name, points)
			-- single touch mode
			if name == "began" then
				target.children[1]:setColor(0, 0, 0, 255)
			end
			if name == "ended" then
				target.children[1]:setColor(255, 255, 255, 255)
				rd.director.runScene(require("game.mainscene").new())
			end
		end)
		:setPos(rd.screen.left + btnWidth / 2, rd.screen.top - btnHeight / 2)
	local labelInfo = rd.label.TTF.new("Home", 25, "Arial", {255, 255, 255, 255}, rd.label.CENTER)
		:addTo(btn)
	return btn
end

function TestButton.preButton(callback)
	local btn = rd.sprite.new("btnPrevious.png")
		:setPos(rd.screen.left + 50, rd.screen.bottom + 50)
		:onTouch(function (target, name, points)
			-- single touch mode
			if name == "began" then
				target:setColor(255, 255, 255, 150)
			end
			if name == "ended" then
				target:setColor(255, 255, 255, 255)
				callback(target)
			end
		end)
	return btn
end

function TestButton.nextButton(callback)
	local btn = rd.sprite.new("btnNext.png")
		:setPos(rd.screen.right - 50, rd.screen.bottom + 50)
		:onTouch(function (target, name, points)
			-- single touch mode
			if name == "began" then
				target:setColor(255, 255, 255, 150)
			end
			if name == "ended" then
				target:setColor(255, 255, 255, 255)
				callback(target)
			end
		end)
	return btn
end
