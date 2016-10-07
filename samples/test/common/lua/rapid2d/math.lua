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

function math.angle2radian(angle)
	return angle * math.pi / 180
end

function math.radian2angle(radian)
	return radian / math.pi * 180
end

function math.rectContainsPoint(rect, point)
    local ret = false

    if (point.x >= rect.x) and (point.x <= rect.x + rect.width) and
       (point.y >= rect.y) and (point.y <= rect.y + rect.height) then
        ret = true
    end

    return ret
end

-- cubic Bezier formula t{0, 1}
function math.cubicBezier(p1, c1, c2, p2, t)
	return (1 - t)^3 * p1 + 
            3 * (1 - t)^2 * t * c1 + 
            3 * (1 - t) * t^2 * c2 +
            t^3 * p2
end
