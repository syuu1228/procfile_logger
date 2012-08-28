require 'csv'

if ARGV.size < 6
	puts "sum_row.rb [percpu_logs] [sum_logs] [offset] [nrows] [header] [nfiles]"
	exit 1
end

NFILES=ARGV[5].to_i
OFFSET=ARGV[2].to_i
NROWS=ARGV[3].to_i
percpu_logs = []
sum_logs = []

(0..NFILES - 1).each do |cpu|
	percpu_logs[cpu] = CSV.open(sprintf(ARGV[0], cpu))
	percpu_logs[cpu].gets
	sum_logs[cpu] = File.open(sprintf(ARGV[1], cpu), mode = "w")
	sum_logs[cpu] << ARGV[4] << "\n"
end

while true
	if percpu_logs[0].eof?
		break
	end
	(0..NFILES - 1).each do |cpu|
		sum = 0
		lines = percpu_logs[cpu].gets
		lines.delete_at(lines.size - 1) if (lines[lines.size - 1] == nil)
		(OFFSET..OFFSET + NROWS - 1).each do |i|
			sum += lines[i].to_i
		end
		sum_logs[cpu] << sum << "\n"
	end
end

(0..NFILES - 1).each do |cpu|
	sum_logs[cpu].close
end
