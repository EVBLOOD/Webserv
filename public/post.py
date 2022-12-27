#!/usr/bin/python3

import cgi
import html

# Read the form data from standard input
form = cgi.FieldStorage()

# Get the form values
name = form.getvalue("name")
email = form.getvalue("email")


print("Content-type: text/html")
print("\r\n\r\n")

# Output the HTML content
print("<html><body>")
print("<h1>Form Results</h1>")
print("<p>Name: {}</p>".format(html.escape(name)))
print("<p>Email: {}</p>".format(html.escape(email)))
print("</body></html>")
