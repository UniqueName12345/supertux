import sys
import re

pattern = re.compile(r"(translate|_)\((.*?)\)")

lisp_template_begin = """(msgids
""";
lisp_template_end = ")"

lisp_content = lisp_template_begin

if (len(sys.argv) < 3):
	print(f"Usage: {sys.argv[0]} <filename> <output file>")
else:
	filename = sys.argv[1]
	out_file = sys.argv[2]
	with open(filename, 'r') as f:
		s = f.read()
	matches = pattern.findall(s)
	strings_found = len(matches) > 0
	for match in matches:
		lines = match[1].split("\\n")
		num_lines = len(lines)
		if num_lines == 1:
			lisp_content += "  (msgid "
			lisp_content += f"(_ {match[1]})"
			lisp_content += ")\r\n"
		else:
			lisp_content += "  (msgid (_ "
			for i, line in enumerate(lines):
				lisp_content += line
				if i < num_lines:
					if i == num_lines - 1:
						lisp_content += "))"
					lisp_content += "\n"

	lisp_content += lisp_template_end

	if strings_found:
		with open(out_file, "w") as output_file:
			output_file.write(lisp_content)
