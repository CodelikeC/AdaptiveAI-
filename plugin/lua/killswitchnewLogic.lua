print("kill new Logic")

--function Activate_killswitch()
    -- Dung moi logic moi dang thuc hien 
--end 

--function Isolate_process()
    -- Co lap logic dong ngo khoi he thong 
--end 

--function Emergency_shutdown(reason)
    -- Tot hoac khoa he thong trong tinh huong khan cap
--end 
Trust_score = 1.0 
Current_logic_status = "active"
New_logic_status =  "pending "

function Activate_killswitch(target_logic)
    if target_logic == "new" then 
        New_logic_status = "disabled"
        print(
            "Killswitch: new logic has been disabled"
        )
        return true 
        elseif target_logic == "current" then
            Current_logic_status = "disabled"
            print("Lua - Killswitch : Current_logic_status has been replaced")
            return true
        else 
            print("error")
            return false 
        end 
end 

function Isolate_process()
    print("Isolate new logic into the sandbox")
    return "sandbox"
end

function Emergency_shutdown(reason)
    print("Lua - Emergency :"..reason)
    Current_logic_status = "shutdown"
    New_logic_status = "shutdown"
    os.exit(1)
end
