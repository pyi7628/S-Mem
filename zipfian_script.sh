for i in {90..91}
do
	./zipf -f zipfian_values_${i}.txt -r 1 -a 0.${i} -n 1048576 -N 4194304 | tee output/cumul_${i}.txt
done
