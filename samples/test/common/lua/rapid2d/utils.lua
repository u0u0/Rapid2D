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

-- export global variable
rd.global = {}
setmetatable(rd.global, {
	__newindex = function(_, name, value)
		rawset(_G, name, value)
	end,

	__index = function(_, name)
		return rawget(_G, name)
	end
})

-- IMPORTANT: disable create unexpected global variable
setmetatable(_G, {
	__newindex = function(_, name, value)
		local msg = "USE 'rd.global.%s = value' INSTEAD OF SET GLOBAL VARIABLE"
		error(string.format(msg, name), 0)
	end
})

-- ONLY for lua table and ENOUGH for Rapid2D
rd.global.class = function(classname, super)
	local cls

	if type(super) ~= "table" then
		super = nil
	end

	if super then
		cls = {}
		setmetatable(cls, {__index = super})
		cls.super = super
	else
		cls = {ctor = function() return self end}
	end

	cls.__cname = classname
	cls.__index = cls

	function cls.new(...)
		local instance = setmetatable({}, cls)
		instance.class = cls
		return instance:ctor(...)
	end

	return cls
end

rd.global.dump = function(value, desciption, nesting)
	if type(nesting) ~= "number" then nesting = 3 end

	local lookupTable = {}
	local result = {}

	local function _v(v)
		if type(v) == "string" then
			v = "\"" .. v .. "\""
		end
		return tostring(v)
	end

	local traceback = string.split(debug.traceback("", 2), "\n")
	print("dump from: " .. string.trim(traceback[3]))

	local function _dump(value, desciption, indent, nest, keylen)
		desciption = desciption or "<var>"
		local spc = ""
		if type(keylen) == "number" then
			spc = string.rep(" ", keylen - string.len(_v(desciption)))
		end
		if type(value) ~= "table" then
			result[#result +1 ] = string.format("%s%s%s = %s", indent, _v(desciption), spc, _v(value))
		elseif lookupTable[value] then
			result[#result +1 ] = string.format("%s%s%s = *REF*", indent, desciption, spc)
		else
			lookupTable[value] = true
			if nest > nesting then
				result[#result +1 ] = string.format("%s%s = *MAX NESTING*", indent, desciption)
			else
				result[#result +1 ] = string.format("%s%s = {", indent, _v(desciption))
				local indent2 = indent.."    "
				local keys = {}
				local keylen = 0
				local values = {}
				for k, v in pairs(value) do
					keys[#keys + 1] = k
					local vk = _v(k)
					local vkl = string.len(vk)
					if vkl > keylen then keylen = vkl end
					values[k] = v
				end
				table.sort(keys, function(a, b)
					if type(a) == "number" and type(b) == "number" then
						return a < b
					else
						return tostring(a) < tostring(b)
					end
				end)
				for i, k in ipairs(keys) do
					_dump(values[k], k, indent2, nest + 1, keylen)
				end
				result[#result +1] = string.format("%s}", indent)
			end
		end
	end
	_dump(value, desciption, "- ", 1)

	for i, line in ipairs(result) do
		print(line)
	end
end

rd.global.handler = function (obj, method)
    return function(...)
        return method(obj, ...)
    end
end
