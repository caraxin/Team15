from collections import defaultdict

def parse_coverage():
	f = open('coverage_results', 'r')

	coverage = {}

	current_result = ""
	percent = [None] * 2
	for line in f:
		if len(line) == 1:
			current_result = ""
			percent = [None] * 2
			continue;

		if line[0] == "F":
			current_result = line[5:]

		if line[0] == "L":
			percent_pos = line.find('%')
			percent[0] = line[15:percent_pos]
			percent[1] = line[percent_pos + 4:]

		if line[0] == "C":
			if coverage.get(current_result, [0,0]) == [0,0]:
				coverage[current_result] = percent
			else:
				if float(percent[0]) > float(coverage[current_result][0]):
					coverage[current_result] = percent

	for result in coverage:
		print result, coverage[result]


parse_coverage()

