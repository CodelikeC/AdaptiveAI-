print("update_new_lua")

function Load_new_logic(path)
print("Loading new logic", path)
-- simualate logic has been loaded 
local logic 
=
{
    name ="New Logic",
    version = "1.0", 
    signature = "valid", 
    risk_score = 0.0
}
return logic
end 

function Validate_metadata(logic)
    -- xac minh metadata xem co bi gia mao khong? 
    if logic and logic.version then 
        return true 
    else 
        return false
    end 
end 

function StoreLogic(logic)
    print("Name:", logic.name)
    print("Version:", logic.version)

    print("risk_score", logic.risk_score)
    print ("signature :", logic.signature)
    
    return true 
end 



