#!/usr/bin/env python

# Required for Python 2.x compatibility
# from __future__ import print_function

# Import the CGI module
import cgi

# Print the Content-Type header
# print("Content-Type: text/html\n")

# Start the HTML content
print("<html>")
print("<head><title>Simple CGI Example</title></head>")
print("<body>")

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
