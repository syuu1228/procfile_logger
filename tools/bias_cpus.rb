require 'csv'

if ARGV.size < 3
	puts "bias_cpus.rb [percpu_logs] [bias_log] [nfiles]"
	exit 1
end

NFILES=ARGV[2].to_i
percpu_logs = []
header = []

(0..NFILES - 1).each do |cpu|
	percpu_logs[cpu] = CSV.open(sprintf(ARGV[0], cpu))
	if cpu == 0
		header = percpu_logs[cpu].gets
	else
		percpu_logs[cpu].gets
	end
end

header.delete_at(header.size - 1) if (header[header.size - 1] == nil)

bias_log = File.open(ARGV[1], mode = "w")
bias_log << header.to_csv

while true
	lines = []
	if percpu_logs[0].eof?
		break
	end
	(0..NFILES - 1).each do |cpu|
		lines[cpu] = percpu_logs[cpu].gets
	end
	(0..header.size - 1).each do |j|
		min = -1
		max = 0
		(0..NFILES - 1).each do |i|
			val = lines[i][j].to_i
			min = val if min == -1 || min > val
			max = val if max < val
		end
		bias_log << max - min << ","
	end
	bias_log << "\n"
end
bias_log.close

