node {
    // Volume (cm3)
    Number iVol=0;
    Number eVol=0;
    TimeMs mt_prev;
    // Time of latest inspiration (ms).
    TimeMs ins_t = 0;
    // Time of previous inspiration (ms).
    TimeMs ins_t_prev = 0;
    // Flow rate (standard litres per minute).
    // Flow has positive value for inspiration/insufflation, negative for expiration/exsufflation.
    Number flow;

    void evaluate(Context ctx) {

        // Measure flow rate only if there is an input pulse.
        if (!isInputDirty<input_UPD>(ctx))
            return;

        TimeMs mt = transactionTime();

        if (isSettingUp())
            return;

        auto sensor = getValue<input_DEV>(ctx);

        // soft reset
        if (isInputDirty<input_RST>(ctx)) {
            sensor->sendCommand(SOFT_RESET);
            delay(100);
            sensor->begin();
        }

        // Measure flow rate, volume and respiratory rate.
        flow = sensor->readFlow();

        TimeMs mt_delta = mt - mt_prev;
        mt_prev = mt;

        if (isInputDirty<input_RVol>(ctx)) {
            iVol=0;
            eVol=0;
        } else {
            if (getValue<input_VolEn>(ctx)){
                if (flow>0) {
                    iVol += flow/60*mt_delta;
                } else {
                    eVol += -flow/60*mt_delta;
                }
            }
        }

        emitValue<output_FR>(ctx, flow);
        emitValue<output_iVOL>(ctx, iVol);
        emitValue<output_eVOL>(ctx, eVol);
        emitValue<output_DONE>(ctx, 1);
    }
}
