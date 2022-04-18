
function sendLedColor(rgb_color)
    local msg_data = {}
    print("PUT")
    ledclient:put(1, "test", msg_data)
end


function onCoapReceive(id, data)
    print("Received ")
    print("ID: ", id)
    print("SIZE: ", data:size())
end

local colors = {0.0, 0.1, 0.2} 
function draw_set_color()
    imgui.text("Select the color the leds should have:")
    colors = imgui.colorEdit3("Color", colors)

    if imgui.button("Set") then    
        print("SEND")    
        sendLedColor(colors)
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
