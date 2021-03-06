
node {

    void evaluate(Context ctx) {
        // Measure flow rate only if there is an input pulse.
        if (!isInputDirty<input_UPD>(ctx))
            return;

        auto sensor = getValue<input_DEV>(ctx);

        emitValue<output_FR>(ctx, sensor->readFlow());
        emitValue<output_DONE>(ctx, 1);
    }
}
