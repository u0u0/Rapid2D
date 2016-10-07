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

local label = {}
label.LEFT = 0  -- text Alignment left
label.CENTER = 1 -- text Alignment center
label.RIGHT = 2 -- text Alignment right

local TTF = class("rd.label.TTF", rd.sprite)

function TTF:ctor(text, fontSize, font, fontColor, align)
	if nil == text then
		return nil
	end
	-- call super.ctor first
	TTF.super.ctor(self)

	self.font = font or "Arial"
	self.fontSize = fontSize or 18
	self.fontColor = fontColor or {255, 255, 255, 255} -- white color
	self.align = align or label.LEFT

	self:_genFrame(text)
	self.csprite = Rapid2D_CSprite.new(self.frame.cframe)

	return self
end

-- internal use func
function TTF:_genFrame(text)
	-- NEVER cache TTF texture and frame
	local texture = Rapid2D_CTexture.newTTF(text, self.font, self.fontSize,
				self.fontColor[1], self.fontColor[2], self.fontColor[3], self.fontColor[4],
				self.align)
	local frame = Rapid2D_CSriteFrame.new(
				"rd.label.TTF",  -- frame name
				false, -- rotated
				false, -- trimmed
				0, 0, texture:getWidth(), texture:getHeight(), -- frame
				0, 0, texture:getWidth(), texture:getHeight(), -- source
				texture:getWidth(), texture:getHeight(), -- soruce size
				texture) -- texture
	local luaFrame = {
		cframe = frame,
		ctexture = texture,
		width = texture:getWidth(),
		height = texture:getHeight()
	}
	self.frame = luaFrame
end

function TTF:setText(text)
	self:_genFrame(text)
	self.csprite:setFrame(self.frame.cframe)
end

label.TTF = TTF
rd.label = label
