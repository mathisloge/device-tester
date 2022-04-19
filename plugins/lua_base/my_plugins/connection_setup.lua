local pb = require "pb"
local protoc = require "protoc"

protoc.new():load([[
        syntax = "proto3";

        message LedCtrlSetColor {
            int32 r = 1;
            int32 g = 2;
            int32 b = 3;
        }
    ]])

function sendLedColor(rgb_color)
    local data = {
        r = math.floor(rgb_color[1] * 255),
        g = math.floor(rgb_color[2] * 255),
        b = math.floor(rgb_color[3] * 255)
    }
    local bytes = pb.encode("LedCtrlSetColor", data)
    -- print(pb.tohex(bytes))
    ledclient:put(1, "test", bytes)
end

function onCoapReceive(id, data)
    -- print("Received ", id)
end

local colors = {0.0, 0.1, 0.2}
local counter = 1.0

local testdata = implot.ScrollingBuffer.new(1000)
function draw_set_color()
    testdata:addPoint(counter, math.sin(counter / 100.0))
    counter = counter + 1

    imgui.text("Select the color the leds should have:")
    local changed = false
    changed, colors = imgui.colorPicker3("Color", colors)
    if changed then
        sendLedColor(colors)
    end

    if imgui.button("Set") then
        print("SEND")
        sendLedColor(colors)
    end

    if implot.beginPlot("TEST") then
        implot.setupAxisLimits(0, counter - 1000, counter, 1)
        implot.setupAxisLimits(3, -1, 1, 0)
        testdata:plotLine("MYLINE")
        implot.endPlot()
    end
end

plugin = {
    name = "Test Connection Setup",
    description = "Protocol for my own LEDs.",
    connections = {
        ledclient = {
            name = "LED Client",
            type = "coap-client",
            ip = "fe80::7ae3:6dff:fe09:8188",
            port = 5683,
            onReceive = onCoapReceive
        },
        serialdebug = {
            name = "LED Debug",
            type = "serial",
            port = "COM3",
            baudrate = 115200
        }
    },
    actions = {
        set_color = {
            name = "Set LED Color",
            description = "sets the led color to the specified rgb values",
            gui = draw_set_color
        }
    }
}

function coap_client:printme()
    print(self)
    print("yes!")
end

function setup()
    print("setup")
    ledclient:printme()
end
