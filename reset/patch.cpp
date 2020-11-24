
node {

    void evaluate(Context ctx) {
        // Measure flow rate only if there is an input pulse.
        if (!isInputDirty<input_RST>(ctx))
            return;

        auto sensor = getValue<input_DEV>(ctx);

        sensor->sendCommand(SOFT_RESET);
        delay(100);
        sensor->begin();

        emitValue<output_DONE>(ctx, 1);
    }
}
