for i in {0..1}; do
    xterm -e "bash -c 'bin/sendTo;bash'" &
    disown
done
