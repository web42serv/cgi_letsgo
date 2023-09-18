#!/usr/bin/python3

import cgi

print("Content-type: text/html\n")
print("<html>")
print("<head><title>MAIN</title></head>")
print("<body>")
print("Insert a name")
print("<form method='post' action='/cgipostres.py'>")
print("Enter a name: <input type='text' name='name'><br>")
print("<input type='submit' value='name submit'>")
print("</form>")
print("</body>")
print("</html>")
