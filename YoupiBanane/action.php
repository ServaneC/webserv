<html>
 <head>
  <title>POST test</title>
  <link rel="stylesheet" type="text/css" href="/static/style1.css">
 </head>
 <body>
 <h1> <?php echo 'Hello World !'; ?> </h1>
 <p> Hello, <?php echo htmlspecialchars($_POST['nom']); ?>.
    You are <?php echo (int)$_POST['age']; ?> years old !</p>

 </body>
</html>