#!/usr/bin/python3

import cgi
import cgitb
cgitb.enable()

form = cgi.FieldStorage()

if "id" in form and "pw" in form:
    id = form["id"].value
    pw = form["pw"].value
    id_list = ["subcho", "hunpark", "wonlgeon", "yeselee", "yham", "isunwoo"]
    if id in id_list and pw == "1234":
        print("<html>")
        print("<head>")
        print("<title>login success</title>")
        print("<style>")
        print("body {")
        print("    font-family: 'Nanum Gothic', sans-serif;")
        print("    background-color: #FFFFFF;")
        print("    margin: 0;")
        print("    padding: 0;")
        print("}")
        print("h1 {")
        print("    text-align: center;")
        print("    background-color: #000000;")
        print("    color: #fff;")
        print("    padding: 20px 0;")
        print("}")
        print("</style>")
        print("</head>")
        print("<body>")
        print("<h1>hello {}</h1>".format(id))
        print("<p style=\"text-align: center;\">No XX, keep coding...</p>")
        print("</body>")
        print("</html>")
    else:
        print("<meta charset=\"utf-8\">")
        print("<html>")
        print("<head>")
        print("<title>login failed</title>")
        print("<style>")
        print("body {")
        print("    font-family: 'Nanum Gothic', sans-serif;")
        print("    background-color: #FFFFFF;")
        print("    margin: 0;")
        print("    padding: 0;")
        print("}")
        print("h1 {")
        print("    text-align: center;")
        print("    background-color: #000000;")
        print("    color: #fff;")
        print("    padding: 20px 0;")
        print("}")
        print("</style>")
        print("</head>")
        print("<body>")
        print("<h1>{} is not our team or inserted wrong password :(</h1>".format(id))
        print("<p style=\"text-align: center;\">이거 보여주려고 어그로 끌었다 미안하다</p>")
        print("</body>")
        print("</html>")
else:
    print("<html>")
    print("<head><title>Fail response</title></head>")
    print("<body>")
    print("<h1>Null Field Error</h1>")
    print("</body>")
    print("</html>")
