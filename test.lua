vedis = require 'vedis'

--打印table
table.print = function(T, CR)
    assert(type(T) == "table",'arg should be a table!')

    CR = CR or '\r\n'
    local cache = {  [T] = "." }
    local function _dump(t,space,name)
        local temp = {}
        for k,v in next,t do
            local key = tostring(k)
            if cache[v] then
                table.insert(temp,"+" .. key .. " {" .. cache[v].."}")
            elseif type(v) == "table" then
                local new_key = name .. "." .. key
                cache[v] = new_key
                table.insert(temp,
                    "+" .. key .. _dump(
                    v,
                    space .. (next(t,k) and "|" or " " ).. string.rep(" ",#key),
                    new_key
                ))
            else
                table.insert(temp,"+" .. key .. " [" .. tostring(v).."]")
            end
        end
        return table.concat(temp,CR..space)
    end
    print(_dump(T, "",""))
end

local vedisdb = vedis.open('test.vedis')

--[[
vedis.begin(vedisdb)
vedis.store(vedisdb, 'foo', 'bar')
vedis.commit(vedisdb)

local result = vedis.fetch(vedisdb, 'foo')
print(result)
--]]

--[[
vedis.begin(vedisdb)
vedis.exec(vedisdb, "SET foo1 bar1")
vedis.commit(vedisdb)

local result = vedis.fetch(vedisdb, 'foo1')
print(result)
--]]

--[[
local result = vedis.exec_result_string(vedisdb, "GET foo10")
if result == nil then
    print('nil')
else
    print(result)
end
--]]

vedis.exec(vedisdb, "MSET username james age 27 mail dude@example.com")
local result = vedis.exec_result_array(vedisdb, "MGET username age mail")
for i,v in ipairs(result) do
    print(v)
end

table.print(result)
