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

local TestBase = class("TestBase", rd.scene)

function TestBase:ctor()
	-- call super.ctor first
	TestBase.super.ctor(self)

	self:setBackgroundColor(250, 248, 239, 255)
	-- init scene objects
	TestButton.homeButton():addTo(self)
	TestButton.preButton(handler(self, TestBase.previousDeal)):addTo(self)
	TestButton.nextButton(handler(self, TestBase.nextDeal)):addTo(self)

	-- show first case
	if #self.testcase > 0 then
		self.caseIndex = 1
		self[self.testcase[self.caseIndex]](self)
	end

	-- create success
	return self
end

function TestBase:previousDeal(target)
	if self.caseIndex > 1 then
		if self.testNode then self.testNode:removeSelf() end
		self.caseIndex = self.caseIndex - 1
		self[self.testcase[self.caseIndex]](self)
	end
end

function TestBase:nextDeal(target)
	if self.caseIndex < #self.testcase then
		if self.testNode then self.testNode:removeSelf() end
		self.caseIndex = self.caseIndex + 1
		self[self.testcase[self.caseIndex]](self)
	end
end

return TestBase
