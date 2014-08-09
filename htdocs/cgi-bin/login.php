<html>
<head>
  <meta charset="UTF-8">
  <title>Log-in - CodePen</title>
  <link rel='stylesheet' href='http://codepen.io/assets/libs/fullpage/jquery-ui.css'>
    <link rel="stylesheet" href="../css/style.css" media="screen" type="text/css" />
</head>
<body>
<div class="login-card">
	<h1>
    <?php echo attempLogin($_POST['user'], $_POST['pass']);?>
	</h1>
</div>

<?php
function attempLogin($username, $password)
{
   $myfile = fopen("authorised_users.txt", "r") or die("Unable to open file!");
   
   while (($line = fgets($myfile)) != NULL) {
   		$userPass = preg_split ("/=/", $line);
   		if (strcmp($userPass[0],$username) == 0 && strcmp($userPass[1], $password) == 0)
   		{
   			fclose($myfile);
   			return "You are now logged in as " . $username . "!";;
   		}
   }

   fclose($myfile);
   return "Login Failed";
} 
?>

</body>
</html>
