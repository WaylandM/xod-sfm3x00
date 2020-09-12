// Tell XOD where it could download the library:
#pragma XOD require "https://github.com/PubInv/SFM3X00"

//Include C++ libraries
{{#global}}
#include <Wire.h>
#include <SFM3X00.h>
{{/global}}



struct State {
    uint8_t mem[sizeof(SFM3X00)];
};

// Define our custom type as a pointer on the class instance.
using Type = SFM3X00*;

{{ GENERATED_CODE }}

void evaluate(Context ctx) {
    // It should be evaluated only once on the first (setup) transaction
    if (isSettingUp()) {
        auto state = getState(ctx);
        auto address = getValue<input_ADDR>(ctx);
        // Create a new object in the memory area reserved previously.
        Type sensor = new (state->mem) SFM3X00(address);
        Wire.begin();
        sensor->begin();
    }

    // Measure flow rate only if there is an input pulse.
    if (isInputDirty<input_UPD>(ctx)) {
        auto state = getState(ctx);
        auto sensor = reinterpret_cast<SFM3X00*>(state->mem);

        emitValue<output_SLM>(ctx, sensor->readFlow());

        emitValue<output_DONE>(ctx, 1);
    }
    

}
