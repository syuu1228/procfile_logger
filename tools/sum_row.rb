require 'csv'

if ARGV.size < 4
	puts "sum_row.rb [percpu_logs] [sum_logs] [header] [nfiles]"
	exit 1
end

NFILES=ARGV[3].to_i
percpu_logs = []
sum_logs = []

for cpu in 0...NFILES do
	percpu_logs[cpu] = CSV.open(sprintf(ARGV[0], cpu))
	percpu_logs[cpu].gets
	sum_logs[cpu] = File.open(sprintf(ARGV[1], cpu), mode = "w")
	sum_logs[cpu] << ARGV[2] << "\n"
end

while true
	if percpu_logs[0].eof?
		break
	end
	for cpu in 0...NFILES do
		sum = 0
		percpu_logs[cpu].gets.each do |v|
			sum += v.to_i
		end
		sum_logs[cpu] << sum << "\n"
	end
end

for cpu in 0...NFILES do
	sum_logs[cpu].close
end
