
node {

    void evaluate(Context ctx) {
        auto sensor = getValue<input_DEV>(ctx);
        emitValue<output_SN>(ctx, sensor->requestSerialNumber());
    }
}
