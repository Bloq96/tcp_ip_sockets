for i in {0..1}; do
    konsole -e "bash -c 'bin/sendTo;bash'" &
    disown
done
