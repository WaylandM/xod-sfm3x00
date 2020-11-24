
node {
    void evaluate(Context ctx) {
        auto sensor = getValue<input_DEV>(ctx);
        emitValue<output_Offset>(ctx, sensor->requestOffset());
        emitValue<output_Scale>(ctx, sensor->requestScaleFactor());
    }
}
