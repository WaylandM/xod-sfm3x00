
node {

    Number vol; // Volume (cm3)
    bool posFlow = false; // Flow has positive value: true for inspiration, false for expiration
    bool posFlowPrev = false; // Previous value of posFlow
    TimeMs mt_prev = transactionTime();
    TimeMs ins_prev = mt_prev;
    
    void evaluate(Context ctx) {
        
        // Measure flow rate only if there is an input pulse.
        if (!isInputDirty<input_UPD>(ctx))
            return;

        TimeMs mt = transactionTime();
        auto sensor = getValue<input_DEV>(ctx);
        Number flow = sensor->readFlow();

        // check direction of flow
        posFlow = 0 < round(flow); // round to prevent small fluctuations from changing value of posFlow

        if (isSettingUp()){
            posFlowPrev=posFlow;
            vol=0;
            return;
        }
        
        if (posFlowPrev!=posFlow) { // change of flow direction
            // if direction of flow has changed
            if(posFlowPrev) {
                emitValue<output_iVOL>(ctx, vol);
                emitValue<output_INS>(ctx, 1);
                emitValue<output_RR>(ctx, 60/(mt-ins_prev));
                ins_prev=mt;
            } else {
                emitValue<output_eVOL>(ctx, -vol);
                emitValue<output_EXP>(ctx, 1);
            }
            posFlowPrev=posFlow;
            vol = 0; // reset the volume
        }

        TimeMs mt_delta = mt - mt_prev;
        mt_prev = mt;
        vol += flow/60*mt_delta;

        emitValue<output_FR>(ctx, flow);
        emitValue<output_DONE>(ctx, 1);
    }
}
