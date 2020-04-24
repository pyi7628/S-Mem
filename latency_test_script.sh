#random read - measure latency

for i in {1..14}
do
	((num=8*${i}))
	./bench -w ${num} -m ${num} -r | tee output/randomRead_latency/output_${num}.txt

	cat output/randomRead_latency/output_${num}.txt |
		awk '/result:/{printf $2", " >> "randomRead_latency_output.csv"}'
	printf "\n" >> randomRead_latency_output.csv
done
