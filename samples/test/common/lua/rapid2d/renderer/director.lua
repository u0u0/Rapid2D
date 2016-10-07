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

-- Singleton class
local director = {}

-- will call from C, every frame
function director.drawScene(dt)
	if director.curScene then
		director.curScene:drawTree(dt)
	end

	-- after first draw
	rd.actionManager.update(dt)
	-- audio affair
	rd.audio.update()
end

function director.sendTouchEvent(t, points)
	if director.curScene then
		director.curScene:dispatchTouchEvent(t, points)
	end
end

function director.sendKeyBoardEvent(t, code)
	if director.curScene then
		director.curScene:dispatchKeyBoardEvent(t, code)
	end
end

function director.runScene(scene)
	director.curScene = scene
	-- do this here?
	collectgarbage("collect")
end

rd.director = director
