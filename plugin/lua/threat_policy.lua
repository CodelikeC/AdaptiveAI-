-- threat_policy.lua
print("[Lua] Loading threat classification policy...")

-- Mức mặc định cho các loại đe dọa
local thresholds = {
    CRITICAL = 0.85,
    HIGH = 0.65,
    MEDIUM = 0.4,
    LOW = 0.2
}

-- Phân loại dựa trên điểm + ngữ cảnh
function Classify_threat(threat_score, context)
    local hour = tonumber(context["hour"]) or 12
    local source = context["source"] or "unknown"

    -- Ưu tiên CRITICAL nếu là kernel source và vượt ngưỡng HIGH
    if source == "kernel_monitor" and threat_score >= thresholds.HIGH then
        return "CRITICAL"
    end

    -- Cảnh báo mạnh hơn vào ban đêm
    if hour >= 0 and hour < 6 and threat_score >= thresholds.HIGH then
        return "CRITICAL"
    end

    -- Phân loại thông thường
    if threat_score >= thresholds.CRITICAL then
        return "CRITICAL"
    elseif threat_score >= thresholds.HIGH then
        return "HIGH"
    elseif threat_score >= thresholds.MEDIUM then
        return "MEDIUM"
    elseif threat_score >= thresholds.LOW then
        return "LOW"
    else
        return "SAFE"
    end
end

-- Lấy thông tin ngưỡng hiện tại (nếu muốn đọc từ Lua)
function Get_thresholds()
    return thresholds
end
