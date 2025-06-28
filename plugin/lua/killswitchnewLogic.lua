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
-- local Killswitch = {}

-- -- cấu trúc trạng thái logic -- 
-- local current_logic = {name = "default_logic", status = "active"}
-- local new_logic = nil 
-- local is_isolated = false 
-- local trust_score = 1.0 

-- -- 1. Kich hoat killswitch cho logic moi hoặc logic cũ..// 
-- function Killswitch.Activate_killswitch(target_logic)
--     if not target_logic or (target_logic ~= "new" and target_logic ~= "current") then 
--         print("killswitch, failed")
--         return false 
--     end 
--     if target_logic == "new" and new_logic then 
--         print("[killswitch] Disabled new logic")
--         new_logic.status = "Disabled"
--     elseif target_logic == "current" then 
--         print(
--             "[killswitch] Cannot find the logic"
--         )
--         return false 
--     end 
--     print("[killswitch] - Activate_killswitch")
--     return true 
-- end 

-- -- 2. Co lap logic de phan vung an toan va minh bach.. 
-- function Killswitch.Isolate_process()
--     if not new_logic then 
--         print("Isolation - no new logic")
--         return false 
--     end 
--     is_isolated = true 
--     new_logic.zone = "sandbox"
--     current_logic.zone  = "Core"
-- end 

-- -- 3. Dung toan bo he thong mot cach khan cap .. // 
-- function Killswitch.Emergency_shutdown(reason)
--     reason = reason or "none"

--     if trust_score < 0.4 then 
--         print(
--             "trust_score : Emergency_shutdown"..trust_score
--         )
--     else
--         print("Emergency_shutdown"..reason)
--     end 
--     -- dat lai trang thai he thong ..
--     current_logic.status = "shutdown"
--     if new_logic then 
--         new_logic.status = "shutdown"
--     end 

--     print(
--         "Emergency : The system is disabled"
--     )
--     os.exit(1)
    
-- end

-- -- create  a new logic ..
-- function Killswitch.Load_new_logic(name)
--     new_logic  = {name  = name or "unamed logic"}
--     print("[Loader] new logic is been fullfilled")
-- end 

-- -- update the trust score 
-- function Killswitch.Set_trust_score(score)
--     trust_score  = score 
--     print("[Trust monitor] - Update trust score:".. trust_score)
-- end 
-- return Killswitch

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
