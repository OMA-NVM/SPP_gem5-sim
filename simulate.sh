#!/bin/bash

simulate() {
    export M5_PATH=.
    build/ARM/gem5.fast configs/example/fs.py \
    --mem-type=DDR3_1600_8x8 \
    --bare-metal --disk-image ./fake.iso \
    --kernel=cpu_tests/benchmarks/bin/arm/$1_gem5-arm64.dbg \
    --cpu-type=DerivO3CPU --machine-type=VExpress_GEM5_V2 --caches --l2cache \
    --l1i_size='32kB' --l1d_size='8kB' --l2_size='8kB' --dtb-filename=none \
    --mem-size=4GB #> gem5.terminal &
    #disown $(jobs -p)
}

#decode Traces generated from cacheConfig.py
traceCache() {
    util/decode_packet_trace.py m5out/cacheTraces.trc.gz cacheTrace
    mv cacheTrace /workspaces/gem5-sim/cpu_tests/benchmarks/traceFiles
}

#decode das andere
traceMem() {
    mv m5out/memTraces.trc.gz /workspaces/gem5-sim/cpu_tests/benchmarks/traceFiles/$1_memTrace.trc.gz
    util/decode_packet_trace.py /workspaces/gem5-sim/cpu_tests/benchmarks/traceFiles/$1_memTrace.trc.gz $1_memTrace
    mv $1_memTrace /workspaces/gem5-sim/cpu_tests/benchmarks/traceFiles
}

case $1 in
    simulate | s)
        simulate $2
    ;;
    cacheTrace | ct)
        traceCache $2
    ;;
    memTrace | mt)
        traceMem $2
    ;;

esac