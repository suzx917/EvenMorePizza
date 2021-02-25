for x in a b c d e; do
 ./pizza < $x.in > $x.out ? &
done

wait