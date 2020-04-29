for i in {40..41}
do
	./zipf -f mini_zipfian_values_${i}.txt -r 1 -a 0.${i} -n 131072 -N 524288 | tee output/mini_cumul_${i}.txt
done
