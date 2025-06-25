print("Check the logic [].....")

function Check_integrity(logic)
    if logic and logic.signature == "valid" then 
        return true 
    else 
        return false
    end
end

function Simulate_behavior(logic)
    print(
        "Simulating logic:", logic.name or "unnamed"
    )
    if logic.risk_score and logic.risk_score < 0.5 then 
        return true 
    else 
        return false 
    end 
end 

function Is_logic_safe(logic)
    local ok_intergrity = Check_integrity(logic)
    local ok_behavior = Simulate_behavior(logic)
    return ok_behavior and ok_intergrity
end 

