node {
    // Volume (cm3)
    Number vol=0;
    // Flow has positive value: true for inspiration/insufflation, false for expiration/exsufflation.
    bool posFlow = true;
    // Previous value of posFlow.
    bool posFlowPrev = true;
    // Previous flow measurement time (ms).
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

        // Check for reset
        bool rst = isInputDirty<input_RST>(ctx);

        TimeMs mt = transactionTime();
        auto sensor = getValue<input_DEV>(ctx);

        // soft reset
        if (rst) {
            sensor->sendCommand(SOFT_RESET);
            delay(100);
            sensor->begin();
        }

        // Setting up. If we start measuring during an inspiration/insufflation or expiration/exsufflation,
        // the first measurement will be inaccurate. The following code makes the program wait for an
        // expiration/exsufflation before starting measurements.
        if (isSettingUp() || rst){
            uint8_t flow_dir_changes = 0;
            while (flow_dir_changes<2) {
                // measure flow
                flow = sensor->readFlow();
                // check direction of flow
                posFlow = 0 < round(flow);
                if (posFlowPrev!=posFlow) {
                    flow_dir_changes += 1;
                    if(posFlowPrev) { 
                        ins_t_prev=mt;
                    }
                    posFlowPrev=posFlow;
                }
                delay(20);
            }
            return;
        }
        
        // measure flow
        flow = sensor->readFlow();
        // check direction of flow
        posFlow = 0 < round(flow); // round to prevent small fluctuations around zero from changing value of posFlow

        // check for change of flow direction
        if (posFlowPrev!=posFlow) {
            // if direction of flow has changed
            if(posFlowPrev) {
                emitValue<output_iVOL>(ctx, vol);
                emitValue<output_INS>(ctx, 1);
                ins_t=mt;
                //TimeMs resp_delta = ins_t-ins_t_prev;
                Number resp_rate = 60/((ins_t-ins_t_prev)/1000.0);
                emitValue<output_RR>(ctx, resp_rate);
                ins_t_prev=ins_t;
            } else {
                emitValue<output_eVOL>(ctx, -vol);
                emitValue<output_EXP>(ctx, 1);
            }
            posFlowPrev=posFlow;
            // reset volume
            vol = 0;
        }

        TimeMs mt_delta = mt - mt_prev;
        mt_prev = mt;
        vol += flow/60*mt_delta;

        emitValue<output_FR>(ctx, flow);
        emitValue<output_DONE>(ctx, 1);
    }
}
