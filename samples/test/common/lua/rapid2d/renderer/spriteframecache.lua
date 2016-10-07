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
local spriteFrameCache = {}
spriteFrameCache.frames = {}

function spriteFrameCache.getFrame(name)
	-- find cached frame
	if spriteFrameCache.frames[name] then
		return spriteFrameCache.frames[name]
	end

	-- load texture from file
	local texture = rd.textureCache.getTexture(name)
	if nil == texture then
		print("[Error]: spriteFrameCache.getFrame() Fail for:", name)
		return nil
	end
	-- make texture into a frame
	local frame = Rapid2D_CSriteFrame.new(
				name,  -- frame name
				false, -- rotated
				false, -- trimmed
				0, 0, texture:getWidth(), texture:getHeight(), -- frame
				0, 0, texture:getWidth(), texture:getHeight(), -- source
				texture:getWidth(), texture:getHeight(), -- soruce size
				texture) -- texture

	-- this is IMPORTANT to make lua manager the lifecycle of texture
	spriteFrameCache.frames[name] = {
		cframe = frame,
		ctexture = texture,
		width = texture:getWidth(),
		height = texture:getHeight()
	}
	return spriteFrameCache.frames[name]
end

-- load TexturePacker json format
function spriteFrameCache.addFrames(json)
	local str = rd.fs.getData(rd.fs.DIR_RES, json, true)
	if nil == str then
		print("[Error]: spriteFrameCache.addFrames() Fail for:", json)
		return nil
	end

	local cfg = cjson.decode(str)
	if nil == cfg then
		print("[Error]: spriteFrameCache.addFrames() Fail on json decode")
		return nil
	end

	-- find path of json
	local startIndex = #json
	while startIndex > 0 do
		-- ascii 47 is '/'
		if string.byte(json, startIndex) == 47 then
			break
		end
		startIndex = startIndex - 1
	end
	-- add path to texture name
	local textureName = string.sub(json, 1, startIndex) .. cfg.meta.image
	local texture = rd.textureCache.getTexture(textureName)
	if nil == texture then
		print("[Error]: spriteFrameCache.getFrame() Fail for:", textureName)
		return nil
	end

	-- get frames
	for key, value in pairs(cfg.frames) do
		-- compatible two json format
		local frameName = value.filename or key
		-- add create frame and add to cache
		local frame = Rapid2D_CSriteFrame.new(
				frameName,  -- frame name
				value.rotated, -- rotated
				value.trimmed, -- trimmed
				value.frame.x, value.frame.y, value.frame.w, value.frame.h, -- frame
				value.spriteSourceSize.x, value.spriteSourceSize.y, -- source
				value.spriteSourceSize.w, value.spriteSourceSize.h, -- source
				value.sourceSize.w, value.sourceSize.h, -- soruce size
				texture) -- texture

		-- this is IMPORTANT to make lua manager the lifecycle of texture
		spriteFrameCache.frames[frameName] = {
			cframe = frame,
			ctexture = texture,
			width = value.sourceSize.w,
			height = value.sourceSize.h
		}
	end
end

rd.spriteFrameCache = spriteFrameCache
