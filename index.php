<head>
  <title>libQGLViewer</title>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
  <link href="../qglviewer.css" rel="stylesheet" type="text/css" />
  <link rel="shortcut icon" href="../images/qglviewer.ico" type="image/x-icon" />
  <link rel="icon" href="../images/qglviewer.icon.png" type="image/png" />
<script type="text/javascript">

  var _gaq = _gaq || [];
  _gaq.push(['_setAccount', 'UA-23223012-2']);
  _gaq.push(['_trackPageview']);

  (function() {
    var ga = document.createElement('script'); ga.type = 'text/javascript'; ga.async = true;
    ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';
    var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(ga, s);
  })();

</script>
</head>
<body>

<div class="banner">
  <a class="qindex highlight" href="../index.html">Home</a>
  <a class="qindex" href="../download.html">Download</a>
  <a class="qindex" href="../examples/index.html">Examples</a>
  <a class="qindex" href="../refManual/hierarchy.html">Documentation</a>
  <a class="qindex" href="../developer.html">Developer</a>
</div>

<br/>

<h1>File listing</h1>
<table style="margin:auto; border:1px solid #DDD">

<?php

$dir=".";

if(is_dir($dir))
{
   $files = array();

   $scan = scandir($dir);
   for ($i=0;$i<count($scan);$i++) {
      if ($scan[$i] != "." && $scan[$i] != ".." && $scan[$i] != "index.php")
        $files[] = $scan[$i];
    }
    
    sort($files);

    for ($i=0;$i<count($files);$i++) {
        echo '<tr><td><a href="'.$files[$i].'" onclick="javascript: _gaq.push([\'_trackPageview\', \''.$files[$i].'\']);">'.$files[$i].'</a><td><td>'.(int)(filesize($files[$i])/1024).' kb</td>'."\n";
    }
}

?>

</table>
</body>
</html>
