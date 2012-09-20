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

for i in 0...logpaths.size do
	logs[i] = CSV.open(logpaths[i])
	if i == 0
		header = logs[i].gets
	else
		logs[i].gets
	end
end
header.delete_at(header.size - 1) if header[header.size - 1] == nil
max = Array.new(header.size, 0)
for i in 0...logs.size do
	data[i] = []
	line = logs[i].gets
	line.delete_at(line.size - 1) if line[line.size - 1] == nil
	for j in 0...line.size do
		v = line[j].to_i
		data[i][j] = v
		max[j] = v if max[j] < v
	end
	logs[i].close
end

for j in 0...header.size do
	g = Gruff::Bar.new
	g.title = title
	g.sort = false
	g.minimum_value = 0
	g.maximum_value = max[j]
	for i in 0...logs.size do
		g.data(names[i], data[i][j])
	end
	g.labels = {0 => header[j]}
	g.write("#{out}_#{header[j]}.png")
end
