
node {

    void evaluate(Context ctx) {
        // Measure flow rate only if there is an input pulse.
        if (!isInputDirty<input_UPD>(ctx))
            return;

        auto sensor = getValue<input_DEV>(ctx);

        emitValue<output_SLM>(ctx, sensor->readFlow() - getValue<input_FErr>(ctx));
        emitValue<output_DONE>(ctx, 1);
    }
}
