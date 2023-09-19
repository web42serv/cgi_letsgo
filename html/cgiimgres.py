#!/usr/bin/python3
import cgi, cgitb
import os

# Enable detailed error messages in the browser
cgitb.enable()

# Create a FieldStorage object to parse the form data
form = cgi.FieldStorage()
print(form)
# Check if the form was submitted and if it contains a file named "filename"
if "filename" in form:
    # Get the file item from the form
    fileitem = form["filename"]

    # Check if the file was uploaded successfully
    if fileitem.filename:
        # Strip leading path from file name to avoid directory traversal attacks
        fn = os.path.basename(fileitem.filename)

        # Define the path to save the uploaded file
        save_path = "/img/" + fn

        # Write the file content to the specified path
        with open(save_path, "wb") as f:  # Open in binary mode ("wb")
            f.write(fileitem.file.read())

        # Display a success message
        print("<html>")
        print("<head><title>Upload Result</title></head>")
        print("<body>")
        print("<h1>File uploaded successfully!</h1>")
        print("<p>Filename: {}</p>".format(fn))
        print("</body>")
        print("</html>")
    else:
        print("<html>")
        print("<head><title>Upload Result</title></head>")
        print("<body>")
        print("<h1>No file was uploaded</h1>")
        print("</body>")
        print("</html>")
else:
    print("<html>")
    print("<head><title>Upload Result</title></head>")
    print("<body>")
    print("<h1>Filename not found in form</h1>")
    print("</body>")
    print("</html>")