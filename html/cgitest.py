#!/usr/bin/python3

import cgi

print("<html>")
print("<head><title>Simple CGI Example</title></head>")
print("<body> test")

# Get data from the form if it exists
# form = cgi.FieldStorage()

# # Check if 'name' field exists in the form
# if "name" in form:
#     name = form["name"].value
#     print("<h1>Hello, {}!</h1>".format(name))
# else:
#     print("<h1>Hello, World!</h1>")

# End the HTML content
print("</body>")
print("</html>")