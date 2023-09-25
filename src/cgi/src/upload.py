#!/usr/local/bin/python3.9

# import cgi, os
# import sys


# try:
#     filename = sys.stdin.read()
#     # # message = parsed['name']
#     # # Test if the file was uploaded
#     if filename:
#         # strip leading path from file name to avoid
#         # directory traversal attacks
#         fn = os.path.basename(filename)
#         if (os.filename.exists(filename)):
#             content = 'File already exists!'
#         else:
#             content = 'Success!'
#     else:
#         content = 'File is not uploaded2!'
# except:
#     content = 'File is not uploaded!'
import sys
import os

try:
    # Read a single line from stdin (the filename)
    filename = sys.stdin.readline().strip()  # Use strip() to remove newline character

    # Test if the file was uploaded
    if filename:
        # Strip leading path from file name to avoid directory traversal attacks
        fn = os.path.basename(filename)
        if os.path.exists(filename):  # Fix the typo here
            content = 'Success!'
    else:
        content = 'File is not uploaded!'
except:
    content = 'File is not uploaded!'

print("""\
<html>
  <head>
    <title>Upload Status</title>
    <link rel="stylesheet" href="css/error.css">
  </head>
  <body>
    <p>Status of Upload</p>
    <h1>%s</h1>
    <a class="button" href="javascript:history.back()">Back</a>
  </body>
</html>
""" % (content))