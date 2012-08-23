require 'csv'

if ARGV.size < 2
	puts "sum_col.rb [log] [sum_log]"
	exit 1
end

sum = []
log = CSV.open(ARGV[0])
sum_log = File.open(ARGV[1], mode = "w")
sum_log << log.gets.to_csv << "\n"

while true
	if log.eof?
		break
	end
	line = log.gets
	(0..line.size - 1).each do |i|
		sum[i] = 0 if sum[i] == nil
		sum[i] += line[i].to_i
	end
end
sum.each do |v|
	sum_log << v << ","
end
sum_log << "\n"
sum_log.close
