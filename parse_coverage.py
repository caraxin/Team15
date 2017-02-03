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

	print_coverage_table(coverage)


def print_coverage_table(coverage):

	line_count = 0
	covered_count = 0

	for x in coverage:
		if 'googletest' in x or 'config_parser' in x:
			continue

		print "File:", x
		print "\tCoverage Percentage", coverage[x][0], "; Lines", coverage[x][1]

		line_count += int(coverage[x][1])
		covered_count += float(coverage[x][0]) * float(coverage[x][1])

	print "Overall Coverage:", float(covered_count) / float(line_count)
	print "Total Lines:", line_count






parse_coverage()

