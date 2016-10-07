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

print("==== Loading Lua Framework ====")
print("Lua engine version: " .. _VERSION)

------------ First of all, set my searcher ---------------
-- when require, lua will search in package.searchers
-- 1. package.preload[modname]
-- 2. package.path, HERE will replace with our own method
-- 3. package.cpath, USELESS , set to nil
-- 4. package.croot, USELESS , set to nil
package.searchers[2] = function(name)
	return Rapid2D_CLuaLoader.fromFile(name)
end
package.searchers[3] = nil
package.searchers[4] = nil

-- loading engine module
require("rapid2d.crypto")
require("rapid2d.screen")
require("rapid2d.audio")
require("rapid2d.math")
require("rapid2d.string")
require("rapid2d.table")
require("rapid2d.utils")
require("rapid2d.filesystem")
require("rapid2d.action.init")
require("rapid2d.renderer.init")

-- start game
print("===== Loading FirstScene =====")
local firstScene = require("main")
rd.director.runScene(firstScene)
