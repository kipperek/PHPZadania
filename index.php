<?php
	session_start();
?>
<!DOCTYPE html>
<html>
	<head>
		<meta http-equiv="content-type" content="text/html; charset=utf-8">
		<LINK rel="stylesheet" type="text/css" href="style.css" title="Default Styles" media="screen">
		<title>PDO - zadanie nr 2</title>		
		
	</head>
	<body>

		<?php
			include "zadanie.inc";
			include "db.inc";
			include "login.inc";
			include "kategorie.inc";
			
			$user =  new LogIn();
			$user->manageUser();
			
			$zadania = new ZadaniaManager();
			$przedmioty = new PrzedmiotyManager();
		?>
		
		<div id="content">
			<div class='menuDiv'>
			<?php
				$przedmioty->showPrzedmioty();
			?>
			</div>
			<div class='tableDiv'>
			<?php
				$przedmioty->showForm();
				$zadania->printTable();	
			?>
			</div>
		</div>
		
		
	</body>
</html>