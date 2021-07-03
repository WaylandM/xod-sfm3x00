node {
    // Volume (cm3)
    Number vol=0;
    // Inspiratory flow: true during inspiration/insufflation.
    bool inspFlow = true;
    // Previous value of inspFlow.
    bool inspFlowPrev = true;
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

        auto thresh = getValue<input_IT>(ctx);

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
                // Measure flow
                flow = sensor->readFlow();
                // Check to see if rate of flow is above user-defined inspiratory threshold.
                inspFlow = round(flow) > thresh;
                if (inspFlowPrev!=inspFlow) {
                    flow_dir_changes += 1;
                    if(inspFlowPrev) {
                        ins_t_prev=mt;
                    }
                    inspFlowPrev=inspFlow;
                }
                delay(20);
            }
            return;
        }

        // Measure flow rate, volume and respiratory rate.
        flow = sensor->readFlow();
        inspFlow = round(flow) > thresh;

        // check for change of flow direction
        if (inspFlowPrev!=inspFlow) {
            // if direction of flow has changed
            if(inspFlowPrev) {
                emitValue<output_iVOL>(ctx, vol);
                emitValue<output_INS>(ctx, 1);
                ins_t=mt;
                // Calculate respiratory rate
                Number resp_rate = 60/((ins_t-ins_t_prev)/1000.0);
                emitValue<output_RR>(ctx, resp_rate);
                ins_t_prev=ins_t;
            } else {
                emitValue<output_EXP>(ctx, 1);
            }
            inspFlowPrev=inspFlow;
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
