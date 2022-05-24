(
    #time=0
    for i in {0..9}; do
        #time=$(($time+10))
        xterm -e "bash -c 'echo \"Hi from client $(($i+1))!\" | bin/connectTo $1;bash'" &
        sleep 1
    done
)
