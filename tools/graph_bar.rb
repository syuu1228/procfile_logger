require 'csv'
require 'rubygems'
require 'gruff'

if ARGV.size < 4
	puts "graph_bar.rb [title] [output] [file0,file1,file2..] [name0,name1,name2...]"
	exit 1
end

title = ARGV[0]
out = ARGV[1]
logpaths = ARGV[2].split(",")
names = ARGV[3].split(",")
logs = []
header = []
data = []

(0..logpaths.size - 1).each do |i|
	logs[i] = CSV.open(logpaths[i])
	if i == 0
		header = logs[i].gets
	else
		logs[i].gets
	end
end
header.delete_at(header.size - 1) if header[header.size - 1] == nil
max = Array.new(header.size, 0)
(0..logs.size - 1).each do |i|
	data[i] = []
	line = logs[i].gets
	line.delete_at(line.size - 1) if line[line.size - 1] == nil
	(0..line.size - 1).each do |j|
		v = line[j].to_i
		data[i][j] = v
		max[j] = v if max[j] < v
	end
	logs[i].close
end

(0..header.size - 1).each do |j|
	g = Gruff::Bar.new
	g.title = title
	g.sort = false
	g.minimum_value = 0
	g.maximum_value = max[j]
	(0..logs.size - 1).each do |i|
		g.data(names[i], data[i][j])
	end
	g.labels = {0 => header[j]}
	g.write("#{out}_#{header[j]}.png")
end
