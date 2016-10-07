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

-------------- base class for action -----------
local action = class("rd.action")

function action:ctor(duration)
	self.duration = duration
	return self
end

function action:start(target)
	self.target = target
	self.elapsed = 0
end

function action:update(dt)
	self.elapsed = self.elapsed + dt
	local scale = self.elapsed / self.duration
	local floor = math.floor(scale)
	if floor >= 1 then
		scale = 1
	end

	self:step(scale)

	-- action ended
	if floor >= 1 then
		self.target = nil
	end
end

-- override me, scale from 0.0 ~ 1.0
function action:step(scale)
end

rd.action = action
rd.actionDelay = action

-------------- Action Manager ------------------
-- Singleton class
local actionManager = {}
actionManager.rootActions = {}

function actionManager.getRootActionCounts(target)
	local count = 0
	for k,v in ipairs(actionManager.rootActions) do
		if v.target == target then
			count = count + 1
		end
	end
	return count
end

function actionManager.addRootAction(action, target)
	action.markDelete = false
	-- add to tail, run on next frame
	table.insert(actionManager.rootActions, action)
	action:start(target)
end

-- this is for node remove, just mark delete, then delete it in update
function actionManager.removeActions(target)
	for k,v in ipairs(actionManager.rootActions) do
		if v.target == target then
			v.markDelete = true
		end
	end
end

-- remove a root action
function actionManager.remove(action)
	for k,v in ipairs(actionManager.rootActions) do
		if v == action then
			v.markDelete = true
		end
	end
end

function actionManager.update(dt)
	local startCount = #actionManager.rootActions
	local i = 1
	while true do
		if i > startCount then
			break
		end
		-- update action
		if actionManager.rootActions[i].markDelete then
			actionManager.rootActions[i].target = nil
		else
			-- update may change table, like callback action.
			actionManager.rootActions[i]:update(dt)
		end

		-- safe remove action
		if nil == actionManager.rootActions[i].target then
			table.remove(actionManager.rootActions, i)
			startCount = startCount - 1
		else
			i = i + 1
		end
	end
end

rd.actionManager = actionManager
