print("Make Decision")

function Get_Context()
    -- trich xuat thong tin he thong hien tai ..// 
    local context = 
    {
        cpu = "Intel i7", 
        os = "Windows", 
        mem = "8GB", 
        threat_level = "medium"
    }
    print("Context captured :", context.os)
    print(
        "os capture:", context.os 
    )
    print("memory captured:", context.mem)
    print("threat_level captured:", context.threat_level)
    return context
end 

function Apply_logic(logic)
    -- ap dung ly thuyet moi neu duoc duyet .. 
    print("Applying logic")
    logic.applied = true 
    return logic
end

function Log_decision()
    -- GHi log/ hanh vi duoc chon va li do 
    print("Log [Decision] applied for logic:", logic.os, reason.context)
    print("Applied for logic:", logic.name or "unnamed")
    print("reason :", reason or "No reason provided")
end

