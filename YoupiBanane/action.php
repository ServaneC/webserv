<html>
 <head>
  <title>Test PHP</title>
  <!-- <link rel="stylesheet" type="text/css" href="/static/style1.css"> -->
 </head>
 <body>
 <?php echo '<p>Bonjour le monde</p>'; ?>
 <p> Bonjour, <?php echo htmlspecialchars($_POST['nom']); ?>.
    Tu as <?php echo (int)$_POST['age']; ?> ans.</p>

 </body>
</html>