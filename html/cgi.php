#!/usr/bin/php-cgi
<?php

// Print the Content-Type header
header("Content-Type: text/html");

// Start the HTML content
echo "<html>";
echo "<head><title>Simple CGI Example</title></head>";
echo "<body>";

// Get data from the form if it exists
if (isset($_GET["name"])) {
    $name = $_GET["name"];
    echo "<h1>Hello, $name!</h1>";
} else {
    echo "<h1>Hello, World!</h1>";
}

// End the HTML content
echo "</body>";
echo "</html>";
?>
