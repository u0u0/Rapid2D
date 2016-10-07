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

local function bounceScale(scale)
    if (scale < 1 / 2.75) then
        return 7.5625 * scale * scale
	elseif (scale < 2 / 2.75) then
        scale = scale - 1.5 / 2.75
        return 7.5625 * scale * scale + 0.75
    elseif (scale < 2.5 / 2.75) then
        scale = scale - 2.25 / 2.75
        return 7.5625 * scale * scale + 0.9375
	end

    scale = scale - 2.625 / 2.75
    return 7.5625 * scale * scale + 0.984375
end

-- get Ease deal method
local switch = {
	easeIn = function(scale, rate)
		return scale^rate
	end,
	easeOut = function(scale, rate)
		return scale^(1 / rate)
	end,
	easeInOut = function(scale, rate)
		scale = scale * 2
		if (scale < 1) then
			return 0.5 * scale^rate
		else
			return 1 - 0.5 * (2 - scale)^rate
		end
	end,
	quadraticIn = function(scale)
		return scale^2
	end,
	quadraticOut = function(scale)
		return - scale * (scale - 2)
	end,
	quadraticInOut = function(scale)
		local resultScale = scale
		scale = scale * 2
		if (scale < 1) then
			resultScale = scale * scale * 0.5
		else
			scale = scale - 1
			resultScale = - 0.5 * (scale * (scale - 2) - 1)
		end
		return resultScale
	end,
	bounceEaseIn = function(scale)
		return 1 - bounceScale(1 - scale)
	end,
	bounceEaseOut = function(scale)
		return bounceScale(scale)
	end,
	bounceEaseInOut = function(scale)
		local newT = 0
		if (scale < 0.5) then
			scale = scale * 2
			newT = (1 - bounceScale(1 - scale)) * 0.5
		else
			newT = bounceScale(scale * 2 - 1) * 0.5 + 0.5
		end

		return newT
	end,
	backEaseIn = function(scale)
		local overshoot = 1.70158
		return scale * scale * ((overshoot + 1) * scale - overshoot)
	end,
	backEaseOut = function(scale)
		local overshoot = 1.70158
		scale = scale - 1
		return scale * scale * ((overshoot + 1) * scale + overshoot) + 1
	end,
	backEaseInOut = function(scale)
		local overshoot = 1.70158 * 1.525
		scale = scale * 2
		if (scale < 1) then
			return (scale * scale * ((overshoot + 1) * scale - overshoot)) / 2
		else
			scale = scale - 2
			return (scale * scale * ((overshoot + 1) * scale + overshoot)) / 2 + 1
		end
	end,
	elasticEaseIn = function(scale, period)
		local newT = 0
		if (scale == 0) or (scale == 1) then
			newT = scale
		else
			local s = period / 4
			scale = scale - 1
			newT = - 2^(10 * scale) * math.sin((scale - s) * 2.0 * math.pi / period)
		end

		return newT
	end,
	elasticEaseOut = function(scale, period)
		local newT = 0
		if (scale == 0) or (scale == 1) then
			newT = scale
		else
			local s = period / 4
			newT = 2^(-10 * scale) * math.sin((scale - s) * 2.0 * math.pi / period) + 1
		end

		return newT
	end,
	elasticEaseInOut = function(scale, period)
		local newT = 0
		if (scale == 0) or (scale == 1) then
			newT = scale
		else
			scale = scale * 2
			if 0 == period then
				period = 0.3 * 1.5
			end

			local s = period / 4
			scale = scale - 1
			if (scale < 0) then
				newT = -0.5 * 2^(10 * scale) * math.sin((scale - s) * 2.0 * math.pi / period)
			else
				newT = 2^(-10 * scale) * math.sin((scale - s) * 2.0 * math.pi / period) * 0.5 + 1
			end
		end
		return newT
	end,
	circEaseIn = function(scale)
		return - 1 * (math.sqrt(1 - scale * scale) - 1)
	end,
	circEaseOut = function(scale)
		scale = scale - 1
		return math.sqrt(1 - scale * scale)
	end,
	circEaseInOut = function(scale)
		scale = scale * 2
		if (scale < 1) then
			return -0.5 * (math.sqrt(1 - scale * scale) - 1)
		end
		scale = scale - 2
		return 0.5 * (math.sqrt(1 - scale * scale) + 1)
	end,
	expoEaseIn = function(scale)
		if 0 == scale then
			return 0
		end
		return 2^(10 * (scale / 1 - 1)) - 1 * 0.001
	end,
	expoEaseOut = function(scale)
		if 1 == scale then
			return 1
		end
		return -2^(-10 * scale / 1) + 1
	end,
	expoEaseInOut = function(scale)
		scale = scale / 0.5
		if (scale < 1) then
			scale = 0.5 * 2^(10 * (scale - 1))
		else
			scale = 0.5 * (-2^(-10 * (scale - 1)) + 2)
		end

		return scale
	end,
	quintEaseIn = function(scale)
		return scale^5
	end,
	quintEaseOut = function(scale)
		scale = scale - 1
		return scale^5 + 1
	end,
	quintEaseInOut = function(scale)
		scale = scale * 2
		if (scale < 1) then
			return 0.5 * scale^5
		end
		scale = scale - 2
		return 0.5 * (scale^5 + 2)
	end,
	quartEaseIn = function(scale)
		return scale^4
	end,
	quartEaseOut = function(scale)
		scale = scale - 1
		return -(scale^4 - 1)
	end,
	quartEaseInOut = function(scale)
		scale = scale * 2
		if (scale < 1) then
			return 0.5 * scale^4
		end
		scale = scale - 2
		return -0.5 * (scale^4 - 2)
	end,
	cubicEaseIn = function(scale)
		return scale^3
	end,
	cubicEaseOut = function(scale)
		scale = scale - 1
		return (scale^3 + 1)
	end,
	cubicEaseInOut = function(scale)
		scale = scale * 2
		if (scale < 1) then
			return 0.5 * scale^3
		end
		scale = scale - 2
		return 0.5 * (scale^3 + 2)
	end,
	quadEaseIn = function(scale)
		return scale^2
	end,
	quadEaseOut = function(scale)
		return -1 * scale * (scale - 2)
	end,
	quadEaseInOut = function(scale)
		scale = scale * 2
		if (scale < 1) then
			return 0.5 * scale^2
		end
		scale = scale - 1
		return -0.5 * (scale * (scale - 2) - 1)
	end,
	sineEaseIn = function(scale)
		return -1 * math.cos(scale * math.pi / 2) + 1
	end,
	sineEaseOut = function(scale)
		return math.sin(scale * math.pi / 2)
	end,
	sineEaseInOut = function(scale)
		return -0.5 * (math.cos(math.pi * scale) - 1)
	end,
}

----------- work for simple action ---------------
local actionEase = class("rd.actionEase", rd.action)

function actionEase:ctor(action, mode, extra)
	self.easeFunction = switch[mode]
	if nil == self.easeFunction then
		return nil
	end
	-- call super first
	actionEase.super.ctor(self, action.duration)
	self.child = action
	self.extra = extra
	return self
end

function actionEase:start(target)
	-- call super first
	actionEase.super.start(self, target)
	self.child:start(target)
end

function actionEase:step(scale)
	scale = self.easeFunction(scale, self.extra)
	self.child:step(scale)
end

rd.actionEase = actionEase
