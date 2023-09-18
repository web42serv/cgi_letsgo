#!/usr/bin/python3

import cgi
import cgitb

# 에러 메시지를 브라우저에 표시
cgitb.enable()

print("Content-type: text/html\n")
print("<html>")
print("<head><title>Simple CGI with POST</title></head>")
print("<body>")

# POST 요청으로부터 데이터를 읽어옴
form = cgi.FieldStorage()

if "a" in form and "b" in form:
    try:
        a = int(form["a"].value)
        b = int(form["b"].value)
        result = a * b
        print("<h1>Result of {} * {} is: {}</h1>".format(a, b, result))
    except ValueError:
        print("<h1>Invalid input. Please enter valid numbers for 'a' and 'b'.</h1>")
else:
    print("insert a and b")

# POST 메서드를 사용하기 때문에 데이터를 입력할 수 있는 폼을 생성합니다.
print("<form method='post' action='/cgipost.py'>")
print("Enter value for 'a': <input type='text' name='a'><br>")
print("Enter value for 'b': <input type='text' name='b'><br>")
print("<input type='submit' value='Submit'>")
print("</form>")

print("</body>")
print("</html>")
