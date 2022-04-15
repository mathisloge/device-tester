plugin = {
    name = "LED Protocol",
    description = "Protocol for my own LEDs.",
    data_generators = {
        first_message = testproto
    }
}

local pb = require "pb"
local protoc = require "protoc"

local p = protoc.new()

p.load [[
    syntax = "proto3";

    message LedCtrlSetColor {
        int32 r = 1;
        int32 g = 2;
        int32 b = 3;
    }
]]
