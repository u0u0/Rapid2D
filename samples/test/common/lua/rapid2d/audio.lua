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
local audio = {}
audio._buffers = {}
audio._sources = {}
-- pos 1 is for BGM
audio._sources[1] = Rapid2D_CAudio.newSource()
audio._BGMVolume = 1.0
audio._effectVolume = 1.0

--------------- buffer -------------------
function audio.loadFile(path)
	if not audio._buffers[path] then
		audio._buffers[path] = Rapid2D_CAudio.newBuffer(path)
	end
	return audio._buffers[path]
end

function audio.unloadFile(path)
	audio._buffers[path] = nil
	collectgarbage("collect")
end

function audio.unloadAllFile()
	audio._buffers = {}
	collectgarbage("collect")
end

--[[
function for CSource
	play2d(buffer, isLoop)
    pause()
    resume()
    stop()
	setVolume(vol)
    getStat()
]]--

--------------- BGM 2D API -------------------
function audio.playBGM(path, isLoop)
	if not audio._buffers[path] then
		print(path .. " have not loaded!!")
		return
	end

	isLoop = isLoop or true
	audio._sources[1]:play2d(audio._buffers[path], isLoop)
	audio._sources[1]:setVolume(audio._BGMVolume)
end

function audio.setBGMVolume(vol)
	if vol > 1.0 then
		vol = 1.0
	end
	if vol < 0.0 then
		vol = 0.0
	end
	audio._sources[1]:setVolume(vol)
	audio._BGMVolume = vol
end

--------------- Effect 2D API -------------------
function audio.playEffect(path, isLoop)
	if not audio._buffers[path] then
		print(path .. " have not loaded!!")
		return
	end
	
	local source = Rapid2D_CAudio.newSource()
	if source then
		isLoop = isLoop or false
		table.insert(audio._sources, source)
		source:setVolume(audio._effectVolume)
		source:play2d(audio._buffers[path], isLoop)
	end
end

function audio.setEffectVolume(vol)
	if vol > 1.0 then
		vol = 1.0
	end
	if vol < 0.0 then
		vol = 0.0
	end
	audio._effectVolume = vol

	for i = 2, #audio._sources do
		audio._sources[i]:setVolume(vol)
	end
end

--------------- effect both on BGM and Effects -------------------
function audio.stop()
	for i = 1, #audio._sources do
		audio._sources[i]:stop()
	end
end

function audio.pause()
	for i = 1, #audio._sources do
		audio._sources[i]:pause()
	end
end

function audio.resume()
	for i = 1, #audio._sources do
		audio._sources[i]:resume()
	end
end

-- INTERNAL API, recircle source from effects, call by director
function audio.update()
	local isRemoved = false
	local total = #audio._sources
	local index = 2
	while index <= total do
		local stat = audio._sources[index]:getStat()
		if 4 == stat then
			table.remove(audio._sources, index)
			total = total - 1
			isRemoved = true
		else
			index = index + 1
		end
	end

	if isRemoved then
		collectgarbage("collect")
	end
end

rd.audio = audio
