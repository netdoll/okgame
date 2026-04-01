-- okgame initialization script
log("Initializing advanced Lua mod with Audio and Sprite support...")

-- Example of using engine bindings
local width = getGridWidth()
local height = getGridHeight()
log("Grid size: " .. tostring(width) .. "x" .. tostring(height))

local frameCount = 0
local lastScore = 0

-- Function called by the engine every frame
function onUpdate()
    frameCount = frameCount + 1
    
    -- Every 600 frames (approx 10 seconds at 60fps), trigger an effect
    if frameCount % 600 == 0 then
        log("10 second check-in from Lua!")
        wiggleScreen()
        
        -- Play a subtle sound every 10 seconds
        playSound("cursor", 0.5)
    end
    
    -- Check for score changes
    local currentScore = getScore()
    if currentScore > lastScore then
        local diff = currentScore - lastScore
        log("Score increased by " .. tostring(diff) .. "! Total: " .. tostring(currentScore))
        
        -- Reward high score increases with a screen shake and sound
        if diff >= 100 then
            log("Big score! Shaking screen and playing level-up sound.")
            shakeScreen()
            playSound("clear", 1.0)
        end
        
        lastScore = currentScore
    end
end

-- Function to demonstrate screen effects and audio
function testEffects()
    log("Triggering start-up effects and sound from Lua")
    shakeScreen()
    playSound("start", 1.0)
    
    -- Create a decorative overlay sprite if asset exists
    -- createScreenSprite("lua_logo", "bobsGameLogoSmall", 10, 10)
end

-- Call test function
testEffects()

log("Advanced Lua mod initialized successfully.")
