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
local crypto = {}

-- if isRaw return is binary string, otherwise return hex string
function crypto.md5(input, isRaw)
	isRaw = isRaw or false
	return Rapid2D_CCrypto.md5(tostring(input), isRaw)
end

function crypto.encryptXXTEA(input, key)
	return Rapid2D_CCrypto.encryptXXTEA(tostring(input), tostring(key))
end

function crypto.decryptXXTEA(input, key)
	return Rapid2D_CCrypto.decryptXXTEA(tostring(input), tostring(key))
end

function crypto.encodeBase64(input)
	return Rapid2D_CCrypto.encodeBase64(tostring(input))
end

function crypto.decodeBase64(input)
	return Rapid2D_CCrypto.decodeBase64(tostring(input))
end

rd.crypto = crypto
