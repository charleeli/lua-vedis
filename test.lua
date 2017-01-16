lvedis = require 'lvedis'

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

local vedisdb = lvedis.open('test.vedis')


lvedis.begin(vedisdb)
lvedis.store(vedisdb, 'foo 12348 78', '456  bar 7')
lvedis.commit(vedisdb)

local result = lvedis.fetch(vedisdb, 'foo 12348 78')
print(result)


--[[
lvedis.begin(vedisdb)
lvedis.exec(vedisdb, "SET\r\n foo1 bar1\r\n")
lvedis.commit(vedisdb)

local result = lvedis.fetch(vedisdb, 'foo1')
print(result)
--]]
--[[
local result = lvedis.exec_result_string(vedisdb, "GET foo10")
if result == nil then
    print('nil')
else
    print(result)
end
--]]

--[[
lvedis.exec(vedisdb, "MSET username james age 27 mail dude@example.com")
local result = lvedis.exec_result_array(vedisdb, "MGET username age mail")
for i,v in ipairs(result) do
    print(v)
end

table.print(result)
--]]

--[[
vedis = require 'vedis'
vedis.open("vds.vedis")
local row = { id = 717066513, pid = "fd fadax", sdkid = nil }
vedis.hmset('my 1test', row)

table.print(vedis.halls('my 1test'))
--]]
--table.print(vedis.hgetall('my 1test'))

