#!/usr/bin/python3

import cgi

print("<html>")
print("<head><title>MAIN</title></head>")
print("<body>")
print("Upload a file")
print("<form enctype='multipart/form-data' action='/cgiimgres.py' method='post'>")
print("File: <input type='file' name='filename'><br>")
print("<input type='submit' value='Upload'>")
print("</form>")
print("</body>")
print("</html>")