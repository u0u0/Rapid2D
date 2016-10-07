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

local screen = {}

screen.width = rd.const.resolutionWidth
screen.height = rd.const.resolutionHeight
screen.left = -rd.const.resolutionWidth / 2
screen.right = rd.const.resolutionWidth / 2
screen.top = rd.const.resolutionHeight / 2
screen.bottom = -rd.const.resolutionHeight / 2
screen.cx = 0
screen.cy = 0

rd.screen = screen
