for i in {0..9}; do
    xterm -e "bash -c 'bin/connectTo;bash'" &
    disown
done
