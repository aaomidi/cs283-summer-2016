Lab 3 Part 3
------------

Amir Omidi
Systems Programming
Lab 3 Part 3
email: aao53@drexel.edu


This part of the lab consisted of writing a very simple web server using the mongoose library that can serve HTML files.

To compile this program use:
```
make build
```
To test how the program works:
```
make run
```

Example run:
```
amir@SKYWINDOWS:Path/P3$ ./p3.o -p 8081
Server running, press enter to exit.

Call from P1:
amir@SKYWINDOWS:Path/P1$ ./p1.o -h localhost -p 8081 -f /index.html
HTTP/1.1 200 OK
Cache: no-cache
Content-Type: text/plain
Content-Length: 96

<html>
<head>
    <title>Hi</title>
</head>
<body>
<h1>Test website.</h1>
</body>
</html>

Request method printed in P3:
Request method: GET
```
