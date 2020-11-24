
node {

    void evaluate(Context ctx) {
        auto sensor = getValue<input_DEV>(ctx);
        emitValue<output_ART>(ctx, sensor->requestArticleNumber());
    }
}
