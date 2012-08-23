require 'rubygems'
require 'facter'
require 'csv'

Facter.loadfacts
NCPUS = Facter.processorcount.to_i

if ARGV.size < 3
	puts "sum_row.rb [percpu_logs] [sum_logs] [header]"
	exit 1
end

percpu_logs = []
sum_logs = []

(0..NCPUS - 1).each do |cpu|
	percpu_logs[cpu] = CSV.open(sprintf(ARGV[0], cpu))
	percpu_logs[cpu].gets
	sum_logs[cpu] = File.open(sprintf(ARGV[1], cpu), mode = "w")
	sum_logs[cpu] << ARGV[2] << "\n"
end

while true
	if percpu_logs[0].eof?
		break
	end
	(0..NCPUS - 1).each do |cpu|
		sum = 0
		percpu_logs[cpu].gets.each do |v|
			sum += v.to_i
		end
		sum_logs[cpu] << sum << "\n"
	end
end

(0..NCPUS - 1).each do |cpu|
	sum_logs[cpu].close
end
