
node {

    void evaluate(Context ctx) {

        if (!isInputDirty<input_UPD>(ctx))
            return;

        auto sensor = getValue<input_DEV>(ctx);


        sensor->startContinuousMeasurement();
        emitValue<output_SLM>(ctx, sensor->readFlow());

        sensor->sendCommand(START_TEMPERATURE_MEASUREMENT);
        emitValue<output_T_raw>(ctx, sensor->readData());

        emitValue<output_DONE>(ctx, 1);
    }
}
