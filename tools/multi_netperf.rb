require 'tempfile'
require 'csv'

if ARGV.size < 3
	puts "multi_netperf.rb [nprocs] [peer] [duration]"
	exit 1
end
number = ARGV[0].to_i
peer = ARGV[1]
duration = ARGV[2]
tempfile = Tempfile::new("netperf", "/tmp");

for i in 1..number do
	Process.fork do
		exec "netperf -v2 -H #{peer} -t TCP_RR -P 0 -c -C -l #{duration} -- -m 32K -M 32K -s 256K -S 256K >> #{tempfile.path}.#{i}"
	end
end
ret = Process.waitall
sum_lat = 0.0
sum_tps = 0.0
sum_loc_cpu = 0.0
sum_rem_cpu = 0.0
for i in 1..number do
	log = CSV.open("#{tempfile.path}.#{i}", mode = "r", options = {:col_sep => ' '})
	j = 0
	tps = 0.0
	lat = 0.0
	loc_cpu = 0.0
	rem_cpu = 0.0
	log.each do |row|
		if j == 1
			loc_cpu = row[6].to_f
			rem_cpu = row[7].to_f
		end
		if j < 6
			j += 1
			next		# scroll down to latency/tps record
		end
		lat = row[4].to_f
		tps = row[5].to_f
	end
	sum_lat += lat
	sum_tps += tps
	sum_loc_cpu += loc_cpu
	sum_rem_cpu += rem_cpu
end
tempfile.close
puts "avg_lat:#{sum_lat / number} Ktps:#{sum_tps / 1024} avg_loc_cpu:#{sum_loc_cpu / number} avg_rem_cpu:#{sum_rem_cpu / number}"
