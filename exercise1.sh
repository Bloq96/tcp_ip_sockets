for i in {0..9}; do
    konsole -e "bash -c 'bin/connectTo;bash'" &
    disown
done
