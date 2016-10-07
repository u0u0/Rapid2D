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
local fs = {}
fs.DIR_RES = 0 -- place picture,font etc.
fs.DIR_LUA = 1 -- place lua source file.
fs.DIR_DATA = 2 -- place writeable data.

function fs.getData(where, name, isTextMode)
	isTextMode = isTextMode or true
	return Rapid2D_CFileSystem.getData(where, name, isTextMode)
end

-- wirte data ONLY on fs.DIR_DATA
function fs.putData(name, data)
	return Rapid2D_CFileSystem.putData(name, data)
end

-- data must table
-- key is use for xxtea encrypt
function fs.profileSave(fileName, data, key)
	if type(data) ~= "table" then
		print("Error, profileSave ONLY save table data")
		return false
	end
	fileName = tostring(fileName)
	key = tostring(key)
	data = cjson.encode(data)
	data = rd.crypto.encryptXXTEA(data, key)
	if nil == data then
		print("Error: profileSave Fail for encryptXXTEA")
		return false
	end

	if false == fs.putData(fileName, data) then
		print("Error: profileSave Fail " .. fileName)
		return false
	end

	return true
end

-- return table data
function fs.profileLoad(fileName, key)
	fileName = tostring(fileName)
	key = tostring(key)
	local data = fs.getData(fs.DIR_DATA, fileName, false)
	if data then
		data = rd.crypto.decryptXXTEA(data, key)
	end
	if data then
		return cjson.decode(data)
	end
end

rd.fs = fs
