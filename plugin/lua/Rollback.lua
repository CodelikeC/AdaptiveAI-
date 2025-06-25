print("Rollback")

function Get_last_safe_logic()
    -- Truy hoi phien ban logic an toan cuoi cung.. //
    local safe_logic = "safe_logic_v1.2"
    print("Retrieved last safe logic:", safe_logic) 
    return safe_logic
end 

function Revert_to_lastLogic()
    -- quay ve logic truoc neu Logic hien tai tiem an rui ro
    local last_logic = Get_last_safe_logic()
    print("Reverting from".. current.."to"..last_logic)
    return last_logic
end

function Notify_again()
    -- Gui canh bao  cho nguoi dung/admin hoac ghi log..
    print("Logic Reverted.Reason:", reason or "unknown risk" )
end
