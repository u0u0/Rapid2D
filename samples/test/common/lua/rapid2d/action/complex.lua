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


------------------- sequence -----------------
local actionSequence = class("rd.actionSequence", rd.action)

function actionSequence:ctor(...)
	self.children = {}

	local duration = 0.0
	for i,v in ipairs({...}) do
		duration = duration + v.duration
		table.insert(self.children, v)
	end
	-- call super
	actionSequence.super.ctor(self, duration)
	return self
end

function actionSequence:start(target)
	-- call super first
	actionSequence.super.start(self, target)
	self.sequenceIndex = 1
	self.children[1]:start(target)
end

function actionSequence:update(dt)
	local curAction = self.children[self.sequenceIndex]
	curAction:update(dt)
	-- check end and updateIndex
	if nil == curAction.target then
		self.sequenceIndex = self.sequenceIndex + 1
		if self.sequenceIndex > #self.children then
			-- sequence end
			self.target = nil
		else
			-- start next action
			self.children[self.sequenceIndex]:start(self.target)
		end
	end
end

rd.actionSequence = actionSequence

------------------- concurrency -----------------
local actionConcurrency = class("rd.actionConcurrency", rd.action)

function actionConcurrency:ctor(...)
	self.children = {}

	local duration = 0.0
	for i,v in ipairs({...}) do
		duration = math.max(duration, v.duration)
		table.insert(self.children, v)
	end
	-- call super
	actionConcurrency.super.ctor(self, duration)
	return self
end

function actionConcurrency:start(target)
	-- call super first
	actionConcurrency.super.start(self, target)

	-- start all children
	for i,action in ipairs(self.children) do
		action:start(target)
	end
	self.runningCount = #self.children
end

function actionConcurrency:update(dt)
	for i,action in ipairs(self.children) do
		if action.target then
			action.update(action, dt)
			-- check end
			if nil == action.target then
				self.runningCount = self.runningCount - 1
				if 0 == self.runningCount then
					-- Concurrency end
					self.target = nil
				end
			end
		end
	end
end

rd.actionConcurrency = actionConcurrency

------------------- repeat -----------------
local actionRepeat = class("rd.actionRepeat", rd.action)

-- if times < 1, repeatForever
function actionRepeat:ctor(action, times)
	self.child = action
	self.times = times
	local duration = action.duration * times
	if times <= 0 then
		-- repeatForever
		duration = math.maxinteger
	end
	-- call super
	actionRepeat.super.ctor(self, duration)
	return self
end

function actionRepeat:start(target)
	-- call super first
	actionRepeat.super.start(self, target)
	self.child:start(target)
	self.runTimes = 0
end

function actionRepeat:update(dt)
	self.child:update(dt)
	-- check end
	if nil == self.child.target then
		self.runTimes = self.runTimes + 1
		if self.runTimes == self.times and self.times > 0 then
			-- repeat end
			self.target = nil
		else
			-- onther run
			self.child:start(self.target)
		end
	end
end

rd.actionRepeat = actionRepeat
