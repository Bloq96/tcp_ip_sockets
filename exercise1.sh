(
    time=0
    for i in {0..9}; do
        time=$(($time+10))
        echo $time
        timeout -k $time $time bin/connectTo $1 <$2 &
    done
);

