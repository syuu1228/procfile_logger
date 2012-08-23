require 'rubygems'
require 'facter'
require 'csv'

Facter.loadfacts
NCPUS = Facter.processorcount.to_i

if ARGV.size < 2
	puts "sum_cpu.rb [percpu_logs] [sum_log]"
	exit 1
end

percpu_logs = []
header = []

(0..NCPUS - 1).each do |cpu|
	percpu_logs[cpu] = CSV.open(sprintf(ARGV[0], cpu))
	if cpu == 0
		header = percpu_logs[cpu].gets
	else
		percpu_logs[cpu].gets
	end
end

sum_log = File.open(ARGV[1], mode = "w")
sum_log << header.to_csv

while true
	lines = []
	if percpu_logs[0].eof?
		break
	end
	(0..NCPUS - 1).each do |cpu|
		lines[cpu] = percpu_logs[cpu].gets
	end
	(0..lines[0].size - 1).each do |j|
		sum = 0
		(0..NCPUS - 1).each do |i|
			val = lines[i][j].to_i
			sum += val
		end
		sum_log << sum << ","
	end
	sum_log << "\n"
end
sum_log.close

