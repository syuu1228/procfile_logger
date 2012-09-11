require 'erb'

puts ERB.new(DATA.read, nil, '-').result(binding)
__END__
<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="utf-8" />
	<title>percpu system loads - cpu<%= ARGV[0] %></title>
</head>
<body>
	<h4>Interrupts</h4>
	<img src="merged_interrupts<%= ARGV[0] %>_merged_interrupts.png" />
	<h4>Stat(system)</h4>
	<img src="stat<%= ARGV[0] %>_system.png" />
	<h4>Stat(user)</h4>
	<img src="stat<%= ARGV[0] %>_user.png" />
	<h4>Stat(softirq)</h4>
	<img src="stat<%= ARGV[0] %>_softirq.png" />
	<h4>Stat(idle)</h4>
	<img src="stat<%= ARGV[0] %>_idle.png" />
</body>
</html>
