<html>
 <head>
  <title>Test PHP</title>
 </head>
 <body>
 <?php echo '<p>Bonjour le monde</p>'; ?>
 <?php echo 'Bonjour ' . htmlspecialchars($_POST["name"]) . '!';?>
 </body>
</html>