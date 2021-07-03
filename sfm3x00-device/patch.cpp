// Tell XOD where it could download the library:
#pragma XOD require "https://github.com/WaylandM/MTW-SFM3X00"

//Include C++ libraries
#include <Wire.h>
#include <SFM3X00.h>

node {
    meta {
        using Type = SFM3X00*;
    }

    uint8_t mem[sizeof(SFM3X00)];

    void evaluate(Context ctx) {
        if (!isSettingUp()) return;

        auto address = getValue<input_ADDR>(ctx);
        auto wire = getValue<input_I2C>(ctx);

        Type sensor = new (mem) SFM3X00(address);

        wire->begin();
        sensor->begin();

        emitValue<output_DEV>(ctx, sensor);

    }
}
