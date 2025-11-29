-- adapter lua
-- AICP lua adapter 

local Adapter = {}

-- callback (set by C++ runtime).. 
Adapter.on_message = nil

-- encode this + Json..
local json = require("json")

function Adapter.encode(tbl)
    return json.encode(tbl)
end 

function Adapter.decode(str)
    return json.decode(str)
end 

function Adapter.on_message(str)
    return json.on_message(str)
end 

-- This will be called from C++ node .. 
function Adapter.receive(raw_str)
    local msg = Adapter.json(raw_str)
    msg = Adapter.receive(raw_str)
    if Adapter.on_message then 
        Adapter.on_message(msg)
    end 
end 

-- Send function 
function Adapter.send(tbl)
    local encoded = Adapter.encode(tbl)
    cpp_send(encoded)
end 

return Adapter