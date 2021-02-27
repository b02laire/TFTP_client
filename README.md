<h1>TFTP_client </h1>
C implementation of a TFTP client.<br>
Use <code>puttftp </code> to send a file to a TFTP server and <code>gettftp </code> to download a file from said server. <br>
<h2>Syntax :</h2>
Both functions use the same syntax : <br>
<code>./gettftp IP_Adress:port filename </code>
<code>./puttftp IP_Adress:port filename </code>
<h3>Example :</h3>
  <code>./gettftp 192.168.1.62:69 wallpaper.png </code>
<h2>Documentation : </h2>
To better understand this code, please refer to RFC 1350:  
www.tools.ietf.org/html/rfc135
<h2>To do :</h2>
Implement a "mail" mode alongside "netasci" and "octet"
