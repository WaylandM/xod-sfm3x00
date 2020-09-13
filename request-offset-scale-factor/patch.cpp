
node {
    void evaluate(Context ctx) {
        if (!isInputDirty<input_UPD>(ctx))
            return;

        auto sensor = getValue<input_DEV>(ctx);

        emitValue<output_Offset>(ctx, sensor->requestOffset());
        emitValue<output_Scale>(ctx, sensor->requestScaleFactor());
        emitValue<output_DONE>(ctx, 1);
    }
}
