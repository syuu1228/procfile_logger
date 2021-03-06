require 'csv'

if ARGV.size < 3
	puts "sum_cpus.rb [percpu_logs] [sum_log] [nfiles]"
	exit 1
end

NFILES=ARGV[2].to_i
percpu_logs = []
header = []

for cpu in 0...NFILES do
	percpu_logs[cpu] = CSV.open(sprintf(ARGV[0], cpu))
	if cpu == 0
		header = percpu_logs[cpu].gets
	else
		percpu_logs[cpu].gets
	end
end

header.delete_at(header.size - 1) if (header[header.size - 1] == nil)

sum_log = File.open(ARGV[1], mode = "w")
sum_log << header.to_csv

while true
	lines = []
	if percpu_logs[0].eof?
		break
	end
	for cpu in 0...NFILES do
		lines[cpu] = percpu_logs[cpu].gets
	end
	for j in 0...header.size do
		sum = 0
		for i in 0...NFILES do
			val = lines[i][j].to_i
			sum += val
		end
		sum_log << sum << ","
	end
	sum_log << "\n"
end
sum_log.close

