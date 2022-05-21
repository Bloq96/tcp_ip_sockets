(
    time=0
    for i in {0..9}; do
        time=$(($time+5))
        xterm -e "bash -c 'timeout $time bin/connectTo $1 <$2';bash" &
        sleep 1
    done
);

