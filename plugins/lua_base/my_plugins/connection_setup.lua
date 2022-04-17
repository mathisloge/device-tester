plugin = {
    name = "Test Connection Setup",
    description = "Protocol for my own LEDs.",
    connection = {
        ledclient = {
            name = "LED Client",
            type = "coap",
            ip = "fe80::7ae3:6dff:fe09:8188",
            port = "5683"
        }, 
        serialdebug = {
            name = "LED Debug", 
            type = "serial",
            port = "COM3",
            baudrate = 115200
        }
    }
}
