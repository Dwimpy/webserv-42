#!/usr/local/bin/python3.9

import sys
import os

try:
    # Read a single line from stdin (the filename)
    filename = sys.stdin.readline().strip()  # Use strip() to remove newline character

    # Test if the file was uploaded
    if filename:
        # Strip leading path from file name to avoid directory traversal attacks
        fn = os.path.basename(filename)
        print(filename)
        if os.path.exists(filename):
            content = 'Success!'
        else:
            content = 'File is not uploaded! Cant find file'
    else:
        content = 'File is not uploaded! No filename as input'
except:
    content = 'File is not uploaded! Unexpected Error'

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