function onCoapReceive(id, data)
    print("Received ")
    print("ID: ", id)
    print("SIZE: ", data:size())
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
