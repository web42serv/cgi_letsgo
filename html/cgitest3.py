#!/usr/bin/python3

import cgi

print("<html>")
print("<head><title>Simple Multiple CGI</title></head>")
print("<body> cgi")

form = cgi.FieldStorage()

if "a" in form and "b" in form:
    a = int(form["a"].value)
    b = int(form["b"].value)
    result = a * b
    print("<h1>result: {}!</h1>".format(result))
else:
    print("<h1>invalid parameter</h1>")

print("</body>")
print("</html>")
